/* plugin.c generated by valac 0.16.1, the Vala compiler
 * generated from plugin.vala, do not modify */

/*
 * Copyright (C) 2009-2012 Michael 'Mickey' Lauer <mlauer@vanille-media.de>
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
#include <fsobasics.h>
#include <fsousage.h>
#include <stdlib.h>
#include <string.h>
#include <fsoframework.h>


#define LOW_LEVEL_TYPE_KERNEL26 (low_level_kernel26_get_type ())
#define LOW_LEVEL_KERNEL26(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), LOW_LEVEL_TYPE_KERNEL26, LowLevelKernel26))
#define LOW_LEVEL_KERNEL26_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), LOW_LEVEL_TYPE_KERNEL26, LowLevelKernel26Class))
#define LOW_LEVEL_IS_KERNEL26(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LOW_LEVEL_TYPE_KERNEL26))
#define LOW_LEVEL_IS_KERNEL26_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LOW_LEVEL_TYPE_KERNEL26))
#define LOW_LEVEL_KERNEL26_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), LOW_LEVEL_TYPE_KERNEL26, LowLevelKernel26Class))

typedef struct _LowLevelKernel26 LowLevelKernel26;
typedef struct _LowLevelKernel26Class LowLevelKernel26Class;
typedef struct _LowLevelKernel26Private LowLevelKernel26Private;
#define _g_free0(var) (var = (g_free (var), NULL))

struct _LowLevelKernel26 {
	FsoFrameworkAbstractObject parent_instance;
	LowLevelKernel26Private * priv;
};

struct _LowLevelKernel26Class {
	FsoFrameworkAbstractObjectClass parent_class;
};


static gpointer low_level_kernel26_parent_class = NULL;
extern gchar* sys_power_state;
static FsoUsageLowLevelIface* low_level_kernel26_fso_usage_low_level_parent_iface = NULL;
static GType low_level_kernel26_type_id = 0;
gchar* sys_power_state = NULL;

GType low_level_kernel26_get_type (void) G_GNUC_CONST;
GType low_level_kernel26_register_type (GTypeModule * module);
enum  {
	LOW_LEVEL_KERNEL26_DUMMY_PROPERTY
};
static gchar* low_level_kernel26_real_repr (FsoFrameworkAbstractObject* base);
static void low_level_kernel26_real_suspend (FsoUsageLowLevel* base);
static FsoUsageResumeReason low_level_kernel26_real_resume (FsoUsageLowLevel* base);
LowLevelKernel26* low_level_kernel26_new (void);
LowLevelKernel26* low_level_kernel26_construct (GType object_type);
static GObject * low_level_kernel26_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
gchar* fso_factory_function (FsoFrameworkSubsystem* subsystem, GError** error);
void fso_register_function (GTypeModule* module);


static gchar* low_level_kernel26_real_repr (FsoFrameworkAbstractObject* base) {
	LowLevelKernel26 * self;
	gchar* result = NULL;
	gchar* _tmp0_;
	self = (LowLevelKernel26*) base;
	_tmp0_ = g_strdup ("<>");
	result = _tmp0_;
	return result;
}


static void low_level_kernel26_real_suspend (FsoUsageLowLevel* base) {
	LowLevelKernel26 * self;
	const gchar* _tmp0_;
	self = (LowLevelKernel26*) base;
	_tmp0_ = sys_power_state;
	fso_framework_file_handling_write ("mem\n", _tmp0_, FALSE);
}


static FsoUsageResumeReason low_level_kernel26_real_resume (FsoUsageLowLevel* base) {
	LowLevelKernel26 * self;
	FsoUsageResumeReason result = 0;
	self = (LowLevelKernel26*) base;
	result = FSO_USAGE_RESUME_REASON_Unknown;
	return result;
}


LowLevelKernel26* low_level_kernel26_construct (GType object_type) {
	LowLevelKernel26 * self = NULL;
	self = (LowLevelKernel26*) fso_framework_abstract_object_construct (object_type);
	return self;
}


LowLevelKernel26* low_level_kernel26_new (void) {
	return low_level_kernel26_construct (LOW_LEVEL_TYPE_KERNEL26);
}


static GObject * low_level_kernel26_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	LowLevelKernel26 * self;
	FsoFrameworkLogger* _tmp0_;
	FsoFrameworkSmartKeyFile* _tmp1_;
	gchar* _tmp2_ = NULL;
	gchar* sysfs_root;
	const gchar* _tmp3_;
	gchar* _tmp4_ = NULL;
	parent_class = G_OBJECT_CLASS (low_level_kernel26_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = LOW_LEVEL_KERNEL26 (obj);
	_tmp0_ = ((FsoFrameworkAbstractObject*) self)->logger;
	fso_framework_logger_info (_tmp0_, "Registering kernel26 low level suspend/resume handling");
	_tmp1_ = ((FsoFrameworkAbstractObject*) self)->config;
	_tmp2_ = fso_framework_smart_key_file_stringValue (_tmp1_, "cornucopia", "sysfs_root", "/sys");
	sysfs_root = _tmp2_;
	_tmp3_ = sysfs_root;
	_tmp4_ = g_build_filename (_tmp3_, "power", "state", NULL);
	_g_free0 (sys_power_state);
	sys_power_state = _tmp4_;
	_g_free0 (sysfs_root);
	return obj;
}


static void low_level_kernel26_class_init (LowLevelKernel26Class * klass) {
	low_level_kernel26_parent_class = g_type_class_peek_parent (klass);
	FSO_FRAMEWORK_ABSTRACT_OBJECT_CLASS (klass)->repr = low_level_kernel26_real_repr;
	G_OBJECT_CLASS (klass)->constructor = low_level_kernel26_constructor;
}


static void low_level_kernel26_fso_usage_low_level_interface_init (FsoUsageLowLevelIface * iface) {
	low_level_kernel26_fso_usage_low_level_parent_iface = g_type_interface_peek_parent (iface);
	iface->suspend = (void (*)(FsoUsageLowLevel*)) low_level_kernel26_real_suspend;
	iface->resume = (FsoUsageResumeReason (*)(FsoUsageLowLevel*)) low_level_kernel26_real_resume;
}


static void low_level_kernel26_instance_init (LowLevelKernel26 * self) {
}


GType low_level_kernel26_get_type (void) {
	return low_level_kernel26_type_id;
}


GType low_level_kernel26_register_type (GTypeModule * module) {
	static const GTypeInfo g_define_type_info = { sizeof (LowLevelKernel26Class), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) low_level_kernel26_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (LowLevelKernel26), 0, (GInstanceInitFunc) low_level_kernel26_instance_init, NULL };
	static const GInterfaceInfo fso_usage_low_level_info = { (GInterfaceInitFunc) low_level_kernel26_fso_usage_low_level_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
	low_level_kernel26_type_id = g_type_module_register_type (module, FSO_FRAMEWORK_TYPE_ABSTRACT_OBJECT, "LowLevelKernel26", &g_define_type_info, 0);
	g_type_module_add_interface (module, low_level_kernel26_type_id, FSO_USAGE_TYPE_LOW_LEVEL, &fso_usage_low_level_info);
	return low_level_kernel26_type_id;
}


/**
 * This function gets called on plugin initialization time.
 * @return the name of your plugin here
 * @note that it needs to be a name in the format <subsystem>.<plugin>
 * else your module will be unloaded immediately.
 **/
gchar* fso_factory_function (FsoFrameworkSubsystem* subsystem, GError** error) {
	gchar* result = NULL;
	FsoFrameworkLogger* _tmp0_;
	gchar* _tmp1_;
	g_return_val_if_fail (subsystem != NULL, NULL);
	_tmp0_ = fso_framework_theLogger;
	fso_framework_logger_debug (_tmp0_, "lowlevel_kernel26 fso_factory_function");
	_tmp1_ = g_strdup ("fsousage.lowlevel_kernel26");
	result = _tmp1_;
	return result;
}


void fso_register_function (GTypeModule* module) {
	g_return_if_fail (module != NULL);
	low_level_kernel26_register_type (module);
}



