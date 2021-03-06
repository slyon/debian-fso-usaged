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


#define LOW_LEVEL_TYPE_ANDROID (low_level_android_get_type ())
#define LOW_LEVEL_ANDROID(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), LOW_LEVEL_TYPE_ANDROID, LowLevelAndroid))
#define LOW_LEVEL_ANDROID_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), LOW_LEVEL_TYPE_ANDROID, LowLevelAndroidClass))
#define LOW_LEVEL_IS_ANDROID(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LOW_LEVEL_TYPE_ANDROID))
#define LOW_LEVEL_IS_ANDROID_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LOW_LEVEL_TYPE_ANDROID))
#define LOW_LEVEL_ANDROID_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), LOW_LEVEL_TYPE_ANDROID, LowLevelAndroidClass))

typedef struct _LowLevelAndroid LowLevelAndroid;
typedef struct _LowLevelAndroidClass LowLevelAndroidClass;
typedef struct _LowLevelAndroidPrivate LowLevelAndroidPrivate;
#define _g_free0(var) (var = (g_free (var), NULL))

struct _LowLevelAndroid {
	FsoFrameworkAbstractObject parent_instance;
	LowLevelAndroidPrivate * priv;
};

struct _LowLevelAndroidClass {
	FsoFrameworkAbstractObjectClass parent_class;
};

struct _LowLevelAndroidPrivate {
	gchar* screenresumetype;
	gchar* screenresumecommand;
};


static gpointer low_level_android_parent_class = NULL;
extern gchar* sys_power_state;
static FsoUsageLowLevelIface* low_level_android_fso_usage_low_level_parent_iface = NULL;
static GType low_level_android_type_id = 0;
gchar* sys_power_state = NULL;

GType low_level_android_get_type (void) G_GNUC_CONST;
GType low_level_android_register_type (GTypeModule * module);
#define LOW_LEVEL_ANDROID_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), LOW_LEVEL_TYPE_ANDROID, LowLevelAndroidPrivate))
enum  {
	LOW_LEVEL_ANDROID_DUMMY_PROPERTY
};
#define LOW_LEVEL_ANDROID_MODULE_NAME "fsousage.lowlevel_android"
static void low_level_android_process_screen_resume (LowLevelAndroid* self);
static void low_level_android_real_suspend (FsoUsageLowLevel* base);
static FsoUsageResumeReason low_level_android_real_resume (FsoUsageLowLevel* base);
static gchar* low_level_android_real_repr (FsoFrameworkAbstractObject* base);
LowLevelAndroid* low_level_android_new (void);
LowLevelAndroid* low_level_android_construct (GType object_type);
static GObject * low_level_android_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void low_level_android_finalize (GObject* obj);
gchar* fso_factory_function (FsoFrameworkSubsystem* subsystem, GError** error);
void fso_register_function (GTypeModule* module);


static const gchar* string_to_string (const gchar* self) {
	const gchar* result = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	result = self;
	return result;
}


