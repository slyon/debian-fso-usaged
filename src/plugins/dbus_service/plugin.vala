/*
 * FSO Resource Controller DBus Service
 *
 * (C) 2009-2010 Michael 'Mickey' Lauer <mlauer@vanille-media.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

using GLib;
using Gee;

internal const string RESOURCE_INTERFACE = "org.freesmartphone.Resource";
internal const string CONFIG_SECTION = "fsousage";
internal const string DEFAULT_LOWLEVEL_MODULE = "kernel26";

internal const string FSO_IDLENOTIFIER_BUS   = "org.freesmartphone.odeviced";
internal const string FSO_IDLENOTIFIER_PATH  = "/org/freesmartphone/Device/IdleNotifier/0";
internal const string FSO_IDLENOTIFIER_IFACE = "org.freesmartphone.Device.IdleNotifier";

internal const string FSO_DBUS_RESOURCE_PROVIDER_SECTION = "D-BUS Service";
internal const string FSO_DBUS_RESOURCE_PROVIDER_KEY = "FSO-provides-resource";

namespace Usage {

/**
 * Controller class implementing org.freesmartphone.Usage API
 *
 * Note: Unfortunately we can't just use libfso-glib (FreeSmartphone.Usage interface)
 * here, since we need access to the dbus sender name (which modifies the interface signature).
 **/
[DBus (name = "org.freesmartphone.Usage")]
public class Controller : FsoFramework.AbstractObject
{
    private FsoFramework.Subsystem subsystem;

    private FsoUsage.LowLevel lowlevel;
    private bool debug_do_not_suspend;
    private bool debug_enable_on_startup;
    private bool disable_on_startup;
    private bool disable_on_shutdown;

    private HashMap<string,Resource> resources;

    dynamic DBus.Object dbus;
    dynamic DBus.Object idlenotifier;

    private FreeSmartphone.UsageSystemAction system_status;

    public Controller( FsoFramework.Subsystem subsystem )
    {
        this.system_status = (FreeSmartphone.UsageSystemAction) 12345; // UNKNOWN

        this.subsystem = subsystem;

        this.subsystem.registerServiceName( FsoFramework.Usage.ServiceDBusName );
        this.subsystem.registerServiceObject( FsoFramework.Usage.ServiceDBusName,
                                              FsoFramework.Usage.ServicePathPrefix, this );

        // debug option: should we really suspend?
        debug_do_not_suspend = config.boolValue( CONFIG_SECTION, "debug_do_not_suspend", false );
        // debug option: should we enable on startup?
        debug_enable_on_startup = config.boolValue( CONFIG_SECTION, "debug_enable_on_startup", false );

        var sync_resources_with_lifecycle = config.stringValue( CONFIG_SECTION, "sync_resources_with_lifecycle", "always" );
        disable_on_startup = ( sync_resources_with_lifecycle == "always" || sync_resources_with_lifecycle == "startup" );
        disable_on_shutdown = ( sync_resources_with_lifecycle == "always" || sync_resources_with_lifecycle == "shutdown" );

        // start listening for name owner changes
        dbusconn = ( (FsoFramework.DBusSubsystem)subsystem ).dbusConnection();
        dbus = dbusconn.get_object( DBus.DBUS_SERVICE_DBUS, DBus.DBUS_PATH_DBUS, DBus.DBUS_INTERFACE_DBUS );
        dbus.NameOwnerChanged += onNameOwnerChanged;

        // get handle to IdleNotifier
        idlenotifier = dbusconn.get_object( FSO_IDLENOTIFIER_BUS, FSO_IDLENOTIFIER_PATH, FSO_IDLENOTIFIER_IFACE );

        // init resources and low level helpers
        initResources();
        initLowlevel();
        scanForResourceProviders();

        // initial status
        Idle.add( () => {
            updateSystemStatus( FreeSmartphone.UsageSystemAction.ALIVE );
            return false;
        } );
    }

    public override string repr()
    {
        return @"<$(resources.size) R>";
    }

    private void initResources()
    {
        resources = new HashMap<string,Resource>( str_hash, str_equal );
    }

