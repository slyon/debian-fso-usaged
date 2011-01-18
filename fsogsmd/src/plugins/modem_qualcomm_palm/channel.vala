/*
 * Copyright (C) 2009-2011 Michael 'Mickey' Lauer <mlauer@vanille-media.de>
 *                         Simon Busch <morphis@gravedo.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

using GLib;
using FsoGsm;
using FsoFramework;

internal class WaitForUnsolicitedResponseData
{
    public GLib.SourceFunc callback;
    public MsmUrcType urc_type { get; set; }
    public GLib.Variant? response { get; set; }
    public uint timeout { get; set; }
}

public class MsmChannel : CommandQueue, Channel, AbstractObject
{
    public Transport transport { get; set; }
    public string name;

    private FreeSmartphone.Usage usage;
    private Gee.ArrayList<WaitForUnsolicitedResponseData> urcWaiters;
    private MsmUnsolicitedResponseHandler urcHandler;
    private bool isInitialized;
    private Msmcomm.ModemStatus currentModemStatus;

    public Msmcomm.Management management_service;
    public Msmcomm.State state_service;
    public Msmcomm.Misc misc_service;
    public Msmcomm.Call call_service;
    public Msmcomm.Sim sim_service;
    public Msmcomm.Phonebook phonebook_service;

    private void onModemStatusChanged( FsoGsm.Modem modem, FsoGsm.Modem.Status status )
    {
        switch ( status )
        {
            case FsoGsm.Modem.Status.INITIALIZING:
                initialize();
                break;
            case FsoGsm.Modem.Status.CLOSING:
                shutdown();
                break;
            default:
                break;
        }
    }

    private void onModemControlStatusChanged( Msmcomm.ModemStatus status )
    {
        currentModemStatus = status;

        /* ignore status updates as long as we are not initialized */
        if ( !isInitialized )
            return;

        switch ( status )
        {
            case Msmcomm.ModemStatus.ACTIVE:
                /* modem controller become active after it was successfully initialized?
                 * that can only one cause: modem was reseted due to internal error. We
                 * have to take care about this and re-synchronize with the modem.
                 */
                logger.error( "Modem control was reseted due to internal error; synchronizing with the modem ..." );

                Posix.sleep(2);
                misc_service.test_alive();

                break;
            case Msmcomm.ModemStatus.INACTIVE:
                break;
        }
    }

    public MsmChannel( string name )
    {
        urcWaiters = new Gee.ArrayList<WaitForUnsolicitedResponseData>();
        urcHandler = new MsmUnsolicitedResponseHandler();

        isInitialized = false;
        currentModemStatus = Msmcomm.ModemStatus.UNKNOWN;

        this.name = name;
        theModem.registerChannel( name, this );
        theModem.signalStatusChanged.connect( onModemStatusChanged );
    }

    public async void initialize()
    {
        /* initialize the modem controller itself */
        logger.debug( "Initialize modem controller ..." );
        management_service.initialize();

        /* wait as long as modem controller is not ready */
        logger.debug( "Waiting for modem controller to be fully initialized ..." );
        Timeout.add_seconds(2, () => {
            if ( currentModemStatus == Msmcomm.ModemStatus.ACTIVE )
            {
                initialize.callback();
                return false;
            }
            return true;
        });
        yield;

        urcHandler.setup();

        /* reset the modem and wait for it to come back */
        logger.debug( "Reseting modem and waiting for it to come back ..." );
        yield state_service.change_operation_mode( Msmcomm.OperationMode.RESET );
        yield waitForUnsolicitedResponse( MsmUrcType.RESET_RADIO_IND );

        Posix.sleep(2);

        logger.debug( "Modem is back after reset now; Synchronizing ..." );
        yield misc_service.test_alive();

        isInitialized = true;
    }

    public async void shutdown()
    {
        logger.debug( "Shutdown modem controller ..." );
        yield management_service.shutdown();
        isInitialized = false;
    }

    public void injectResponse( string response )
    {
        assert_not_reached();
    }

    public async bool suspend()
    {
        return true;
    }

    public async bool resume()
    {
        return true;
    }

    private async bool requestModemResource()
    {
        try
        {
            yield usage.request_resource( "Modem" );
            registerObjects();
        }
        catch ( FreeSmartphone.UsageError err  )
        {
            logger.error( @"Modem resource is not available: $(err.message)" );
            return false;
        }

        return true;
    }

    private void registerObjects()
    {
        management_service =  Bus.get_proxy_sync<Msmcomm.Management>( BusType.SYSTEM, "org.msmcomm", "/org/msmcomm" );
        misc_service =  Bus.get_proxy_sync<Msmcomm.Misc>( BusType.SYSTEM, "org.msmcomm", "/org/msmcomm" );
        state_service =  Bus.get_proxy_sync<Msmcomm.State>( BusType.SYSTEM, "org.msmcomm", "/org/msmcomm" );
    }

    public async bool open()
    {
        usage = Bus.get_proxy_sync<FreeSmartphone.Usage>( BusType.SYSTEM, "org.freesmartphone.ousaged", "/org/freesmartphone/Usage" );

        return yield requestModemResource();
    }

    private async void releaseModemResource()
    {
        try
        {
            yield usage.release_resource( "Modem" );
        }
        catch ( FreeSmartphone.UsageError err0 )
        {
        }
        catch ( GLib.Error err1 )
        {
        }
    }

    public async void close()
    {
        yield releaseModemResource();
    }

    public void registerUnsolicitedHandler( FsoFramework.CommandQueue.UnsolicitedHandler urchandler )
    {

    }

    public async void freeze( bool drain = false )
    {
    }

    public async void thaw()
    {
    }

    public override string repr()
    {
        return "<>";
    }

    /**
     * Lets wait for a specific unsolicited response to recieve and return it's payload
     * after it finaly recieves.
     **/
    public async GLib.Variant waitForUnsolicitedResponse( MsmUrcType type )
    {
        // Create waiter and yield until urc occurs
        var data = new WaitForUnsolicitedResponseData();
        data.urc_type = type;
        data.callback = waitForUnsolicitedResponse.callback;
        urcWaiters.add( data );
        yield;

        // Urc occured so we can return the recieved message structure to the caller who
        // has now not longer to wait for the urc
        urcWaiters.remove( data );
        return data.response;
    }

    /**
     * Notify the occurence of a unsolicted response to the modem agent which informs all
     * registered clients for this type of message.
     **/
    public async void notifyUnsolicitedResponse( MsmUrcType type, GLib.Variant? response )
    {
        var waiters = retriveUrcWaiters( type );

        // awake all waiters for the notified urc type and supply them the message payload
        foreach (var waiter in waiters )
        {
            waiter.response = response;
            waiter.callback();
        }
    }

    private Gee.ArrayList<WaitForUnsolicitedResponseData> retriveUrcWaiters( MsmUrcType type )
    {
        var result = new Gee.ArrayList<WaitForUnsolicitedResponseData>();

        foreach ( var waiter in urcWaiters )
        {
            if ( waiter.urc_type == type )
            {
                result.add( waiter );
            }
        }

        return result;
    }
}

