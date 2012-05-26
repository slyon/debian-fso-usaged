/* lowlevel.c generated by valac 0.14.2, the Vala compiler
 * generated from lowlevel.vala, do not modify */

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
#include <fsobasics.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define FSO_USAGE_TYPE_RESUME_REASON (fso_usage_resume_reason_get_type ())

#define FSO_USAGE_TYPE_LOW_LEVEL (fso_usage_low_level_get_type ())
#define FSO_USAGE_LOW_LEVEL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), FSO_USAGE_TYPE_LOW_LEVEL, FsoUsageLowLevel))
#define FSO_USAGE_IS_LOW_LEVEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FSO_USAGE_TYPE_LOW_LEVEL))
#define FSO_USAGE_LOW_LEVEL_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), FSO_USAGE_TYPE_LOW_LEVEL, FsoUsageLowLevelIface))

typedef struct _FsoUsageLowLevel FsoUsageLowLevel;
typedef struct _FsoUsageLowLevelIface FsoUsageLowLevelIface;

#define FSO_USAGE_TYPE_NULL_LOW_LEVEL (fso_usage_null_low_level_get_type ())
#define FSO_USAGE_NULL_LOW_LEVEL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), FSO_USAGE_TYPE_NULL_LOW_LEVEL, FsoUsageNullLowLevel))
#define FSO_USAGE_NULL_LOW_LEVEL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), FSO_USAGE_TYPE_NULL_LOW_LEVEL, FsoUsageNullLowLevelClass))
#define FSO_USAGE_IS_NULL_LOW_LEVEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FSO_USAGE_TYPE_NULL_LOW_LEVEL))
#define FSO_USAGE_IS_NULL_LOW_LEVEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FSO_USAGE_TYPE_NULL_LOW_LEVEL))
#define FSO_USAGE_NULL_LOW_LEVEL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), FSO_USAGE_TYPE_NULL_LOW_LEVEL, FsoUsageNullLowLevelClass))

typedef struct _FsoUsageNullLowLevel FsoUsageNullLowLevel;
typedef struct _FsoUsageNullLowLevelClass FsoUsageNullLowLevelClass;
typedef struct _FsoUsageNullLowLevelPrivate FsoUsageNullLowLevelPrivate;

typedef enum  {
	FSO_USAGE_RESUME_REASON_Invalid = 0,
	FSO_USAGE_RESUME_REASON_Accelerometer,
	FSO_USAGE_RESUME_REASON_AuxKey,
	FSO_USAGE_RESUME_REASON_Bluetooth,
	FSO_USAGE_RESUME_REASON_Debug,
	FSO_USAGE_RESUME_REASON_FullBattery,
	FSO_USAGE_RESUME_REASON_GSM,
	FSO_USAGE_RESUME_REASON_GFX,
	FSO_USAGE_RESUME_REASON_GPS,
	FSO_USAGE_RESUME_REASON_Headphone,
	FSO_USAGE_RESUME_REASON_LowBattery,
	FSO_USAGE_RESUME_REASON_PMU,
	FSO_USAGE_RESUME_REASON_PowerKey,
	FSO_USAGE_RESUME_REASON_PhoneKey,
	FSO_USAGE_RESUME_REASON_Unknown,
	FSO_USAGE_RESUME_REASON_WiFi,
	FSO_USAGE_RESUME_REASON_RingerSwitch,
	FSO_USAGE_RESUME_REASON_Slider,
	FSO_USAGE_RESUME_REASON_Keypad,
	FSO_USAGE_RESUME_REASON_RTC
} FsoUsageResumeReason;

struct _FsoUsageLowLevelIface {
	GTypeInterface parent_iface;
	void (*suspend) (FsoUsageLowLevel* self);
	FsoUsageResumeReason (*resume) (FsoUsageLowLevel* self);
};

struct _FsoUsageNullLowLevel {
	FsoFrameworkAbstractObject parent_instance;
	FsoUsageNullLowLevelPrivate * priv;
};