    private void initLowlevel()
    {
        // check preferred low level suspend/resume plugin and instanciate
        var lowleveltype = config.stringValue( CONFIG_SECTION, "lowlevel_type", DEFAULT_LOWLEVEL_MODULE );
        string typename = "none";

        switch ( lowleveltype )
        {
            case "kernel26":
                typename = "LowLevelKernel26";
                break;
            case "kernel26_staysalive":
                typename = "LowLevelKernel26_StaysAlive";
                break;
            case "openmoko":
                typename = "LowLevelOpenmoko";
                break;
            default:
                logger.warning( @"Invalid lowlevel_type $lowleveltype; suspend/resume will NOT be available!" );
                lowlevel = new FsoUsage.NullLowLevel();
                return;
        }

        if ( lowleveltype != "none" )
        {
            var lowlevelclass = Type.from_name( typename );
            if ( lowlevelclass == Type.INVALID  )
            {
                logger.warning( @"Can't find plugin for lowlevel_type $lowleveltype; suspend/resume will NOT be available!" );
                lowlevel = new FsoUsage.NullLowLevel();
                return;
            }

            lowlevel = Object.new( lowlevelclass ) as FsoUsage.LowLevel;
            logger.info( @"Ready. Using lowlevel plugin $lowleveltype to handle suspend/resume" );
        }
    }

    private void scanForResourceProviders()
    {
        assert( logger.debug( @"Scanning for resource providers in $(Config.DBUS_SYSTEM_SERVICE_DIR)" ) );

        var dir = GLib.Dir.open( Config.DBUS_SYSTEM_SERVICE_DIR );

        for ( var name = dir.read_name(); name != null; name = dir.read_name() )
        {
            if ( name.has_suffix( ".service" ) )
            {
                var smk = new FsoFramework.SmartKeyFile();
                if ( smk.loadFromFile( GLib.Path.build_filename( Config.DBUS_SYSTEM_SERVICE_DIR, name ) ) )
                {
                    var fsoresources = smk.stringListValue( FSO_DBUS_RESOURCE_PROVIDER_SECTION, FSO_DBUS_RESOURCE_PROVIDER_KEY, {} );
                    if ( fsoresources.length > 0 )
                    {
                        foreach ( var resource in fsoresources )
                        {
                            assert( logger.debug( @"Service $name claims to provide FSO resource $resource" ) );
                            if ( resource in resources.keys )
                            {
                                assert( logger.debug( @"Skipping resource $resource which has already been registered" ) );
                            }
                            else
                            {
                                var r = new Resource( resource, new DBus.BusName( name.replace( ".service", "" ) ), null ); // register as shadow resource
                                resources[resource] = r;
                            }
                        }
                    }
                    else
                    {
                        assert( logger.debug( @"Service $name does not provide any FSO resources" ) );
                    }
                }
            }
        }
    }

    private void onResourceAppearing( Resource r )
    {
        assert( logger.debug( @"Resource $(r.name) served by $(r.busname) @ $(r.objectpath) has just been registered" ) );
        this.resource_available( r.name, true ); // DBUS SIGNAL

        if ( debug_enable_on_startup )
        {
            try
            {
                r.enable();
            }
            catch ( FreeSmartphone.ResourceError e )
            {
                logger.warning( @"Error while trying to (initially) enable resource $(r.name): $(e.message)" );
            }
            catch ( DBus.Error e )
            {
                logger.warning( @"Error while trying to (initially) enable resource $(r.name): $(e.message)" );
            }
        }

        if ( disable_on_startup )
        {
            // initial status is disabled
            try
            {
                r.disable();
            }
            catch ( FreeSmartphone.ResourceError e )
            {
                logger.warning( @"Error while trying to (initially) disable resource $(r.name): $(e.message)" );
            }
            catch ( DBus.Error e )
            {
                logger.warning( @"Error while trying to (initially) disable resource $(r.name): $(e.message)" );
            }
        }
    }

    private void onResourceVanishing( Resource r )
    {
        assert( logger.debug( @"Resource $(r.name) served by $(r.busname) @ $(r.objectpath) has just been unregistered" ) );
        this.resource_available( r.name, false ); // DBUS SIGNAL
    }

    private void onNameOwnerChanged( dynamic DBus.Object obj, string name, string oldowner, string newowner )
    {
#if DEBUG
        debug( "name owner changed: %s (%s => %s)", name, oldowner, newowner );
#endif
        // we're only interested in services disappearing
        if ( newowner != "" )
            return;

        assert( logger.debug( "%s disappeared. checking whether resources are affected...".printf( name ) ) );

        //FIXME: Consider keeping the known busnames in a map as well, so we don't have to iterate through all values

        var resourcesToRemove = new Gee.HashSet<Resource>();

        foreach ( var r in resources.values )
        {
            // first, check whether the resource provider might have vanished
            if ( r.busname == name )
            {
                onResourceVanishing( r );
                resourcesToRemove.add( r );
            }
            // second, check whether it was one of the users
            else
            {
                if ( r.hasUser( name ) )
                {
                    r.delUser( name );
                }
            }
        }
        foreach ( var r in resourcesToRemove )
        {
            resources.remove( r.name );
        }
    }

