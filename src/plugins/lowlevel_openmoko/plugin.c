/* plugin.c generated by valac 0.14.2, the Vala compiler
 * generated from plugin.vala, do not modify */

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
#include <stdlib.h>
#include <string.h>
#include <fsobasics.h>
#include <fsousage.h>
#include <fsoframework.h>


#define LOW_LEVEL_TYPE_OPENMOKO (low_level_openmoko_get_type ())
#define LOW_LEVEL_OPENMOKO(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), LOW_LEVEL_TYPE_OPENMOKO, LowLevelOpenmoko))
#define LOW_LEVEL_OPENMOKO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), LOW_LEVEL_TYPE_OPENMOKO, LowLevelOpenmokoClass))
#define LOW_LEVEL_IS_OPENMOKO(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LOW_LEVEL_TYPE_OPENMOKO))
#define LOW_LEVEL_IS_OPENMOKO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LOW_LEVEL_TYPE_OPENMOKO))
#define LOW_LEVEL_OPENMOKO_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), LOW_LEVEL_TYPE_OPENMOKO, LowLevelOpenmokoClass))

typedef struct _LowLevelOpenmoko LowLevelOpenmoko;
typedef struct _LowLevelOpenmokoClass LowLevelOpenmokoClass;
typedef struct _LowLevelOpenmokoPrivate LowLevelOpenmokoPrivate;
#define _g_hash_table_unref0(var) ((var == NULL) ? NULL : (var = (g_hash_table_unref (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))

struct _LowLevelOpenmoko {
	FsoFrameworkAbstractObject parent_instance;
	LowLevelOpenmokoPrivate * priv;
};

struct _LowLevelOpenmokoClass {
	FsoFrameworkAbstractObjectClass parent_class;
};

struct _LowLevelOpenmokoPrivate {
	GHashTable* intmap1;
	GHashTable* intmap2;
};


static gpointer low_level_openmoko_parent_class = NULL;
extern gchar* sys_power_state;
extern gchar* sys_resume_reason;
extern gchar* sys_resume_subreason;
static FsoUsageLowLevelIface* low_level_openmoko_fso_usage_low_level_parent_iface = NULL;
static GType low_level_openmoko_type_id = 0;
gchar* sys_power_state = NULL;
gchar* sys_resume_reason = NULL;
gchar* sys_resume_subreason = NULL;

#define SYSFS_RESUME_REASON_PATH "/class/i2c-adapter/i2c-0/0-0073/neo1973-resume.0/resume_reason"
#define SYSFS_RESUME_SUBREASON_PATH "/class/i2c-adapter/i2c-0/0-0073/resume_reason"
GType low_level_openmoko_get_type (void) G_GNUC_CONST;
GType low_level_openmoko_register_type (GTypeModule * module);
#define LOW_LEVEL_OPENMOKO_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), LOW_LEVEL_TYPE_OPENMOKO, LowLevelOpenmokoPrivate))
enum  {
	LOW_LEVEL_OPENMOKO_DUMMY_PROPERTY
};
static gchar* low_level_openmoko_real_repr (FsoFrameworkAbstractObject* base);
static void low_level_openmoko_real_suspend (FsoUsageLowLevel* base);
static FsoUsageResumeReason low_level_openmoko_real_resume (FsoUsageLowLevel* base);
LowLevelOpenmoko* low_level_openmoko_new (void);
LowLevelOpenmoko* low_level_openmoko_construct (GType object_type);
static GObject * low_level_openmoko_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void _g_free0_ (gpointer var);
static void low_level_openmoko_finalize (GObject* obj);
gchar* fso_factory_function (FsoFrameworkSubsystem* subsystem, GError** error);
void fso_register_function (GTypeModule* module);
static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func);
static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func);
static gint _vala_array_length (gpointer array);


static gchar* low_level_openmoko_real_repr (FsoFrameworkAbstractObject* base) {
	LowLevelOpenmoko * self;
	gchar* result = NULL;
	gchar* _tmp0_;
	self = (LowLevelOpenmoko*) base;
	_tmp0_ = g_strdup ("<>");
	result = _tmp0_;
	return result;
}


static void low_level_openmoko_real_suspend (FsoUsageLowLevel* base) {
	LowLevelOpenmoko * self;
	const gchar* _tmp0_;
	self = (LowLevelOpenmoko*) base;
	_tmp0_ = sys_power_state;
	fso_framework_file_handling_write ("mem\n", _tmp0_, FALSE);
}