struct _FsoUsageNullLowLevelClass {
	FsoFrameworkAbstractObjectClass parent_class;
};


static gpointer fso_usage_null_low_level_parent_class = NULL;
static FsoUsageLowLevelIface* fso_usage_null_low_level_fso_usage_low_level_parent_iface = NULL;

GType fso_usage_resume_reason_get_type (void) G_GNUC_CONST;
GType fso_usage_low_level_get_type (void) G_GNUC_CONST;
gboolean fso_usage_low_level_isUserInitiated (FsoUsageLowLevel* self, FsoUsageResumeReason r);
void fso_usage_low_level_suspend (FsoUsageLowLevel* self);
FsoUsageResumeReason fso_usage_low_level_resume (FsoUsageLowLevel* self);
GType fso_usage_null_low_level_get_type (void) G_GNUC_CONST;
enum  {
	FSO_USAGE_NULL_LOW_LEVEL_DUMMY_PROPERTY
};
static gchar* fso_usage_null_low_level_real_repr (FsoFrameworkAbstractObject* base);
static void fso_usage_null_low_level_real_suspend (FsoUsageLowLevel* base);
static FsoUsageResumeReason fso_usage_null_low_level_real_resume (FsoUsageLowLevel* base);
FsoUsageNullLowLevel* fso_usage_null_low_level_new (void);
FsoUsageNullLowLevel* fso_usage_null_low_level_construct (GType object_type);


GType fso_usage_resume_reason_get_type (void) {
	static volatile gsize fso_usage_resume_reason_type_id__volatile = 0;
	if (g_once_init_enter (&fso_usage_resume_reason_type_id__volatile)) {
		static const GEnumValue values[] = {{FSO_USAGE_RESUME_REASON_Invalid, "FSO_USAGE_RESUME_REASON_Invalid", "invalid"}, {FSO_USAGE_RESUME_REASON_Accelerometer, "FSO_USAGE_RESUME_REASON_Accelerometer", "accelerometer"}, {FSO_USAGE_RESUME_REASON_AuxKey, "FSO_USAGE_RESUME_REASON_AuxKey", "auxkey"}, {FSO_USAGE_RESUME_REASON_Bluetooth, "FSO_USAGE_RESUME_REASON_Bluetooth", "bluetooth"}, {FSO_USAGE_RESUME_REASON_Debug, "FSO_USAGE_RESUME_REASON_Debug", "debug"}, {FSO_USAGE_RESUME_REASON_FullBattery, "FSO_USAGE_RESUME_REASON_FullBattery", "fullbattery"}, {FSO_USAGE_RESUME_REASON_GSM, "FSO_USAGE_RESUME_REASON_GSM", "gsm"}, {FSO_USAGE_RESUME_REASON_GFX, "FSO_USAGE_RESUME_REASON_GFX", "gfx"}, {FSO_USAGE_RESUME_REASON_GPS, "FSO_USAGE_RESUME_REASON_GPS", "gps"}, {FSO_USAGE_RESUME_REASON_Headphone, "FSO_USAGE_RESUME_REASON_Headphone", "headphone"}, {FSO_USAGE_RESUME_REASON_LowBattery, "FSO_USAGE_RESUME_REASON_LowBattery", "lowbattery"}, {FSO_USAGE_RESUME_REASON_PMU, "FSO_USAGE_RESUME_REASON_PMU", "pmu"}, {FSO_USAGE_RESUME_REASON_PowerKey, "FSO_USAGE_RESUME_REASON_PowerKey", "powerkey"}, {FSO_USAGE_RESUME_REASON_PhoneKey, "FSO_USAGE_RESUME_REASON_PhoneKey", "phonekey"}, {FSO_USAGE_RESUME_REASON_Unknown, "FSO_USAGE_RESUME_REASON_Unknown", "unknown"}, {FSO_USAGE_RESUME_REASON_WiFi, "FSO_USAGE_RESUME_REASON_WiFi", "wifi"}, {FSO_USAGE_RESUME_REASON_RingerSwitch, "FSO_USAGE_RESUME_REASON_RingerSwitch", "ringerswitch"}, {FSO_USAGE_RESUME_REASON_Slider, "FSO_USAGE_RESUME_REASON_Slider", "slider"}, {FSO_USAGE_RESUME_REASON_Keypad, "FSO_USAGE_RESUME_REASON_Keypad", "keypad"}, {FSO_USAGE_RESUME_REASON_RTC, "FSO_USAGE_RESUME_REASON_RTC", "rtc"}, {0, NULL, NULL}};
		GType fso_usage_resume_reason_type_id;
		fso_usage_resume_reason_type_id = g_enum_register_static ("FsoUsageResumeReason", values);
		g_once_init_leave (&fso_usage_resume_reason_type_id__volatile, fso_usage_resume_reason_type_id);
	}
	return fso_usage_resume_reason_type_id__volatile;
}


