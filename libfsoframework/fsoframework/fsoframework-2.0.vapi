/* fsoframework-2.0.vapi generated by valac, do not modify. */

[CCode (cprefix = "FsoFramework", lower_case_cprefix = "fso_framework_")]
namespace FsoFramework {
	[CCode (cprefix = "FsoFrameworkDevice", lower_case_cprefix = "fso_framework_device_")]
	namespace Device {
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		[DBus (name = "org.freesmartphone.Device.Display")]
		public interface Display : GLib.Object {
			public abstract bool GetBacklightPower ();
			public abstract int GetBrightness ();
			public abstract GLib.HashTable<string,GLib.Value?> GetInfo ();
			public abstract void SetBacklightPower (bool power);
			public abstract void SetBrightness (int brightness);
		}
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		[DBus (name = "org.freesmartphone.Device.Info")]
		public interface Info : GLib.Object {
			public abstract GLib.HashTable<string,GLib.Value?> GetCpuInfo ();
		}
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		[DBus (name = "org.freesmartphone.Device.LED")]
		public interface LED : GLib.Object {
			public abstract string GetName ();
			public abstract void SetBlinking (int delay_on, int delay_off) throws DBus.Error;
			public abstract void SetBrightness (int brightness);
			public abstract void SetNetworking (string iface, string mode) throws DBus.Error;
		}
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string DisplayServiceFace;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string DisplayServicePath;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string InfoServiceFace;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string InfoServicePath;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string LedServiceFace;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string LedServicePath;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string ServiceDBusName;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string ServiceFacePrefix;
		[CCode (cheader_filename = "fsoframework/interfaces.h")]
		public const string ServicePathPrefix;
	}
	[CCode (cprefix = "FsoFrameworkFileHandling", lower_case_cprefix = "fso_framework_file_handling_")]
	namespace FileHandling {
		[CCode (cheader_filename = "fsoframework/utilities.h")]
		public static bool isPresent (string filename);
		[CCode (cheader_filename = "fsoframework/utilities.h")]
		public static string read (string filename);
		[CCode (cheader_filename = "fsoframework/utilities.h")]
		public static void write (string contents, string filename);
	}
	[CCode (cprefix = "FsoFrameworkStringHandling", lower_case_cprefix = "fso_framework_string_handling_")]
	namespace StringHandling {
		[CCode (cheader_filename = "fsoframework/utilities.h")]
		public static string stringListToString (string[] list);
	}
	[CCode (cprefix = "FsoFrameworkUserGroupHandling", lower_case_cprefix = "fso_framework_user_group_handling_")]
	namespace UserGroupHandling {
		[CCode (cheader_filename = "fsoframework/utilities.h")]
		public static Posix.gid_t gidForGroup (string group);
		[CCode (cheader_filename = "fsoframework/utilities.h")]
		public static bool switchToUserAndGroup (string user, string group);
		[CCode (cheader_filename = "fsoframework/utilities.h")]
		public static Posix.uid_t uidForUser (string user);
	}
	[CCode (cheader_filename = "fsoframework/logger.h")]
	public abstract class AbstractLogger : FsoFramework.Logger, GLib.Object {
		protected string destination;
		protected string domain;
		protected uint level;
		protected virtual string format (string message, string level);
		public static string levelToString (GLib.LogLevelFlags level);
		public AbstractLogger (string domain);
		public static GLib.LogLevelFlags stringToLevel (string level);
		protected virtual void write (string message);
	}
	[CCode (cheader_filename = "fsoframework/object.h")]
	public abstract class AbstractObject : GLib.Object {
		protected FsoFramework.SmartKeyFile config;
		protected FsoFramework.Logger logger;
		public abstract string repr ();
		public string classname { get; construct; }
	}
	[CCode (cheader_filename = "fsoframework/subsystem.h")]
	public abstract class AbstractSubsystem : FsoFramework.Subsystem, GLib.Object {
		protected FsoFramework.Logger logger;
		public AbstractSubsystem (string name);
		public virtual bool registerServiceName (string servicename);
		public virtual bool registerServiceObject (string servicename, string objectname, GLib.Object obj);
	}
	[CCode (cheader_filename = "fsoframework/plugin.h")]
	public class BasePlugin : FsoFramework.Plugin, GLib.TypeModule {
		public override bool load ();
		public BasePlugin (string filename, FsoFramework.Subsystem subsystem);
		public override void unload ();
	}
	[CCode (cheader_filename = "fsoframework/subsystem.h")]
	public class BaseSubsystem : FsoFramework.AbstractSubsystem {
		public BaseSubsystem (string name);
	}
	[CCode (cheader_filename = "fsoframework/transport.h")]
	public class BaseTransport : FsoFramework.Transport {
		protected GLib.ByteArray buffer;
		protected int fd;
		protected bool hard;
		protected FsoFramework.TransportHupFunc hupfunc;
		protected FsoFramework.Logger logger;
		protected string name;
		protected bool raw;
		protected FsoFramework.TransportReadFunc readfunc;
		protected uint speed;
		public bool actionCallback (GLib.IOChannel source, GLib.IOCondition condition);
		public override void close ();
		public override void freeze ();
		public override string getName ();
		public override bool isOpen ();
		public BaseTransport (string name, uint speed = 0, bool raw = true, bool hard = true);
		public override bool open ();
		public override int read (void* data, int len);
		public virtual string repr ();
		protected void restartWriter ();
		public override void setDelegates (FsoFramework.TransportReadFunc? readfunc, FsoFramework.TransportHupFunc? hupfunc);
		public override void thaw ();
		public override int write (void* data, int len);
		public bool writeCallback (GLib.IOChannel source, GLib.IOCondition condition);
	}
	[CCode (cheader_filename = "fsoframework/subsystem.h")]
	public class DBusSubsystem : FsoFramework.AbstractSubsystem {
		public DBusSubsystem (string name);
		public override bool registerServiceName (string servicename);
		public override bool registerServiceObject (string servicename, string objectname, GLib.Object obj);
	}
	[CCode (cheader_filename = "fsoframework/logger.h")]
	public class FileLogger : FsoFramework.AbstractLogger {
		public FileLogger (string domain);
		public void setFile (string filename, bool append = false);
		protected override void write (string message);
	}
	[CCode (cheader_filename = "fsoframework/logger.h")]
	public class NullLogger : FsoFramework.AbstractLogger {
		public NullLogger (string domain);
		protected override void write (string message);
	}
	[CCode (cheader_filename = "fsoframework/transport.h")]
	public class PtyTransport : FsoFramework.BaseTransport {
		public override string getName ();
		public PtyTransport ();
		public override bool open ();
		public override string repr ();
	}
	[CCode (cheader_filename = "fsoframework/transport.h")]
	public class SerialTransport : FsoFramework.BaseTransport {
		public SerialTransport (string portname, uint portspeed, bool raw = true, bool hard = true);
		public override bool open ();
		public override string repr ();
	}
	[CCode (cheader_filename = "fsoframework/smartkeyfile.h")]
	public class SmartKeyFile : GLib.Object {
		public bool boolValue (string section, string key, bool defaultvalue = false);
		public bool hasSection (string section);
		public int intValue (string section, string key, int defaultvalue = 0);
		public bool loadFromFile (string filename);
		public SmartKeyFile ();
		public GLib.List<string> sectionsWithPrefix (string? prefix = null);
		public string[]? stringListValue (string section, string key, string[]? defaultvalue = null);
		public string stringValue (string section, string key, string defaultvalue = "");
	}
	[CCode (cheader_filename = "fsoframework/smartkeyfile.h")]
	public class SmartKeyFileSection : GLib.Object {
		public static FsoFramework.SmartKeyFileSection? openSection (FsoFramework.SmartKeyFile kf, string section);
	}
	[CCode (cheader_filename = "fsoframework/logger.h")]
	public class SyslogLogger : FsoFramework.AbstractLogger {
		protected override string format (string message, string level);
		public SyslogLogger (string domain);
		protected override void write (string message);
	}
	[CCode (cheader_filename = "fsoframework/transport.h")]
	public abstract class Transport : GLib.Object {
		public abstract void close ();
		public static FsoFramework.Transport create (string type, string name = "", uint speed = 0, bool raw = true, bool hard = true);
		public abstract void freeze ();
		public abstract string getName ();
		public abstract bool isOpen ();
		public abstract bool open ();
		public abstract int read (void* data, int len);
		public abstract void setDelegates (FsoFramework.TransportReadFunc? readfunc, FsoFramework.TransportHupFunc? hupfunc);
		public abstract void thaw ();
		public abstract int write (void* data, int length);
	}
	[CCode (cheader_filename = "fsoframework/logger.h")]
	public interface Logger : GLib.Object {
		public abstract void debug (string message);
		public abstract void error (string message);
		public abstract void info (string message);
		public abstract void setDestination (string destination);
		public abstract void setLevel (GLib.LogLevelFlags level);
		public abstract void setReprDelegate (ReprDelegate repr);
		public abstract void warning (string message);
	}
	[CCode (cheader_filename = "fsoframework/plugin.h")]
	public interface Plugin : GLib.Object {
		public abstract FsoFramework.PluginInfo info ();
		public abstract void loadAndInit () throws FsoFramework.PluginError;
	}
	[CCode (cheader_filename = "fsoframework/subsystem.h")]
	public interface Subsystem : GLib.Object {
		public abstract uint loadPlugins ();
		public abstract string name ();
		public abstract GLib.List<FsoFramework.PluginInfo?> pluginsInfo ();
		public abstract uint registerPlugins ();
		public abstract bool registerServiceName (string servicename);
		public abstract bool registerServiceObject (string servicename, string objectname, GLib.Object obj);
	}
	[CCode (type_id = "FSO_FRAMEWORK_TYPE_PLUGIN_INFO", cheader_filename = "fsoframework/plugin.h")]
	public struct PluginInfo {
		public string name;
		public bool loaded;
	}
	[CCode (cprefix = "FSO_FRAMEWORK_TRANSPORT_STATE_", cheader_filename = "fsoframework/transport.h")]
	public enum TransportState {
		CLOSED,
		OPEN,
		ALIVE,
		FROZEN,
		DEAD
	}
	[CCode (cprefix = "FSO_FRAMEWORK_PLUGIN_ERROR_", cheader_filename = "fsoframework/plugin.h")]
	public errordomain PluginError {
		UNABLE_TO_LOAD,
		REGISTER_NOT_FOUND,
		FACTORY_NOT_FOUND,
		UNABLE_TO_INITIALIZE,
	}
	[CCode (cprefix = "FSO_FRAMEWORK_TRANSPORT_ERROR_", cheader_filename = "fsoframework/transport.h")]
	public errordomain TransportError {
		UNABLE_TO_OPEN,
		UNABLE_TO_WRITE,
	}
	[CCode (cheader_filename = "fsoframework/plugin.h")]
	public static delegate string FactoryFunc (FsoFramework.Subsystem subsystem);
	[CCode (cheader_filename = "fsoframework/plugin.h")]
	public static delegate void RegisterFunc (GLib.TypeModule bar);
	[CCode (cheader_filename = "fsoframework/transport.h")]
	public delegate void TransportHupFunc (FsoFramework.Transport transport);
	[CCode (cheader_filename = "fsoframework/transport.h")]
	public delegate void TransportReadFunc (FsoFramework.Transport transport);
	[CCode (cheader_filename = "fsoframework/common.h")]
	public const string DEFAULT_LOG_DESTINATION;
	[CCode (cheader_filename = "fsoframework/common.h")]
	public const string DEFAULT_LOG_LEVEL;
	[CCode (cheader_filename = "fsoframework/common.h")]
	public const string DEFAULT_LOG_TYPE;
	[CCode (cheader_filename = "fsoframework/interfaces.h")]
	public const string ServiceDBusPrefix;
	[CCode (cheader_filename = "fsoframework/interfaces.h")]
	public const string ServiceFacePrefix;
	[CCode (cheader_filename = "fsoframework/interfaces.h")]
	public const string ServicePathPrefix;
	[CCode (cheader_filename = "fsoframework/common.h")]
	public static FsoFramework.Logger createLogger (string domain);
	[CCode (cheader_filename = "fsoframework/common.h")]
	public static string getPrefixForExecutable ();
	[CCode (cheader_filename = "fsoframework/common.h")]
	public static FsoFramework.SmartKeyFile theMasterKeyFile ();
}
[CCode (cheader_filename = "fsoframework/subsystem.h")]
[DBus (name = "org.freesmartphone.DBus.Objects")]
public interface DBusObjects {
	public abstract void getNodes ();
}
[CCode (cheader_filename = "fsoframework/logger.h")]
public delegate string ReprDelegate ();