static glong string_strnlen (gchar* str, glong maxlen) {
	glong result = 0L;
	gchar* _tmp0_;
	glong _tmp1_;
	gchar* _tmp2_ = NULL;
	gchar* end;
	gchar* _tmp3_;
	_tmp0_ = str;
	_tmp1_ = maxlen;
	_tmp2_ = memchr (_tmp0_, 0, (gsize) _tmp1_);
	end = _tmp2_;
	_tmp3_ = end;
	if (_tmp3_ == NULL) {
		glong _tmp4_;
		_tmp4_ = maxlen;
		result = _tmp4_;
		return result;
	} else {
		gchar* _tmp5_;
		gchar* _tmp6_;
		_tmp5_ = end;
		_tmp6_ = str;
		result = (glong) (_tmp5_ - _tmp6_);
		return result;
	}
}


static gchar* string_substring (const gchar* self, glong offset, glong len) {
	gchar* result = NULL;
	glong string_length = 0L;
	gboolean _tmp0_ = FALSE;
	glong _tmp1_;
	gboolean _tmp3_;
	glong _tmp9_;
	glong _tmp15_;
	glong _tmp18_;
	glong _tmp19_;
	glong _tmp20_;
	glong _tmp21_;
	glong _tmp22_;
	gchar* _tmp23_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp1_ = offset;
	if (_tmp1_ >= ((glong) 0)) {
		glong _tmp2_;
		_tmp2_ = len;
		_tmp0_ = _tmp2_ >= ((glong) 0);
	} else {
		_tmp0_ = FALSE;
	}
	_tmp3_ = _tmp0_;
	if (_tmp3_) {
		glong _tmp4_;
		glong _tmp5_;
		glong _tmp6_ = 0L;
		_tmp4_ = offset;
		_tmp5_ = len;
		_tmp6_ = string_strnlen ((gchar*) self, _tmp4_ + _tmp5_);
		string_length = _tmp6_;
	} else {
		gint _tmp7_;
		gint _tmp8_;
		_tmp7_ = strlen (self);
		_tmp8_ = _tmp7_;
		string_length = (glong) _tmp8_;
	}
	_tmp9_ = offset;
	if (_tmp9_ < ((glong) 0)) {
		glong _tmp10_;
		glong _tmp11_;
		glong _tmp12_;
		_tmp10_ = string_length;
		_tmp11_ = offset;
		offset = _tmp10_ + _tmp11_;
		_tmp12_ = offset;
		g_return_val_if_fail (_tmp12_ >= ((glong) 0), NULL);
	} else {
		glong _tmp13_;
		glong _tmp14_;
		_tmp13_ = offset;
		_tmp14_ = string_length;
		g_return_val_if_fail (_tmp13_ <= _tmp14_, NULL);
	}
	_tmp15_ = len;
	if (_tmp15_ < ((glong) 0)) {
		glong _tmp16_;
		glong _tmp17_;
		_tmp16_ = string_length;
		_tmp17_ = offset;
		len = _tmp16_ - _tmp17_;
	}
	_tmp18_ = offset;
	_tmp19_ = len;
	_tmp20_ = string_length;
	g_return_val_if_fail ((_tmp18_ + _tmp19_) <= _tmp20_, NULL);
	_tmp21_ = offset;
	_tmp22_ = len;
	_tmp23_ = g_strndup (((gchar*) self) + _tmp21_, (gsize) _tmp22_);
	result = _tmp23_;
	return result;
}


