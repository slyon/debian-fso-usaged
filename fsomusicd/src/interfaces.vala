/* 
 * File Name: 
 * Creation Date: 23-08-2009
 * Last Modified:
 *
 * Authored by Frederik 'playya' Sdun <Frederik.Sdun@googlemail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */
using GLib;
using DBus;

namespace FreeSmartphone.MusicPlayer
{
    public enum State
    {
        PLAYING,
        PAUSED,
        STOPPED
    }
    public const string BASE_OBJECT_PATH = "/org/freesmartphone/MusicPlayer";
    public const string BUSNAME = "org.freesmartphone.omusicd";

    [DBus (name="org.freesmartphone.MusicPlayer")]
    public interface IMusicPlayer
    {
        public abstract HashTable<string,Value?> get_info_for_file( string file );
        public abstract string get_playing();
        public abstract void set_playing( string file );
        public abstract void play();
        public abstract void pause();
        public abstract void stop();
        public abstract void previous();
        public abstract void next();
        public abstract void seek_forward( int step );
        public abstract void seek_backward( int step );
        public abstract void jump( int pos );
        public abstract ObjectPath[] get_playlists();
        public abstract ObjectPath get_current_playlist();
        public abstract void delete_playlist( string list );
        public abstract ObjectPath new_playlist( string name );
        public abstract string[] search( string query );
        public abstract signal void progress( int progress );
        public abstract signal void playing_changed( string file );
        public abstract signal void state( State state );
        public abstract signal void playlist( ObjectPath path );
    }
    [DBus (name="org.freesmartphone.MusicPlayer.Playlist")]
    public interface IPlaylist
    {
        public abstract string[] get_files() throws PlaylistError, DBus.Error;
        public abstract void insert( int position, string file ) throws PlaylistError, DBus.Error;
        public abstract void remove( int position ) throws PlaylistError, DBus.Error;
        public abstract void insert_dir( int position, string dir, bool recursive ) throws PlaylistError, DBus.Error;
        public abstract string get_at_position( int position ) throws PlaylistError, DBus.Error;
        public abstract void load_from_file( string filename ) throws PlaylistError, DBus.Error;
        public abstract string get_name() throws PlaylistError, DBus.Error;
        public abstract void change_name( string new_name ) throws PlaylistError, DBus.Error;
        public abstract signal void name( string name );
        public abstract signal void playing( string file );
        public abstract signal void deleted();
        public abstract signal void file_removed( int position );
        public abstract signal void file_added( int position, string filename);
    }
    public errordomain PlaylistError
    {
        FILE_NOT_FOUND,
        OUT_OF_RANGE,
        FILETYPE_NOT_SUPPORTED
    }

    [DBus (name = "org.freedesktop.DBus")]
    public interface DBusService : GLib.Object 
    {
        public abstract string hello() throws DBus.Error;
        public abstract uint request_name(string param0, uint param1) throws DBus.Error;
        public abstract uint release_name(string param0) throws DBus.Error;
        public abstract uint start_service_by_name(string param0, uint param1) throws DBus.Error;
        public abstract void update_activation_environment(GLib.HashTable<string, string> param0) throws DBus.Error;
        public abstract bool name_has_owner(string param0) throws DBus.Error;
        public abstract string[] list_names() throws DBus.Error;
        public abstract string[] list_activatable_names() throws DBus.Error;
        public abstract void add_match(string param0) throws DBus.Error;
        public abstract void remove_match(string param0) throws DBus.Error;
        public abstract string get_name_owner(string param0) throws DBus.Error;
        public abstract string[] list_queued_owners(string param0) throws DBus.Error;
        public abstract uint get_connection_unix_user(string param0) throws DBus.Error;
        public abstract uint get_connection_unix_process_i_d(string param0) throws DBus.Error;
        public abstract uchar[] get_adt_audit_session_data(string param0) throws DBus.Error;
        public abstract uchar[] get_connection_s_e_linux_security_context(string param0) throws DBus.Error;
        public abstract void reload_config() throws DBus.Error;
        public abstract string get_id() throws DBus.Error;
        public signal void name_owner_changed(string param0, string param1, string param2);
        public signal void name_lost(string param0);
        public signal void name_acquired(string param0);
    }
    public const string DBUS_PATH = "/org/freedesktop/DBus";
    public const string DBUS_BUS = "org.freedesktop.DBus";
}