static void low_level_android_process_screen_resume (LowLevelAndroid* self) {
	const gchar* _tmp0_;
	g_return_if_fail (self != NULL);
	_tmp0_ = self->priv->screenresumetype;
	if (g_strcmp0 (_tmp0_, "userspace") == 0) {
		const gchar* _tmp1_;
		_tmp1_ = self->priv->screenresumecommand;
		if (g_strcmp0 (_tmp1_, "") != 0) {
			const gchar* _tmp2_;
			gint _tmp3_ = 0;
			gint ok;
			gint _tmp4_;
			_tmp2_ = self->priv->screenresumecommand;
			_tmp3_ = system (_tmp2_);
			ok = _tmp3_;
			_tmp4_ = ok;
			if (_tmp4_ != 0) {
				FsoFrameworkLogger* _tmp5_;
				const gchar* _tmp6_;
				const gchar* _tmp7_ = NULL;
				gint _tmp8_;
				gchar* _tmp9_ = NULL;
				gchar* _tmp10_;
				gchar* _tmp11_ = NULL;
				gchar* _tmp12_;
				_tmp5_ = ((FsoFrameworkAbstractObject*) self)->logger;
				_tmp6_ = self->priv->screenresumecommand;
				_tmp7_ = string_to_string (_tmp6_);
				_tmp8_ = ok;
				_tmp9_ = g_strdup_printf ("%i", _tmp8_);
				_tmp10_ = _tmp9_;
				_tmp11_ = g_strconcat ("Can't execute '", _tmp7_, "' - error code ", _tmp10_, NULL);
				_tmp12_ = _tmp11_;
				fso_framework_logger_error (_tmp5_, _tmp12_);
				_g_free0 (_tmp12_);
				_g_free0 (_tmp10_);
			} else {
				FsoFrameworkLogger* _tmp13_;
				const gchar* _tmp14_;
				const gchar* _tmp15_ = NULL;
				gint _tmp16_;
				gchar* _tmp17_ = NULL;
				gchar* _tmp18_;
				gchar* _tmp19_ = NULL;
				gchar* _tmp20_;
				gboolean _tmp21_ = FALSE;
				FsoFrameworkLogger* _tmp22_;
				gboolean _tmp23_ = FALSE;
				_tmp13_ = ((FsoFrameworkAbstractObject*) self)->logger;
				_tmp14_ = self->priv->screenresumecommand;
				_tmp15_ = string_to_string (_tmp14_);
				_tmp16_ = ok;
				_tmp17_ = g_strdup_printf ("%i", _tmp16_);
				_tmp18_ = _tmp17_;
				_tmp19_ = g_strconcat ("'", _tmp15_, "' executed - return code ", _tmp18_, NULL);
				_tmp20_ = _tmp19_;
				_tmp21_ = fso_framework_logger_debug (_tmp13_, _tmp20_);
				g_assert (_tmp21_);
				_g_free0 (_tmp20_);
				_g_free0 (_tmp18_);
				_tmp22_ = ((FsoFrameworkAbstractObject*) self)->logger;
				_tmp23_ = fso_framework_logger_debug (_tmp22_, "resume executed");
				g_assert (_tmp23_);
			}
		} else {
			FsoFrameworkLogger* _tmp24_;
			gboolean _tmp25_ = FALSE;
			_tmp24_ = ((FsoFrameworkAbstractObject*) self)->logger;
			_tmp25_ = fso_framework_logger_debug (_tmp24_, "empty screen_resume_command");
			g_assert (_tmp25_);
		}
	} else {
		FsoFrameworkLogger* _tmp26_;
		const gchar* _tmp27_;
		const gchar* _tmp28_ = NULL;
		gchar* _tmp29_ = NULL;
		gchar* _tmp30_;
		gboolean _tmp31_ = FALSE;
		_tmp26_ = ((FsoFrameworkAbstractObject*) self)->logger;
		_tmp27_ = self->priv->screenresumetype;
		_tmp28_ = string_to_string (_tmp27_);
		_tmp29_ = g_strconcat ("unsupported screen_resume_type: ", _tmp28_, NULL);
		_tmp30_ = _tmp29_;
		_tmp31_ = fso_framework_logger_debug (_tmp26_, _tmp30_);
		g_assert (_tmp31_);
		_g_free0 (_tmp30_);
	}
}


/**
     * Android/Linux specific suspend function to cope with Android/Linux differences:
     *
     * 1.) Android/Linux kernels do not suspend synchronously.
     *
     * 2.) When a resume event comes in, they rely on userland to decide whether to
     * fully wake up or fall asleep again.
     *
     **/
static void low_level_android_real_suspend (FsoUsageLowLevel* base) {
	LowLevelAndroid * self;
	FsoFrameworkLogger* _tmp0_;
	gboolean _tmp1_ = FALSE;
	const gchar* _tmp2_;
	self = (LowLevelAndroid*) base;
	_tmp0_ = ((FsoFrameworkAbstractObject*) self)->logger;
	_tmp1_ = fso_framework_logger_debug (_tmp0_, "Setting power state 'mem'");
	g_assert (_tmp1_);
	_tmp2_ = sys_power_state;
	fso_framework_file_handling_write ("mem\n", _tmp2_, FALSE);
}


static FsoUsageResumeReason low_level_android_real_resume (FsoUsageLowLevel* base) {
	LowLevelAndroid * self;
	FsoUsageResumeReason result = 0;
	FsoFrameworkLogger* _tmp0_;
	gboolean _tmp1_ = FALSE;
	const gchar* _tmp2_;
	self = (LowLevelAndroid*) base;
	_tmp0_ = ((FsoFrameworkAbstractObject*) self)->logger;
	_tmp1_ = fso_framework_logger_debug (_tmp0_, "Setting power state 'on'");
	g_assert (_tmp1_);
	_tmp2_ = sys_power_state;
	fso_framework_file_handling_write ("on\n", _tmp2_, FALSE);
	low_level_android_process_screen_resume (self);
	result = FSO_USAGE_RESUME_REASON_Unknown;
	return result;
}


static gchar* low_level_android_real_repr (FsoFrameworkAbstractObject* base) {
	LowLevelAndroid * self;
	gchar* result = NULL;
	gchar* _tmp0_;
	self = (LowLevelAndroid*) base;
	_tmp0_ = g_strdup ("<>");
	result = _tmp0_;
	return result;
}


LowLevelAndroid* low_level_android_construct (GType object_type) {
	LowLevelAndroid * self = NULL;
	self = (LowLevelAndroid*) fso_framework_abstract_object_construct (object_type);
	return self;
}


LowLevelAndroid* low_level_android_new (void) {
	return low_level_android_construct (LOW_LEVEL_TYPE_ANDROID);
}