static FsoUsageResumeReason low_level_openmoko_real_resume (FsoUsageLowLevel* base) {
	LowLevelOpenmoko * self;
	FsoUsageResumeReason result = 0;
	const gchar* _tmp0_;
	gchar* _tmp1_ = NULL;
	gchar* _tmp2_;
	gchar** _tmp3_;
	gchar** _tmp4_ = NULL;
	gchar** _tmp5_;
	gint _tmp5__length1;
	gchar** reasons;
	gint reasons_length1;
	gint _reasons_size_;
	gchar* _tmp6_;
	gchar* reasonkey;
	gchar** _tmp7_;
	gint _tmp7__length1;
	GHashTable* _tmp13_;
	const gchar* _tmp14_;
	gconstpointer _tmp15_ = NULL;
	guint reasonvalue;
	guint _tmp16_;
	guint _tmp21_;
	guint _tmp37_;
	self = (LowLevelOpenmoko*) base;
	_tmp0_ = sys_resume_reason;
	_tmp1_ = fso_framework_file_handling_read (_tmp0_);
	_tmp2_ = _tmp1_;
	_tmp4_ = _tmp3_ = g_strsplit (_tmp2_, "\n", 0);
	_tmp5_ = _tmp4_;
	_tmp5__length1 = _vala_array_length (_tmp3_);
	_g_free0 (_tmp2_);
	reasons = _tmp5_;
	reasons_length1 = _tmp5__length1;
	_reasons_size_ = reasons_length1;
	_tmp6_ = g_strdup ("unknown");
	reasonkey = _tmp6_;
	_tmp7_ = reasons;
	_tmp7__length1 = reasons_length1;
	{
		gchar** line_collection = NULL;
		gint line_collection_length1 = 0;
		gint _line_collection_size_ = 0;
		gint line_it = 0;
		line_collection = _tmp7_;
		line_collection_length1 = _tmp7__length1;
		for (line_it = 0; line_it < _tmp7__length1; line_it = line_it + 1) {
			gchar* _tmp8_;
			gchar* line = NULL;
			_tmp8_ = g_strdup (line_collection[line_it]);
			line = _tmp8_;
			{
				const gchar* _tmp9_;
				gboolean _tmp10_ = FALSE;
				_tmp9_ = line;
				_tmp10_ = g_str_has_prefix (_tmp9_, "*");
				if (_tmp10_) {
					const gchar* _tmp11_;
					gchar* _tmp12_ = NULL;
					_tmp11_ = line;
					_tmp12_ = string_substring (_tmp11_, (glong) 2, (glong) (-1));
					_g_free0 (reasonkey);
					reasonkey = _tmp12_;
					_g_free0 (line);
					break;
				}
				_g_free0 (line);
			}
		}
	}
	_tmp13_ = self->priv->intmap1;
	_tmp14_ = reasonkey;
	_tmp15_ = g_hash_table_lookup (_tmp13_, _tmp14_);
	reasonvalue = GPOINTER_TO_UINT (_tmp15_);
	_tmp16_ = reasonvalue;
	if (_tmp16_ == ((guint) 0)) {
		FsoFrameworkLogger* _tmp17_;
		const gchar* _tmp18_;
		gchar* _tmp19_ = NULL;
		gchar* _tmp20_;
		_tmp17_ = ((FsoFrameworkAbstractObject*) self)->logger;
		_tmp18_ = sys_resume_reason;
		_tmp19_ = g_strdup_printf ("No resume reason marked in %s", _tmp18_);
		_tmp20_ = _tmp19_;
		fso_framework_logger_info (_tmp17_, _tmp20_);
		_g_free0 (_tmp20_);
		result = FSO_USAGE_RESUME_REASON_Unknown;
		_g_free0 (reasonkey);
		reasons = (_vala_array_free (reasons, reasons_length1, (GDestroyNotify) g_free), NULL);
		return result;
	}
	_tmp21_ = reasonvalue;
	if (_tmp21_ == ((guint) FSO_USAGE_RESUME_REASON_PMU)) {
		FsoFrameworkLogger* _tmp22_;
		const gchar* _tmp23_;
		gchar* _tmp24_ = NULL;
		gchar* _tmp25_;
		const gchar* _tmp26_;
		gchar* _tmp27_ = NULL;
		gchar* subreasonkey;
		GHashTable* _tmp28_;
		const gchar* _tmp29_;
		gconstpointer _tmp30_ = NULL;
		guint subreasonvalue;
		guint _tmp31_;
		guint _tmp36_;
		_tmp22_ = ((FsoFrameworkAbstractObject*) self)->logger;
		_tmp23_ = sys_resume_reason;
		_tmp24_ = g_strdup_printf ("PMU resume reason marked in %s", _tmp23_);
		_tmp25_ = _tmp24_;
		fso_framework_logger_debug (_tmp22_, _tmp25_);
		_g_free0 (_tmp25_);
		_tmp26_ = sys_resume_subreason;
		_tmp27_ = fso_framework_file_handling_read (_tmp26_);
		subreasonkey = _tmp27_;
		_tmp28_ = self->priv->intmap2;
		_tmp29_ = subreasonkey;
		_tmp30_ = g_hash_table_lookup (_tmp28_, _tmp29_);
		subreasonvalue = GPOINTER_TO_UINT (_tmp30_);
		_tmp31_ = subreasonvalue;
		if (_tmp31_ == ((guint) 0)) {
			FsoFrameworkLogger* _tmp32_;
			const gchar* _tmp33_;
			gchar* _tmp34_ = NULL;
			gchar* _tmp35_;
			_tmp32_ = ((FsoFrameworkAbstractObject*) self)->logger;
			_tmp33_ = subreasonkey;
			_tmp34_ = g_strdup_printf ("Unknown subreason %s for PMU resume, please fix me!", _tmp33_);
			_tmp35_ = _tmp34_;
			fso_framework_logger_debug (_tmp32_, _tmp35_);
			_g_free0 (_tmp35_);
			result = FSO_USAGE_RESUME_REASON_PMU;
			_g_free0 (subreasonkey);
			_g_free0 (reasonkey);
			reasons = (_vala_array_free (reasons, reasons_length1, (GDestroyNotify) g_free), NULL);
			return result;
		}
		_tmp36_ = subreasonvalue;
		result = (FsoUsageResumeReason) _tmp36_;
		_g_free0 (subreasonkey);
		_g_free0 (reasonkey);
		reasons = (_vala_array_free (reasons, reasons_length1, (GDestroyNotify) g_free), NULL);
		return result;
	}
	_tmp37_ = reasonvalue;
	result = (FsoUsageResumeReason) _tmp37_;
	_g_free0 (reasonkey);
	reasons = (_vala_array_free (reasons, reasons_length1, (GDestroyNotify) g_free), NULL);
	return result;
}