/**
     * Whether a resume reason is user initiated or not
     **/
gboolean fso_usage_low_level_isUserInitiated (FsoUsageLowLevel* self, FsoUsageResumeReason r) {
	gboolean result = FALSE;
	gboolean res;
	FsoUsageResumeReason _tmp0_;
	res = FALSE;
	_tmp0_ = r;
	switch (_tmp0_) {
		case FSO_USAGE_RESUME_REASON_Accelerometer:
		{
			res = TRUE;
			break;
		}
		case FSO_USAGE_RESUME_REASON_Headphone:
		{
			res = TRUE;
			break;
		}
		case FSO_USAGE_RESUME_REASON_PowerKey:
		{
			res = TRUE;
			break;
		}
		case FSO_USAGE_RESUME_REASON_PhoneKey:
		{
			res = TRUE;
			break;
		}
		case FSO_USAGE_RESUME_REASON_Unknown:
		{
			res = TRUE;
			break;
		}
		default:
		break;
	}
	result = res;
	return result;
}


/**
     * Suspend the device
     **/
void fso_usage_low_level_suspend (FsoUsageLowLevel* self) {
	g_return_if_fail (self != NULL);
	FSO_USAGE_LOW_LEVEL_GET_INTERFACE (self)->suspend (self);
}


/**
     * Resume the device, return the resume reason, if available
     **/
FsoUsageResumeReason fso_usage_low_level_resume (FsoUsageLowLevel* self) {
	g_return_val_if_fail (self != NULL, 0);
	return FSO_USAGE_LOW_LEVEL_GET_INTERFACE (self)->resume (self);
}


static void fso_usage_low_level_base_init (FsoUsageLowLevelIface * iface) {
	static gboolean initialized = FALSE;
	if (!initialized) {
		initialized = TRUE;
	}
}


GType fso_usage_low_level_get_type (void) {
	static volatile gsize fso_usage_low_level_type_id__volatile = 0;
	if (g_once_init_enter (&fso_usage_low_level_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (FsoUsageLowLevelIface), (GBaseInitFunc) fso_usage_low_level_base_init, (GBaseFinalizeFunc) NULL, (GClassInitFunc) NULL, (GClassFinalizeFunc) NULL, NULL, 0, 0, (GInstanceInitFunc) NULL, NULL };
		GType fso_usage_low_level_type_id;
		fso_usage_low_level_type_id = g_type_register_static (G_TYPE_INTERFACE, "FsoUsageLowLevel", &g_define_type_info, 0);
		g_type_interface_add_prerequisite (fso_usage_low_level_type_id, FSO_FRAMEWORK_TYPE_ABSTRACT_OBJECT);
		g_once_init_leave (&fso_usage_low_level_type_id__volatile, fso_usage_low_level_type_id);
	}
	return fso_usage_low_level_type_id__volatile;
}