    internal bool onIdleForSuspend()
    {
        var resourcesAlive = 0;
        foreach ( var r in resources.values )
        {
            if ( ( r.status != ResourceStatus.SUSPENDED ) && ( r.status != ResourceStatus.DISABLED ) )
            {
                logger.warning( @"Resource $(r.name) is not suspended nor disabled" );
                resourcesAlive++;
            }
        }
        if ( resourcesAlive > 0 )
        {
            logger.error( @"$resourcesAlive resources still alive :( Aborting Suspend!" );
            return false;
        }
        logger.info( ">>>>>>> KERNEL SUSPEND" );

        if ( !debug_do_not_suspend )
        {
            lowlevel.suspend();
        }
        else
        {
            assert( logger.debug( "Not really suspending as instructed per debug_do_not_suspend. Sleeping 5 seconds instead..." ) );
            Posix.sleep( 5 );
        }
        logger.info( "<<<<<<< KERNEL RESUME" );

        FsoUsage.ResumeReason reason = lowlevel.resume();
        logger.info( @"Resume reason seems to be $reason" );
        resumeAllResources();

        instance.updateSystemStatus( FreeSmartphone.UsageSystemAction.RESUME );

        var idlestate = lowlevel.isUserInitiated( reason ) ? "busy" : "idle";
        try
        {
            idlenotifier.SetState( idlestate );
        }
        catch ( DBus.Error e )
        {
            logger.error( @"DBus Error while talking to IdleNotifier: $(e.message)" );
        }

        instance.updateSystemStatus( FreeSmartphone.UsageSystemAction.ALIVE );

        return false; // MainLoop: Don't call again
    }

    internal Resource getResource( string name ) throws FreeSmartphone.UsageError, FreeSmartphone.Error
    {
        if ( system_status != FreeSmartphone.UsageSystemAction.ALIVE )
        {
            throw new FreeSmartphone.Error.INVALID_PARAMETER( @"System action $system_status in progress; please try again later." );
        }

        Resource r = resources[name];
        if ( r == null )
        {
            throw new FreeSmartphone.UsageError.RESOURCE_UNKNOWN( @"Resource $name had never been registered" );
        }

        assert( logger.debug( "Current users for %s = %s".printf( r.name, FsoFramework.StringHandling.stringListToString( r.allUsers() ) ) ) );

        return r;
    }

    public async void disableAllResources()
    {
        assert( logger.debug( "Disabling all resources..." ) );
        foreach ( var r in resources.values )
        {
            try
            {
                yield r.disable();
            }
            catch ( FreeSmartphone.ResourceError e1 )
            {
                logger.warning( @"Error while trying to disable resource $(r.name): $(e1.message)" );
            }
            catch ( DBus.Error e2 )
            {
                logger.warning( @"Error while trying to disable resource $(r.name): $(e2.message)" );
            }
        }
        assert( logger.debug( "... done" ) );
    }

    public async void suspendAllResources()
    {
        assert( logger.debug( "Suspending all resources..." ) );
        foreach ( var r in resources.values )
        {
            try
            {
                yield r.suspend();
            }
            catch ( FreeSmartphone.ResourceError e1 )
            {
                logger.warning( @"Error while trying to suspend resource $(r.name): $(e1.message)" );
            }
            catch ( DBus.Error e2 )
            {
                logger.warning( @"Error while trying to suspend resource $(r.name): $(e2.message)" );
            }
        }
        assert( logger.debug( "... done disabling." ) );
    }

    internal async void resumeAllResources()
    {
        assert( logger.debug( "Resuming all resources..." ) );
        foreach ( var r in resources.values )
        {
            try
            {
                yield r.resume();
            }
            catch ( FreeSmartphone.ResourceError e1 )
            {
                logger.warning( @"Error while trying to resume resource $(r.name): $(e1.message)" );
            }
            catch ( DBus.Error e2 )
            {
                logger.warning( @"Error while trying to resume resource $(r.name): $(e2.message)" );
            }
        }
        assert( logger.debug( "... done resuming." ) );
    }

    //
    // DBUS API (for providers)
    //
    public void register_resource( DBus.BusName sender, string name, DBus.ObjectPath path ) throws FreeSmartphone.UsageError, DBus.Error
    {
#if DEBUG
        debug( "register_resource called with parameters: %s %s %s", sender, name, path );
#endif
        if ( name in resources.keys )
        {
            // shadow resource?
            if ( resources[name].objectpath != null )
            {
                // no
                throw new FreeSmartphone.UsageError.RESOURCE_EXISTS( @"Resource $name already registered" );
            }
            else
            {
#if DEBUG
                debug( @"shadow $name will now be substituted with the real thing..." );
#endif
                resources[name].objectpath = path;
                resources[name].proxy = dbusconn.get_object( sender, path, RESOURCE_INTERFACE ) as FreeSmartphone.Resource;

                return;
            }
        }

        var r = new Resource( name, sender, path );
        resources[name] = r;

        onResourceAppearing( r );
    }