LowLevelOpenmoko* low_level_openmoko_construct (GType object_type) {
	LowLevelOpenmoko * self = NULL;
	self = (LowLevelOpenmoko*) fso_framework_abstract_object_construct (object_type);
	return self;
}


LowLevelOpenmoko* low_level_openmoko_new (void) {
	return low_level_openmoko_construct (LOW_LEVEL_TYPE_OPENMOKO);
}


static void _g_free0_ (gpointer var) {
	var = (g_free (var), NULL);
}


static GObject * low_level_openmoko_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	LowLevelOpenmoko * self;
	FsoFrameworkLogger* _tmp0_;
	FsoFrameworkSmartKeyFile* _tmp1_;
	gchar* _tmp2_ = NULL;
	gchar* sysfs_root;
	const gchar* _tmp3_;
	gchar* _tmp4_ = NULL;
	const gchar* _tmp5_;
	gchar* _tmp6_ = NULL;
	const gchar* _tmp7_;
	gchar* _tmp8_ = NULL;
	GHashFunc _tmp9_;
	GEqualFunc _tmp10_;
	GHashTable* _tmp11_;
	GHashTable* _tmp12_;
	gchar* _tmp13_;
	GHashTable* _tmp14_;
	gchar* _tmp15_;
	GHashTable* _tmp16_;
	gchar* _tmp17_;
	GHashTable* _tmp18_;
	gchar* _tmp19_;
	GHashTable* _tmp20_;
	gchar* _tmp21_;
	GHashTable* _tmp22_;
	gchar* _tmp23_;
	GHashTable* _tmp24_;
	gchar* _tmp25_;
	GHashTable* _tmp26_;
	gchar* _tmp27_;
	GHashTable* _tmp28_;
	gchar* _tmp29_;
	GHashTable* _tmp30_;
	gchar* _tmp31_;
	GHashTable* _tmp32_;
	gchar* _tmp33_;
	GHashTable* _tmp34_;
	gchar* _tmp35_;
	GHashTable* _tmp36_;
	gchar* _tmp37_;
	GHashTable* _tmp38_;
	gchar* _tmp39_;
	GHashTable* _tmp40_;
	gchar* _tmp41_;
	GHashTable* _tmp42_;
	gchar* _tmp43_;
	GHashFunc _tmp44_;
	GEqualFunc _tmp45_;
	GHashTable* _tmp46_;
	GHashTable* _tmp47_;
	gchar* _tmp48_;
	GHashTable* _tmp49_;
	gchar* _tmp50_;
	parent_class = G_OBJECT_CLASS (low_level_openmoko_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = LOW_LEVEL_OPENMOKO (obj);
	_tmp0_ = ((FsoFrameworkAbstractObject*) self)->logger;
	fso_framework_logger_info (_tmp0_, "Registering openmoko low level suspend/resume handling");
	_tmp1_ = ((FsoFrameworkAbstractObject*) self)->config;
	_tmp2_ = fso_framework_smart_key_file_stringValue (_tmp1_, "cornucopia", "sysfs_root", "/sys");
	sysfs_root = _tmp2_;
	_tmp3_ = sysfs_root;
	_tmp4_ = g_build_filename (_tmp3_, "power", "state", NULL);
	_g_free0 (sys_power_state);
	sys_power_state = _tmp4_;
	_tmp5_ = sysfs_root;
	_tmp6_ = g_build_filename (_tmp5_, SYSFS_RESUME_REASON_PATH, NULL);
	_g_free0 (sys_resume_reason);
	sys_resume_reason = _tmp6_;
	_tmp7_ = sysfs_root;
	_tmp8_ = g_build_filename (_tmp7_, SYSFS_RESUME_SUBREASON_PATH, NULL);
	_g_free0 (sys_resume_subreason);
	sys_resume_subreason = _tmp8_;
	_tmp9_ = g_str_hash;
	_tmp10_ = g_str_equal;
	_tmp11_ = g_hash_table_new_full (_tmp9_, _tmp10_, _g_free0_, NULL);
	_g_hash_table_unref0 (self->priv->intmap1);
	self->priv->intmap1 = _tmp11_;
	_tmp12_ = self->priv->intmap1;
	_tmp13_ = g_strdup ("EINT00_ACCEL1");
	g_hash_table_insert (_tmp12_, _tmp13_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Accelerometer));
	_tmp14_ = self->priv->intmap1;
	_tmp15_ = g_strdup ("EINT01_GSM");
	g_hash_table_insert (_tmp14_, _tmp15_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_GSM));
	_tmp16_ = self->priv->intmap1;
	_tmp17_ = g_strdup ("EINT02_BLUETOOTH");
	g_hash_table_insert (_tmp16_, _tmp17_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Bluetooth));
	_tmp18_ = self->priv->intmap1;
	_tmp19_ = g_strdup ("EINT03_DEBUGBRD");
	g_hash_table_insert (_tmp18_, _tmp19_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Debug));
	_tmp20_ = self->priv->intmap1;
	_tmp21_ = g_strdup ("EINT04_JACK");
	g_hash_table_insert (_tmp20_, _tmp21_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Headphone));
	_tmp22_ = self->priv->intmap1;
	_tmp23_ = g_strdup ("EINT05_WLAN");
	g_hash_table_insert (_tmp22_, _tmp23_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_WiFi));
	_tmp24_ = self->priv->intmap1;
	_tmp25_ = g_strdup ("EINT06_AUXKEY");
	g_hash_table_insert (_tmp24_, _tmp25_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_AuxKey));
	_tmp26_ = self->priv->intmap1;
	_tmp27_ = g_strdup ("EINT07_HOLDKEY");
	g_hash_table_insert (_tmp26_, _tmp27_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Headphone));
	_tmp28_ = self->priv->intmap1;
	_tmp29_ = g_strdup ("EINT08_ACCEL2");
	g_hash_table_insert (_tmp28_, _tmp29_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Accelerometer));
	_tmp30_ = self->priv->intmap1;
	_tmp31_ = g_strdup ("EINT09_PMU");
	g_hash_table_insert (_tmp30_, _tmp31_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_PMU));
	_tmp32_ = self->priv->intmap1;
	_tmp33_ = g_strdup ("EINT10_NULL");
	g_hash_table_insert (_tmp32_, _tmp33_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Invalid));
	_tmp34_ = self->priv->intmap1;
	_tmp35_ = g_strdup ("EINT11_NULL");
	g_hash_table_insert (_tmp34_, _tmp35_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Invalid));
	_tmp36_ = self->priv->intmap1;
	_tmp37_ = g_strdup ("EINT12_GLAMO");
	g_hash_table_insert (_tmp36_, _tmp37_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_GFX));
	_tmp38_ = self->priv->intmap1;
	_tmp39_ = g_strdup ("EINT13_NULL");
	g_hash_table_insert (_tmp38_, _tmp39_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Invalid));
	_tmp40_ = self->priv->intmap1;
	_tmp41_ = g_strdup ("EINT14_NULL");
	g_hash_table_insert (_tmp40_, _tmp41_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Invalid));
	_tmp42_ = self->priv->intmap1;
	_tmp43_ = g_strdup ("EINT15_NULL");
	g_hash_table_insert (_tmp42_, _tmp43_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_Invalid));
	_tmp44_ = g_str_hash;
	_tmp45_ = g_str_equal;
	_tmp46_ = g_hash_table_new_full (_tmp44_, _tmp45_, _g_free0_, NULL);
	_g_hash_table_unref0 (self->priv->intmap2);
	self->priv->intmap2 = _tmp46_;
	_tmp47_ = self->priv->intmap2;
	_tmp48_ = g_strdup ("0000000200");
	g_hash_table_insert (_tmp47_, _tmp48_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_LowBattery));
	_tmp49_ = self->priv->intmap2;
	_tmp50_ = g_strdup ("0002000000");
	g_hash_table_insert (_tmp49_, _tmp50_, GUINT_TO_POINTER ((guint) FSO_USAGE_RESUME_REASON_PowerKey));
	_g_free0 (sysfs_root);
	return obj;
}