static gchar* fso_usage_null_low_level_real_repr (FsoFrameworkAbstractObject* base) {
	FsoUsageNullLowLevel * self;
	gchar* result = NULL;
	gchar* _tmp0_;
	self = (FsoUsageNullLowLevel*) base;
	_tmp0_ = g_strdup ("<>");
	result = _tmp0_;
	return result;
}


static void fso_usage_null_low_level_real_suspend (FsoUsageLowLevel* base) {
	FsoUsageNullLowLevel * self;
	FsoFrameworkLogger* _tmp0_;
	self = (FsoUsageNullLowLevel*) base;
	_tmp0_ = ((FsoFrameworkAbstractObject*) self)->logger;
	fso_framework_logger_warning (_tmp0_, "NullLowlevel::suspend() - this is probably not what you want. Sleeping" \
" 5 seconds...");
	sleep ((guint) 5);
}


static FsoUsageResumeReason fso_usage_null_low_level_real_resume (FsoUsageLowLevel* base) {
	FsoUsageNullLowLevel * self;
	FsoUsageResumeReason result = 0;
	FsoFrameworkLogger* _tmp0_;
	self = (FsoUsageNullLowLevel*) base;
	_tmp0_ = ((FsoFrameworkAbstractObject*) self)->logger;
	fso_framework_logger_warning (_tmp0_, "NullLowlevel::resume() - this is probably not what you want. Resume re" \
"ason will be unknown!");
	result = FSO_USAGE_RESUME_REASON_Unknown;
	return result;
}


FsoUsageNullLowLevel* fso_usage_null_low_level_construct (GType object_type) {
	FsoUsageNullLowLevel * self = NULL;
	self = (FsoUsageNullLowLevel*) fso_framework_abstract_object_construct (object_type);
	return self;
}


FsoUsageNullLowLevel* fso_usage_null_low_level_new (void) {
	return fso_usage_null_low_level_construct (FSO_USAGE_TYPE_NULL_LOW_LEVEL);
}


static void fso_usage_null_low_level_class_init (FsoUsageNullLowLevelClass * klass) {
	fso_usage_null_low_level_parent_class = g_type_class_peek_parent (klass);
	FSO_FRAMEWORK_ABSTRACT_OBJECT_CLASS (klass)->repr = fso_usage_null_low_level_real_repr;
}


static void fso_usage_null_low_level_fso_usage_low_level_interface_init (FsoUsageLowLevelIface * iface) {
	fso_usage_null_low_level_fso_usage_low_level_parent_iface = g_type_interface_peek_parent (iface);
	iface->suspend = (void (*)(FsoUsageLowLevel*)) fso_usage_null_low_level_real_suspend;
	iface->resume = (FsoUsageResumeReason (*)(FsoUsageLowLevel*)) fso_usage_null_low_level_real_resume;
}


static void fso_usage_null_low_level_instance_init (FsoUsageNullLowLevel * self) {
}


GType fso_usage_null_low_level_get_type (void) {
	static volatile gsize fso_usage_null_low_level_type_id__volatile = 0;
	if (g_once_init_enter (&fso_usage_null_low_level_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (FsoUsageNullLowLevelClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) fso_usage_null_low_level_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (FsoUsageNullLowLevel), 0, (GInstanceInitFunc) fso_usage_null_low_level_instance_init, NULL };
		static const GInterfaceInfo fso_usage_low_level_info = { (GInterfaceInitFunc) fso_usage_null_low_level_fso_usage_low_level_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		GType fso_usage_null_low_level_type_id;
		fso_usage_null_low_level_type_id = g_type_register_static (FSO_FRAMEWORK_TYPE_ABSTRACT_OBJECT, "FsoUsageNullLowLevel", &g_define_type_info, 0);
		g_type_add_interface_static (fso_usage_null_low_level_type_id, FSO_USAGE_TYPE_LOW_LEVEL, &fso_usage_low_level_info);
		g_once_init_leave (&fso_usage_null_low_level_type_id__volatile, fso_usage_null_low_level_type_id);
	}
	return fso_usage_null_low_level_type_id__volatile;
}


