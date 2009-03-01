/* fsoframework-2.0.vapi generated by valac, do not modify. */

[CCode (cprefix = "FsoFramework", lower_case_cprefix = "fso_framework_")]
namespace FsoFramework {
	[CCode (cheader_filename = "fsoframework/logger.h")]
	public abstract class AbstractLogger : GLib.Object {
		protected string destination;
		protected string domain;
		protected uint level;
		public void debug (string message);
		public void error (string message);
		public void info (string message);
		public AbstractLogger (string domain);
		public void setDestination (string destination);
		public void setLevel (GLib.LogLevelFlags level);
		public void warning (string message);
		protected virtual void write (string message);
	}
	[CCode (cheader_filename = "fsoframework/logger.h")]
	public class FileLogger : FsoFramework.AbstractLogger {
		public FileLogger (string domain);
		public void setFile (string filename, bool append = false);
	}
	[CCode (cheader_filename = "fsoframework/smartkeyfile.h")]
	public class SmartKeyFile : GLib.Object {
		public bool boolValue (string section, string key, bool defaultvalue);
		public int intValue (string section, string key, int defaultvalue);
		public bool loadFromFile (string filename);
		public SmartKeyFile ();
		public string stringValue (string section, string key, string defaultvalue);
	}
	[CCode (cheader_filename = "fsoframework/common.h")]
	public static FsoFramework.SmartKeyFile MasterKeyFile ();
}