static void low_level_openmoko_class_init (LowLevelOpenmokoClass * klass) {
	low_level_openmoko_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (LowLevelOpenmokoPrivate));
	FSO_FRAMEWORK_ABSTRACT_OBJECT_CLASS (klass)->repr = low_level_openmoko_real_repr;
	G_OBJECT_CLASS (klass)->constructor = low_level_openmoko_constructor;
	G_OBJECT_CLASS (klass)->finalize = low_level_openmoko_finalize;
}


static void low_level_openmoko_fso_usage_low_level_interface_init (FsoUsageLowLevelIface * iface) {
	low_level_openmoko_fso_usage_low_level_parent_iface = g_type_interface_peek_parent (iface);
	iface->suspend = (void (*)(FsoUsageLowLevel*)) low_level_openmoko_real_suspend;
	iface->resume = (FsoUsageResumeReason (*)(FsoUsageLowLevel*)) low_level_openmoko_real_resume;
}


static void low_level_openmoko_instance_init (LowLevelOpenmoko * self) {
	self->priv = LOW_LEVEL_OPENMOKO_GET_PRIVATE (self);
}


static void low_level_openmoko_finalize (GObject* obj) {
	LowLevelOpenmoko * self;
	self = LOW_LEVEL_OPENMOKO (obj);
	_g_hash_table_unref0 (self->priv->intmap1);
	_g_hash_table_unref0 (self->priv->intmap2);
	G_OBJECT_CLASS (low_level_openmoko_parent_class)->finalize (obj);
}


