/*
 * Generic Resource Controller
 *
 * Written by Michael 'Mickey' Lauer <mlauer@vanille-media.de>
 * All Rights Reserved
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

namespace Usage
{
/**
 * Enum for resource status
 **/
public enum ResourceStatus
{
    UNKNOWN,
    ENABLING,
    ENABLED,
    SUSPENDING,
    SUSPENDED,
    RESUMING,
    DISABLING,
    DISABLED
}

/**
 * Helper class encapsulating a registered resource
 **/
public class Resource : Object
{
    public string name { get; set; }
    public DBus.BusName busname { get; set; }
    public DBus.ObjectPath objectpath { get; set; }
    public ResourceStatus status { get; set; }
    public FreeSmartphone.UsageResourcePolicy policy { get; set; }
    public ArrayList<string> users { get; set; }

    private FreeSmartphone.Resource proxy;

    // called before deserializing, init all non-value types here
    construct
    {
        this.users = new ArrayList<string>( str_equal );
    }

    public Resource( string name, DBus.BusName busname, DBus.ObjectPath objectpath )
    {
        this.name = name;
        this.busname = busname;
        this.objectpath = objectpath;
        this.status = ResourceStatus.UNKNOWN;
        this.policy = FreeSmartphone.UsageResourcePolicy.AUTO;

        proxy = dbusconn.get_object( busname, objectpath, RESOURCE_INTERFACE ) as FreeSmartphone.Resource;
        // workaround until vala 0.7.4
        proxy.ref();

        //message( "Resource %s served by %s @ %s created", name, busname, objectpath );
    }

    ~Resource()
    {
        //message( "Resource %s served by %s @ %s destroyed", name, busname, objectpath );
    }

    private void updateStatus()
    {
        var info = new HashTable<string,Value?>( str_hash, str_equal );
        var p = Value( typeof(int) );
        p.set_int( policy );
        info.insert( "policy", p );
        var u = Value( typeof(int) );
        u.set_int( users.size );
        info.insert( "refcount", u );
        instance.resource_changed( name, isEnabled(), info ); // DBUS SIGNAL
    }

    public bool isEnabled()
    {
        return ( status == ResourceStatus.ENABLED );
    }

    public bool hasUser( string user )
    {
        return ( user in users );
    }

    public void setPolicy( FreeSmartphone.UsageResourcePolicy policy )
    {
        if ( policy == this.policy )
            return;
        else
            ( this.policy = policy );

        switch ( policy )
        {
            case FreeSmartphone.UsageResourcePolicy.DISABLED:
                disable();
                break;
            case FreeSmartphone.UsageResourcePolicy.ENABLED:
                enable();
                break;
            case FreeSmartphone.UsageResourcePolicy.AUTO:
                if ( users.size > 0 )
                    enable();
                else
                    disable();
                break;
            default:
                assert_not_reached();
        }

        updateStatus();
    }

    public void addUser( string user ) throws FreeSmartphone.UsageError
    {
        if ( user in users )
            throw new FreeSmartphone.UsageError.USER_EXISTS( "Resource %s already requested by user %s".printf( name, user ) );

        if ( policy == FreeSmartphone.UsageResourcePolicy.DISABLED )
            throw new FreeSmartphone.UsageError.POLICY_DISABLED( "Resource %s cannot be requested by %s per policy".printf( name, user ) );

        users.insert( 0, user );

        if ( policy == FreeSmartphone.UsageResourcePolicy.AUTO && users.size == 1 )
            enable();

        updateStatus();
    }

    public void delUser( string user ) throws FreeSmartphone.UsageError
    {
        if ( !(user in users) )
            throw new FreeSmartphone.UsageError.USER_UNKNOWN( "Resource %s never been requested by user %s".printf( name, user ) );

        users.remove( user );

        if ( policy == FreeSmartphone.UsageResourcePolicy.AUTO && users.size == 0 )
            disable();

        updateStatus();
    }

