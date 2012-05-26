/* main.c generated by valac 0.14.2, the Vala compiler
 * generated from main.vala, do not modify */

/*
 * Copyright (C) 2009-2011 Michael 'Mickey' Lauer <mlauer@vanille-media.de>
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

#include <glib.h>
#include <glib-object.h>
#include <fsoframework.h>
#include <signal.h>
#include <fsobasics.h>
#include <stdlib.h>
#include <string.h>

#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_main_loop_unref0(var) ((var == NULL) ? NULL : (var = (g_main_loop_unref (var), NULL)))


extern GMainLoop* mainloop;
GMainLoop* mainloop = NULL;
extern FsoFrameworkSubsystem* subsystem;
FsoFrameworkSubsystem* subsystem = NULL;

void sighandler (gint signum);
gint _vala_main (gchar** args, int args_length1);
static void _sighandler_sighandler_t (gint signal);


void sighandler (gint signum) {
	gint _tmp0_;
	FsoFrameworkLogger* _tmp1_;
	gint _tmp2_;
	gchar* _tmp3_ = NULL;
	gchar* _tmp4_;
	FsoFrameworkSubsystem* _tmp5_;
	GMainLoop* _tmp6_;
	_tmp0_ = signum;
	signal (_tmp0_, NULL);
	_tmp1_ = fso_framework_theLogger;
	_tmp2_ = signum;
	_tmp3_ = g_strdup_printf ("received signal -%d, shutting down...", _tmp2_);
	_tmp4_ = _tmp3_;
	fso_framework_logger_info (_tmp1_, _tmp4_);
	_g_free0 (_tmp4_);
	_tmp5_ = subsystem;
	fso_framework_subsystem_shutdown (_tmp5_);
	_tmp6_ = mainloop;
	g_main_loop_quit (_tmp6_);
}


static void _sighandler_sighandler_t (gint signal) {
	sighandler (signal);
}


gint _vala_main (gchar** args, int args_length1) {
	gint result = 0;
	FsoFrameworkDBusSubsystem* _tmp0_;
	FsoFrameworkSubsystem* _tmp1_;
	FsoFrameworkSubsystem* _tmp2_;
	guint _tmp3_ = 0U;
	guint count;
	FsoFrameworkLogger* _tmp4_;
	guint _tmp5_;
	gchar* _tmp6_ = NULL;
	gchar* _tmp7_;
	guint _tmp8_;
	FsoFrameworkLogger* _tmp13_;
	_tmp0_ = fso_framework_dbus_subsystem_new ("fsousage");
	_g_object_unref0 (subsystem);
	subsystem = (FsoFrameworkSubsystem*) _tmp0_;
	_tmp1_ = subsystem;
	fso_framework_subsystem_registerPlugins (_tmp1_);
	_tmp2_ = subsystem;
	_tmp3_ = fso_framework_subsystem_loadPlugins (_tmp2_);
	count = _tmp3_;
	_tmp4_ = fso_framework_theLogger;
	_tmp5_ = count;
	_tmp6_ = g_strdup_printf ("loaded %u plugins", _tmp5_);
	_tmp7_ = _tmp6_;
	fso_framework_logger_info (_tmp4_, _tmp7_);
	_g_free0 (_tmp7_);
	_tmp8_ = count;
	if (_tmp8_ > ((guint) 0)) {
		GMainLoop* _tmp9_;
		FsoFrameworkLogger* _tmp10_;
		GMainLoop* _tmp11_;
		FsoFrameworkLogger* _tmp12_;
		_tmp9_ = g_main_loop_new (NULL, FALSE);
		_g_main_loop_unref0 (mainloop);
		mainloop = _tmp9_;
		_tmp10_ = fso_framework_theLogger;
		fso_framework_logger_info (_tmp10_, "fsousaged => mainloop");
		signal (SIGINT, _sighandler_sighandler_t);
		signal (SIGTERM, _sighandler_sighandler_t);
		signal (SIGBUS, _sighandler_sighandler_t);
		signal (SIGSEGV, _sighandler_sighandler_t);
		_tmp11_ = mainloop;
		g_main_loop_run (_tmp11_);
		_tmp12_ = fso_framework_theLogger;
		fso_framework_logger_info (_tmp12_, "mainloop => fsousaged");
	}
	_tmp13_ = fso_framework_theLogger;
	fso_framework_logger_info (_tmp13_, "fsousaged exit");
	result = 0;
	return result;
}


int main (int argc, char ** argv) {
	g_type_init ();
	return _vala_main (argv, argc);
}