GType low_level_openmoko_get_type (void) {
	return low_level_openmoko_type_id;
}


GType low_level_openmoko_register_type (GTypeModule * module) {
	static const GTypeInfo g_define_type_info = { sizeof (LowLevelOpenmokoClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) low_level_openmoko_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (LowLevelOpenmoko), 0, (GInstanceInitFunc) low_level_openmoko_instance_init, NULL };
	static const GInterfaceInfo fso_usage_low_level_info = { (GInterfaceInitFunc) low_level_openmoko_fso_usage_low_level_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
	low_level_openmoko_type_id = g_type_module_register_type (module, FSO_FRAMEWORK_TYPE_ABSTRACT_OBJECT, "LowLevelOpenmoko", &g_define_type_info, 0);
	g_type_module_add_interface (module, low_level_openmoko_type_id, FSO_USAGE_TYPE_LOW_LEVEL, &fso_usage_low_level_info);
	return low_level_openmoko_type_id;
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
	fso_framework_logger_debug (_tmp0_, "lowlevel_openmoko fso_factory_function");
	_tmp1_ = g_strdup ("fsousage.lowlevel_openmoko");
	result = _tmp1_;
	return result;
}


void fso_register_function (GTypeModule* module) {
	g_return_if_fail (module != NULL);
	low_level_openmoko_register_type (module);
}


static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func) {
	if ((array != NULL) && (destroy_func != NULL)) {
		int i;
		for (i = 0; i < array_length; i = i + 1) {
			if (((gpointer*) array)[i] != NULL) {
				destroy_func (((gpointer*) array)[i]);
			}
		}
	}
}


static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func) {
	_vala_array_destroy (array, array_length, destroy_func);
	g_free (array);
}


static gint _vala_array_length (gpointer array) {
	int length;
	length = 0;
	if (array) {
		while (((gpointer*) array)[length]) {
			length++;
		}
	}
	return length;
}