    public void syncUsers()
    {
        dynamic DBus.Object busobj = dbusconn.get_object( DBus.DBUS_SERVICE_DBUS, DBus.DBUS_PATH_DBUS, DBus.DBUS_INTERFACE_DBUS );
        string[] busnames = busobj.ListNames();

        var usersToRemove = new ArrayList<string>();

        foreach ( var userbusname in users )
        {
            var found = false;
            foreach ( var busname in busnames )
            {
                if ( userbusname == busname )
                    found = true;
                    break;
            }
            if ( !found )
                usersToRemove.add( userbusname );
        }
        foreach ( var userbusname in usersToRemove )
        {
            instance.logger.warning( "Resource %s user %s has vanished.".printf( name, userbusname ) );
            delUser( userbusname );
        }
    }

    public string[] allUsers()
    {
        string[] res = {};
        foreach ( var user in users )
            res += user;
        return res;
    }

    public bool isPresent()
    {
        dynamic DBus.Object peer = dbusconn.get_object( busname, objectpath, DBus.DBUS_INTERFACE_PEER );
        try
        {
            peer.Ping();
            return true;
        }
        catch ( DBus.Error e )
        {
            instance.logger.warning( "Resource %s incommunicado: %s".printf( name, e.message ) );
            return false;
        }
    }

    public void enable() throws FreeSmartphone.ResourceError, DBus.Error
    {
        try
        {
            proxy.enable();
            status = ResourceStatus.ENABLED;
            updateStatus();
        }
        catch ( DBus.Error e )
        {
            instance.logger.error( "Resource %s can't be enabled: %s. Trying to disable instead".printf( name, e.message ) );
            proxy.disable();
            throw e;
        }
    }

    public void disable() throws FreeSmartphone.ResourceError, DBus.Error
    {
        try
        {
            proxy.disable();
            status = ResourceStatus.DISABLED;
        }
        catch ( DBus.Error e )
        {
            instance.logger.error( "Resource %s can't be disabled: %s. Setting status to UNKNOWN".printf( name, e.message ) );
            status = ResourceStatus.UNKNOWN;
            throw e;
        }
    }

    public void suspend() throws FreeSmartphone.ResourceError, DBus.Error
    {
        if ( status == ResourceStatus.ENABLED )
        {
            try
            {
                proxy.suspend();
                status = ResourceStatus.SUSPENDED;
            }
            catch ( DBus.Error e )
            {
                instance.logger.error( "Resource %s can't be suspended: %s. Trying to disable instead".printf( name, e.message ) );
                proxy.disable();
                throw e;
            }
        }
        else
        {
            instance.logger.debug( "Resource %s not enabled: not suspending".printf( name ) );
        }
    }

    public void resume() throws FreeSmartphone.ResourceError, DBus.Error
    {
        if ( status == ResourceStatus.SUSPENDED )
        {
            try
            {
                proxy.resume();
                status = ResourceStatus.ENABLED;
            }
            catch ( DBus.Error e )
            {
                instance.logger.error( "Resource %s can't be resumed: %s. Trying to disable instead".printf( name, e.message ) );
                proxy.disable();
                throw e;
            }
        }
        else
        {
            instance.logger.debug( "Resource %s not suspended: not resuming".printf( name ) );
        }
    }
}

/**
 * Serialized state class
 *
 * All properties here will be saved on forced shutdown
 **/
public class PersistentData : Object
{
    public HashMap<string,Resource> resources { get; set; }

    construct
    {
        resources = new HashMap<string,Resource>( str_hash, str_equal, str_equal );
    }
}

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
    private bool do_not_suspend;

    private PersistentData data;
    private weak HashMap<string,Resource> resources;

    dynamic DBus.Object dbus;
    dynamic DBus.Object idlenotifier;

    public Controller( FsoFramework.Subsystem subsystem )
    {
        this.subsystem = subsystem;

        this.subsystem.registerServiceName( FsoFramework.Usage.ServiceDBusName );
        this.subsystem.registerServiceObject( FsoFramework.Usage.ServiceDBusName,
                                              FsoFramework.Usage.ServicePathPrefix, this );

        // should we really suspend?
        do_not_suspend = config.boolValue( CONFIG_SECTION, "do_not_suspend", false );

        // start listening for name owner changes
        dbusconn = ( (FsoFramework.DBusSubsystem)subsystem ).dbusConnection();
        dbus = dbusconn.get_object( DBus.DBUS_SERVICE_DBUS, DBus.DBUS_PATH_DBUS, DBus.DBUS_INTERFACE_DBUS );
        dbus.NameOwnerChanged += onNameOwnerChanged;

        // get handle to IdleNotifier
        idlenotifier = dbusconn.get_object( FSO_IDLENOTIFIER_BUS, FSO_IDLENOTIFIER_PATH, FSO_IDLENOTIFIER_IFACE );

        // delayed init
        Idle.add( onIdleForInit );
    }

    public override string repr()
    {
        return "<%s>".printf( FsoFramework.Usage.ServicePathPrefix );
    }