    public void unregister_resource( DBus.BusName sender, string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        var r = getResource( name );

        if ( r.busname != sender )
            throw new FreeSmartphone.UsageError.RESOURCE_UNKNOWN( @"Resource $name not yours" );

        onResourceVanishing( r );

        resources.remove( name );
    }

    internal void shutdownPlugin()
    {
        if ( disable_on_shutdown )
        {
            disableAllResources();
        }
    }

    public void updateSystemStatus( FreeSmartphone.UsageSystemAction action )
    {
        if ( action == system_status )
        {
            return;
        }

        system_status = action;
        this.system_action( action );
    }

    //
    // DBUS API (for consumers)
    //
    public async string get_resource_policy( string name ) throws FreeSmartphone.UsageError, FreeSmartphone.Error, DBus.Error
    {
        switch ( getResource( name ).policy )
        {
            case FreeSmartphone.UsageResourcePolicy.ENABLED:
                return "enabled";
            case FreeSmartphone.UsageResourcePolicy.DISABLED:
                return "disabled";
            case FreeSmartphone.UsageResourcePolicy.AUTO:
                return "auto";
            default:
                var error = "unknown resource policy value %d for resource %s".printf( getResource( name ).policy, name );
                logger.error( error );
                throw new FreeSmartphone.Error.INTERNAL_ERROR( error );
        }
    }

    //public void set_resource_policy( string name, FreeSmartphone.UsageResourcePolicy policy ) throws FreeSmartphone.UsageError, FreeSmartphone.Error, DBus.Error
    public async void set_resource_policy( string name, string policy ) throws FreeSmartphone.UsageError, FreeSmartphone.Error, DBus.Error
    {
        logger.debug( @"Set resource policy for $name to $policy" );
        var resource = getResource( name );

        switch ( policy )
        {
            case "enabled":
                yield resource.setPolicy( FreeSmartphone.UsageResourcePolicy.ENABLED );
                break;
            case "disabled":
                yield resource.setPolicy( FreeSmartphone.UsageResourcePolicy.DISABLED );
                break;
            case "auto":
                yield resource.setPolicy( FreeSmartphone.UsageResourcePolicy.AUTO );
                break;
            default:
                throw new FreeSmartphone.Error.INVALID_PARAMETER( "ResourcePolicy needs to be one of { \"enabled\", \"disabled\", \"auto\" }" );
                break;
        }
    }

    public async bool get_resource_state( string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        return getResource( name ).isEnabled();
    }

    public async string[] get_resource_users( string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        return getResource( name ).allUsers();
    }

    public async string[] list_resources() throws DBus.Error
    {
        string[] res = {};
        foreach ( var key in resources.keys )
            res += key;
        return res;
    }

    public async void request_resource( DBus.BusName sender, string name ) throws FreeSmartphone.ResourceError, FreeSmartphone.UsageError, DBus.Error
    {
        var cmd = new RequestResource( getResource( name ) );
        yield cmd.run( sender );
    }

    public async void release_resource( DBus.BusName sender, string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        var cmd = new ReleaseResource( getResource( name ) );
        yield cmd.run( sender );
    }

    public async void shutdown() throws DBus.Error
    {
        var cmd = new Shutdown();
        yield cmd.run();
    }

    public async void reboot() throws DBus.Error
    {
        var cmd = new Reboot();
        yield cmd.run();
    }

    public async void suspend() throws DBus.Error
    {
        var cmd = new Suspend();
        yield cmd.run();
    }

    public async void resume() throws DBus.Error
    {
        var cmd = new Resume();
        yield cmd.run();
    }

    // DBUS SIGNALS
    public signal void resource_available( string name, bool availability );
    public signal void resource_changed( string name, bool state, GLib.HashTable<string,GLib.Value?> attributes );
    public signal void system_action( FreeSmartphone.UsageSystemAction action );
}

} /* end namespace */

namespace Usage { public Usage.Controller instance; }
internal DBus.Connection dbusconn;

public static string fso_factory_function( FsoFramework.Subsystem subsystem ) throws Error
{
    Usage.instance = new Usage.Controller( subsystem );
    return "fsousage.dbus_service";
}

public static void fso_shutdown_function()
{
    Usage.instance.shutdownPlugin();
}

[ModuleInit]
public static void fso_register_function( TypeModule module )
{
    FsoFramework.theLogger.debug( "usage dbus_service fso_register_function()" );
}