static GObject * low_level_android_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	LowLevelAndroid * self;
	FsoFrameworkLogger* _tmp0_;
	FsoFrameworkSmartKeyFile* _tmp1_;
	gchar* _tmp2_ = NULL;
	gchar* sysfs_root;
	const gchar* _tmp3_;
	gchar* _tmp4_ = NULL;
	const gchar* _tmp5_;
	FsoFrameworkSmartKeyFile* _tmp6_;
	gchar* _tmp7_ = NULL;
	FsoFrameworkSmartKeyFile* _tmp8_;
	gchar* _tmp9_ = NULL;
	parent_class = G_OBJECT_CLASS (low_level_android_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = LOW_LEVEL_ANDROID (obj);
	_tmp0_ = ((FsoFrameworkAbstractObject*) self)->logger;
	fso_framework_logger_info (_tmp0_, "Registering android low level suspend/resume handling");
	_tmp1_ = ((FsoFrameworkAbstractObject*) self)->config;
	_tmp2_ = fso_framework_smart_key_file_stringValue (_tmp1_, "cornucopia", "sysfs_root", "/sys");
	sysfs_root = _tmp2_;
	_tmp3_ = sysfs_root;
	_tmp4_ = g_build_filename (_tmp3_, "power", "state", NULL);
	_g_free0 (sys_power_state);
	sys_power_state = _tmp4_;
	_tmp5_ = sys_power_state;
	fso_framework_file_handling_write ("on\n", _tmp5_, FALSE);
	_tmp6_ = ((FsoFrameworkAbstractObject*) self)->config;
	_tmp7_ = fso_framework_smart_key_file_stringValue (_tmp6_, LOW_LEVEL_ANDROID_MODULE_NAME, "screen_resume_type", "kernel");
	_g_free0 (self->priv->screenresumetype);
	self->priv->screenresumetype = _tmp7_;
	_tmp8_ = ((FsoFrameworkAbstractObject*) self)->config;
	_tmp9_ = fso_framework_smart_key_file_stringValue (_tmp8_, LOW_LEVEL_ANDROID_MODULE_NAME, "screen_resume_command", "");
	_g_free0 (self->priv->screenresumecommand);
	self->priv->screenresumecommand = _tmp9_;
	_g_free0 (sysfs_root);
	return obj;
}


static void low_level_android_class_init (LowLevelAndroidClass * klass) {
	low_level_android_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (LowLevelAndroidPrivate));
	FSO_FRAMEWORK_ABSTRACT_OBJECT_CLASS (klass)->repr = low_level_android_real_repr;
	G_OBJECT_CLASS (klass)->constructor = low_level_android_constructor;
	G_OBJECT_CLASS (klass)->finalize = low_level_android_finalize;
}


static void low_level_android_fso_usage_low_level_interface_init (FsoUsageLowLevelIface * iface) {
	low_level_android_fso_usage_low_level_parent_iface = g_type_interface_peek_parent (iface);
	iface->suspend = (void (*)(FsoUsageLowLevel*)) low_level_android_real_suspend;
	iface->resume = (FsoUsageResumeReason (*)(FsoUsageLowLevel*)) low_level_android_real_resume;
}


static void low_level_android_instance_init (LowLevelAndroid * self) {
	self->priv = LOW_LEVEL_ANDROID_GET_PRIVATE (self);
}


static void low_level_android_finalize (GObject* obj) {
	LowLevelAndroid * self;
	self = LOW_LEVEL_ANDROID (obj);
	_g_free0 (self->priv->screenresumetype);
	_g_free0 (self->priv->screenresumecommand);
	G_OBJECT_CLASS (low_level_android_parent_class)->finalize (obj);
}


GType low_level_android_get_type (void) {
	return low_level_android_type_id;
}


GType low_level_android_register_type (GTypeModule * module) {
	static const GTypeInfo g_define_type_info = { sizeof (LowLevelAndroidClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) low_level_android_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (LowLevelAndroid), 0, (GInstanceInitFunc) low_level_android_instance_init, NULL };
	static const GInterfaceInfo fso_usage_low_level_info = { (GInterfaceInitFunc) low_level_android_fso_usage_low_level_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
	low_level_android_type_id = g_type_module_register_type (module, FSO_FRAMEWORK_TYPE_ABSTRACT_OBJECT, "LowLevelAndroid", &g_define_type_info, 0);
	g_type_module_add_interface (module, low_level_android_type_id, FSO_USAGE_TYPE_LOW_LEVEL, &fso_usage_low_level_info);
	return low_level_android_type_id;
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
	fso_framework_logger_debug (_tmp0_, "lowlevel_android fso_factory_function");
	_tmp1_ = g_strdup (LOW_LEVEL_ANDROID_MODULE_NAME);
	result = _tmp1_;
	return result;
}


void fso_register_function (GTypeModule* module) {
	g_return_if_fail (module != NULL);
	low_level_android_register_type (module);
}



