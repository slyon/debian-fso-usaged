/**
 * Copyright (C) 2009 Michael 'Mickey' Lauer <mlauer@vanille-media.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

using GLib;

using FsoUsage;

internal const string SYSFS_RESUME_REASON_PATH    = "/class/i2c-adapter/i2c-0/0-0073/neo1973-resume.0/resume_reason";
internal const string SYSFS_RESUME_SUBREASON_PATH = "/class/i2c-adapter/i2c-0/0-0073/resume_reason";

class LowLevel.Openmoko : FsoUsage.LowLevel, FsoFramework.AbstractObject
{
    private HashTable<string,uint> intmap1;
    private HashTable<string,uint> intmap2;

    construct
    {
        logger.info( "Registering openmoko low level suspend/resume handling" );
        // grab sysfs paths
        var sysfs_root = config.stringValue( "cornucopia", "sysfs_root", "/sys" );
        sys_power_state = Path.build_filename( sysfs_root, "power", "state" );
        sys_resume_reason = Path.build_filename( sysfs_root, SYSFS_RESUME_REASON_PATH );
        sys_resume_subreason = Path.build_filename( sysfs_root, SYSFS_RESUME_SUBREASON_PATH );

        intmap1 = new HashTable<string,uint>( str_hash, str_equal );
        intmap1.insert( "EINT00_ACCEL1", ResumeReason.Accelerometer );
        intmap1.insert( "EINT01_GSM", ResumeReason.GSM );
        intmap1.insert( "EINT02_BLUETOOTH", ResumeReason.Bluetooth );
        intmap1.insert( "EINT03_DEBUGBRD", ResumeReason.Debug );
        intmap1.insert( "EINT04_JACK", ResumeReason.Headphone );
        intmap1.insert( "EINT05_WLAN", ResumeReason.WiFi );
        intmap1.insert( "EINT06_AUXKEY", ResumeReason.AuxKey );
        intmap1.insert( "EINT07_HOLDKEY", ResumeReason.Headphone );
        intmap1.insert( "EINT08_ACCEL2", ResumeReason.Accelerometer );
        intmap1.insert( "EINT09_PMU", ResumeReason.PMU );
        intmap1.insert( "EINT10_NULL", ResumeReason.Invalid );
        intmap1.insert( "EINT11_NULL", ResumeReason.Invalid );
        intmap1.insert( "EINT12_GLAMO", ResumeReason.GFX );
        intmap1.insert( "EINT13_NULL", ResumeReason.Invalid );
        intmap1.insert( "EINT14_NULL", ResumeReason.Invalid );
        intmap1.insert( "EINT15_NULL", ResumeReason.Invalid );

        intmap2 = new HashTable<string,uint>( str_hash, str_equal );
        intmap2.insert( "0000000200", ResumeReason.LowBattery );
        intmap2.insert( "0002000000", ResumeReason.PowerKey );
    }

    public override string repr()
    {
        return "<>";
    }

    public void suspend()
    {
        FsoFramework.FileHandling.write( "mem\n", sys_power_state );
    }

    public ResumeReason resume()
    {
        var reasons = FsoFramework.FileHandling.read( sys_resume_reason ).split( "\n" );
        var reasonkey = "unknown";
        foreach ( var line in reasons )
        {
            if ( line.has_prefix( "*" ) )
            {
                reasonkey = line.substring( 2 );
                break;
            }
        }
        var reasonvalue = intmap1.lookup( reasonkey );
        if ( reasonvalue == 0 )
        {
           logger.info( "No resume reason marked in %s".printf( sys_resume_reason ) );
           return ResumeReason.Unknown;
        }

        if ( reasonvalue == ResumeReason.PMU )
        {
           logger.debug( "PMU resume reason marked in %s".printf( sys_resume_reason ) );

           var subreasonkey = FsoFramework.FileHandling.read( sys_resume_subreason );
           var subreasonvalue = intmap2.lookup( subreasonkey );
           if ( subreasonvalue == 0 )
           {
               logger.debug( "Unknown subreason %s for PMU resume, please fix me!".printf( subreasonkey ) );
               return ResumeReason.PMU;
           }
           return (ResumeReason)subreasonvalue;
        }
        return (ResumeReason)reasonvalue;
    }
}

string sys_power_state;
string sys_resume_reason;
string sys_resume_subreason;

/**
 * This function gets called on plugin initialization time.
 * @return the name of your plugin here
 * @note that it needs to be a name in the format <subsystem>.<plugin>
 * else your module will be unloaded immediately.
 **/
public static string fso_factory_function( FsoFramework.Subsystem subsystem ) throws Error
{
    debug( "lowlevel_openmoko fso_factory_function" );
    return "fsousaged.lowlevel_openmoko";
}

[ModuleInit]
public static void fso_register_function( TypeModule module )
{
    // do not remove this function
}