#if PERSISTENCE
    private void syncResourcesAndUsers()
    {
        // for ever resource, we check whether it's still present, and if so,
        // whether any of the consumers might have disappeared meanwhile
        var resourcesToRemove = new Gee.HashSet<Resource>();

        foreach ( var r in resources.get_values() )
        {
            if ( !r.isPresent() )
            {
                resourcesToRemove.add( r );
            }
        }
        foreach ( var r in resourcesToRemove )
        {
            resources.remove( r.name );
            this.resource_available( r.name, false ); // DBUS SIGNAL
        }

        foreach ( var r in resources.get_values() )
        {
            r.syncUsers();
        }
    }
#endif

    private bool onIdleForInit()
    {
        // check preferred low level suspend/resume plugin and instanciate
        var lowleveltype = config.stringValue( CONFIG_SECTION, "lowlevel_type", DEFAULT_LOWLEVEL_MODULE );
        string typename = "none";

        switch ( lowleveltype )
        {
            case "kernel26":
                typename = "LowLevelKernel26";
                break;
            case "openmoko":
                typename = "LowLevelOpenmoko";
                break;
            default:
                warning( "Invalid lowlevel_type '%s'; suspend/resume will NOT be available!".printf( lowleveltype ) );
                return false; // don't call me again
        }

        if ( lowleveltype != "none" )
        {
            var lowlevelclass = Type.from_name( typename );
            if ( lowlevelclass == Type.INVALID  )
            {
                logger.warning( "Can't find plugin for lowlevel_type = '%s'".printf( lowleveltype ) );
                return false; // don't call me again
            }

            lowlevel = Object.new( lowlevelclass ) as FsoUsage.LowLevel;
            logger.info( "Ready. Using lowlevel plugin '%s' to handle suspend/resume".printf( lowleveltype ) );
        }
#if PERSISTENCE
        // check whether we have crash data
        if ( loadPersistentData() )
        {
            resources = data.resources;
            syncResourcesAndUsers();
        }
        else
#endif
        {
            data = new PersistentData();
            resources = data.resources;
        }

        return false; // don't call me again
    }

    private void onResourceAppearing( Resource r )
    {
        logger.debug( "Resource %s served by %s @ %s has just been registered".printf( r.name, r.busname, r.objectpath ) );
        this.resource_available( r.name, true ); // DBUS SIGNAL

        // initial status is disabled
        try
        {
            r.disable();
        }
        catch ( FreeSmartphone.ResourceError e )
        {
            logger.warning( "Error while trying to (initially) disable resource %s: %s".printf( r.name, e.message ) );
        }
        catch ( DBus.Error e )
        {
            logger.warning( "Error while trying to (initially) disable resource %s: %s".printf( r.name, e.message ) );
        }
    }

    private void onResourceVanishing( Resource r )
    {
        logger.debug( "Resource %s served by %s @ %s has just been unregistered".printf( r.name, r.busname, r.objectpath ) );
        this.resource_available( r.name, false ); // DBUS SIGNAL

        try
        {
            r.disable();
        }
        catch ( FreeSmartphone.ResourceError e )
        {
            logger.warning( "Error while trying to (initially) disable resource %s: %s".printf( r.name, e.message ) );
        }
        catch ( DBus.Error e )
        {
            logger.warning( "Error while trying to (finally) disable resource %s: %s".printf( r.name, e.message ) );
        }
    }

    private void onNameOwnerChanged( dynamic DBus.Object obj, string name, string oldowner, string newowner )
    {
        //message( "name owner changed: %s (%s => %s)", name, oldowner, newowner );
        // we're only interested in services disappearing
        if ( newowner != "" )
            return;

        logger.debug( "%s disappeared. checking whether resources are affected...".printf( name ) );

        //FIXME: Consider keeping the known busnames in a map as well, so we don't have to iterate through all values

        var resourcesToRemove = new Gee.HashSet<Resource>();

        foreach ( var r in resources.get_values() )
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

    private bool onIdleForSuspend()
    {
        suspendAllResources();
        logger.debug( ">>>>>>> KERNEL SUSPEND" );
        if ( !do_not_suspend )
            lowlevel.suspend();
        else
            Posix.sleep( 5 );
        logger.debug( "<<<<<<< KERNEL RESUME" );
        FsoUsage.ResumeReason reason = lowlevel.resume();
        logger.info( "Resume reason seems to be %s".printf( FsoFramework.StringHandling.enumToString( typeof( FsoUsage.ResumeReason ), reason) ) );
        resumeAllResources();
        this.system_action( FreeSmartphone.UsageSystemAction.RESUME ); // DBUS SIGNAL

        var idlestate = lowlevel.isUserInitiated( reason ) ? "busy" : "idle";
        try
        {
            idlenotifier.SetState( idlestate );
        }
        catch ( DBus.Error e )
        {
            logger.error( "DBus Error while talking to IdleNotifier: %s".printf( e.message ) );
        }
        return false; // MainLoop: Don't call again
    }

    private Resource getResource( string name ) throws FreeSmartphone.UsageError
    {
        Resource r = resources[name];
        if ( r == null )
            throw new FreeSmartphone.UsageError.RESOURCE_UNKNOWN( "Resource %s had never been registered".printf( name ) );

        logger.debug( "current users for %s = %s".printf( r.name, FsoFramework.StringHandling.stringListToString( r.allUsers() ) ) );

        return r;
    }

    private void disableAllResources()
    {
        foreach ( var r in resources.get_values() )
        {
            try
            {
                r.disable();
            }
            catch ( FreeSmartphone.ResourceError e )
            {
                logger.warning( "Error while trying to suspend resource %s: %s".printf( r.name, e.message ) );
            }
            catch ( DBus.Error e )
            {
                logger.warning( "Error while trying to disable resource %s: %s".printf( r.name, e.message ) );
            }
        }
    }

    private void suspendAllResources()
    {
        foreach ( var r in resources.get_values() )
        {
            try
            {
                r.suspend();
            }
            catch ( FreeSmartphone.ResourceError e )
            {
                logger.warning( "Error while trying to suspend resource %s: %s".printf( r.name, e.message ) );
            }
            catch ( DBus.Error e )
            {
                logger.warning( "Error while trying to suspend resource %s: %s".printf( r.name, e.message ) );
            }
        }
    }

    private void resumeAllResources()
    {
        foreach ( var r in resources.get_values() )
        {
            try
            {
                r.resume();
            }
            catch ( FreeSmartphone.ResourceError e )
            {
                logger.warning( "Error while trying to suspend resource %s: %s".printf( r.name, e.message ) );
            }
            catch ( DBus.Error e )
            {
                logger.warning( "Error while trying to resume resource %s: %s".printf( r.name, e.message ) );
            }
        }
    }

#if PERSISTENCE
    // not public, since we don't want to expose it via dbus
    internal void savePersistentData()
    {
        logger.info( "Saving resource status to file..." );
        var file = File.new_for_path( "/tmp/serialize.output" );
        var stream = file.replace( null, false, FileCreateFlags.NONE, null );
        Persistence.JsonTypeSerializer.instance().ignoreUnknown = true;
        var serializer = new Persistence.JsonSerializer( stream );
        serializer.serialize_object( data );
    }

    internal bool loadPersistentData()
    {
        var file = File.new_for_path( "/tmp/serialize.output" );
        if ( !file.query_exists( null ) )
        {
            return false;
        }
        var stream = file.read( null );
        Persistence.JsonTypeSerializer.instance().ignoreUnknown = true;
        var deserializer = new Persistence.JsonDeserializer<PersistentData>( stream );
        data = deserializer.deserialize_object() as PersistentData;
        return true;
    }
#endif

    //
    // DBUS API (for providers)
    //
    public void register_resource( DBus.BusName sender, string name, DBus.ObjectPath path ) throws FreeSmartphone.UsageError, DBus.Error
    {
        message( "register_resource called with parameters: %s %s %s", sender, name, path );
        if ( name in resources.get_keys() )
            throw new FreeSmartphone.UsageError.RESOURCE_EXISTS( "Resource %s already registered".printf( name ) );

        var r = new Resource( name, sender, path );
        resources[name] = r;

        onResourceAppearing( r );
    }

    public void unregister_resource( DBus.BusName sender, string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        var r = getResource( name );

        if ( r.busname != sender )
            throw new FreeSmartphone.UsageError.RESOURCE_UNKNOWN( "Resource %s not yours".printf( name ) );

        onResourceVanishing( r );

        resources.remove( name );
    }

    //
    // DBUS API (for consumers)
    //
    //public FreeSmartphone.UsageResourcePolicy get_resource_policy( string name ) throws FreeSmartphone.UsageError, FreeSmartphone.Error, DBus.Error
    public string get_resource_policy( string name ) throws FreeSmartphone.UsageError, FreeSmartphone.Error, DBus.Error
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
    public void set_resource_policy( string name, string policy ) throws FreeSmartphone.UsageError, FreeSmartphone.Error, DBus.Error
    {
        message( "set resource policy for %s to %s", name, policy );

        if ( policy == "enabled" )
            getResource( name ).setPolicy( FreeSmartphone.UsageResourcePolicy.ENABLED );
        else if ( policy == "disabled" )
            getResource( name ).setPolicy( FreeSmartphone.UsageResourcePolicy.DISABLED );
        else if ( policy == "auto" )
            getResource( name ).setPolicy( FreeSmartphone.UsageResourcePolicy.AUTO );
        else
            throw new FreeSmartphone.Error.INVALID_PARAMETER( "ResourcePolicy needs to be one of { \"enabled\", \"disabled\", \"auto\" }" );
    }

    public bool get_resource_state( string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        return getResource( name ).isEnabled();
    }

    public string[] get_resource_users( string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        return getResource( name ).allUsers();
    }

    public string[] list_resources() throws DBus.Error
    {
        string[] res = {};
        foreach ( var key in resources.get_keys() )
            res += key;
        return res;
    }

    public void request_resource( DBus.BusName sender, string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        getResource( name ).addUser( sender );
    }

    public void release_resource( DBus.BusName sender, string name ) throws FreeSmartphone.UsageError, DBus.Error
    {
        getResource( name ).delUser( sender );
    }

    public void shutdown() throws DBus.Error
    {
        this.system_action( FreeSmartphone.UsageSystemAction.SHUTDOWN ); // DBUS SIGNAL
        disableAllResources();
        Posix.system( "shutdown -h now" );
    }

    public void reboot() throws DBus.Error
    {
        this.system_action( FreeSmartphone.UsageSystemAction.REBOOT ); // DBUS SIGNAL
        disableAllResources();
        Posix.system( "reboot" );
    }

    public void suspend() throws DBus.Error
    {
        this.system_action( FreeSmartphone.UsageSystemAction.SUSPEND ); // DBUS SIGNAL
        // we need to suspend async, otherwise the dbus call would timeout
        Idle.add( onIdleForSuspend );
    }

    // DBUS SIGNALS
    public signal void resource_available( string name, bool availability );
    public signal void resource_changed( string name, bool state, GLib.HashTable<string,GLib.Value?> attributes );
    public signal void system_action( FreeSmartphone.UsageSystemAction action );
}

} /* end namespace */

Usage.Controller instance;
DBus.Connection dbusconn;

public static string fso_factory_function( FsoFramework.Subsystem subsystem ) throws Error
{
    instance = new Usage.Controller( subsystem );
    return "fsousage.controller";
}

public static void fso_shutdown_function()
{
#if PERSISTENCE
    instance.savePersistentData();
#endif
}

[ModuleInit]
public static void fso_register_function( TypeModule module )
{
    debug( "usage controller fso_register_function()" );
}
