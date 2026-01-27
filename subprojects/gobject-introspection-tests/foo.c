/*
SPDX-License-Identifier: GPL-2.0-or-later AND LGPL-2.0-or-later AND MIT
SPDX-FileCopyrightText: 2007-2010 Johan Dahlin
SPDX-FileCopyrightText: 2008-2010 Colin Walters <walters@verbum.org>
SPDX-FileCopyrightText: 2008 Jürg Billeter
SPDX-FileCopyrightText: 2008 Philip Van Hoof
SPDX-FileCopyrightText: 2009, 2011-2012 Dan Winship
SPDX-FileCopyrightText: 2010 Collabora, Ltd.
SPDX-FileCopyrightText: 2010 litl, LLC
SPDX-FileCopyrightText: 2010 Sugar Labs
SPDX-FileCopyrightText: 2011 Owen Taylor
SPDX-FileCopyrightText: 2011 Red Hat, Inc.
SPDX-FileCopyrightText: 2011 Torsten Schönfeld
SPDX-FileCopyrightText: 2018 Christoph Reiter
SPDX-FileCopyrightText: 2020 Centricular
SPDX-FileCopyrightText: 2024 Simon McVittie
SPDX-FileCopyrightText: 2024 Philip Chimento <philip.chimento@gmail.com>
*/

#include <time.h>

#include "foo.h"

/**
 * SECTION:standalone_section
 *
 * This documentation section isn't associated with an object
 * in particular, but it should be serialized in the gir nevertheless.
 */

/* A hidden type not exposed publicly, similar to GUPNP's XML wrapper
   object */
typedef struct _RegressFooHidden RegressFooHidden;

void regress_foo_private_function (RegressFooObject *regress_foo);
void regress_foo_do_regress_foo (RegressFooInterface *self, int x);
RegressFooHidden *regress_foo_hidden_copy (const RegressFooHidden *boxed);
void regress_foo_hidden_free (RegressFooHidden *boxed);
GType regress_foo_hidden_get_type (void);
RegressFooBoxed *regress_foo_boxed_copy (const RegressFooBoxed *boxed);
void regress_foo_boxed_free (RegressFooBoxed *boxed);
void regress_foo_dbus_data_free (RegressFooDBusData *boxed);
RegressFooDBusData *regress_foo_dbus_data_copy (const RegressFooDBusData *boxed);

typedef struct
{
  int i;
} PrivateStruct;

void
regress_foo_private_function (RegressFooObject *regress_foo G_GNUC_UNUSED)
{
}

GType
regress_foo_interface_get_type (void)
{
  static GType object_type = 0;

  if (!object_type)
    {
      object_type = g_type_register_static_simple (G_TYPE_INTERFACE,
                                                   "RegressFooInterface",
                                                   sizeof (RegressFooInterfaceIface),
                                                   NULL, 0, NULL, 0);

      g_type_interface_add_prerequisite (object_type, G_TYPE_OBJECT);
    }

  return object_type;
}

void
regress_foo_interface_do_regress_foo (RegressFooInterface *self, int x)
{
  REGRESS_FOO_INTERFACE_GET_INTERFACE (self)->do_regress_foo (self, x);
}

void
regress_foo_interface_static_method (int x G_GNUC_UNUSED)
{
}

enum
{
  PROP_0,
  PROP_STRING,
  PROP_HIDDEN
};

enum
{
  SIGNAL,
  LAST_SIGNAL
};

static guint regress_foo_object_signals[LAST_SIGNAL] = { 0 };

static void
regress_foo_regress_foo_interface_init (gpointer g_iface,
                                        gpointer iface_data G_GNUC_UNUSED)
{
  RegressFooInterfaceIface *iface = (RegressFooInterfaceIface *) g_iface;
  iface->do_regress_foo = regress_foo_do_regress_foo;
}

enum
{
  SUBIFACE_DESTROY_EVENT,
  SUBIFACE_LAST_SIGNAL
};

static void
regress_foo_sub_interface_class_init (gpointer g_class, gpointer class_data);

static guint regress_foo_subiface_signals[SUBIFACE_LAST_SIGNAL] = { 0 };

GType
regress_foo_sub_interface_get_type (void)
{
  static GType object_type = 0;

  if (!object_type)
    {
      object_type = g_type_register_static_simple (G_TYPE_INTERFACE,
                                                   "RegressFooSubInterface",
                                                   sizeof (RegressFooSubInterfaceIface),
                                                   regress_foo_sub_interface_class_init, 0, NULL, 0);

      g_type_interface_add_prerequisite (object_type, REGRESS_FOO_TYPE_INTERFACE);
    }

  return object_type;
}

static void
regress_foo_sub_interface_class_init (gpointer g_class G_GNUC_UNUSED,
                                      gpointer class_data G_GNUC_UNUSED)
{
  regress_foo_subiface_signals[SUBIFACE_DESTROY_EVENT] =
    g_signal_new ("destroy-event", REGRESS_FOO_TYPE_SUBINTERFACE,
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (RegressFooSubInterfaceIface, destroy_event),
                  NULL, NULL, NULL,
                  G_TYPE_NONE,
                  0,
                  G_TYPE_NONE);
}

void
regress_foo_sub_interface_do_bar (RegressFooSubInterface *self)
{
  REGRESS_FOO_SUBINTERFACE_GET_INTERFACE (self)->do_bar (self);
}

/**
 * regress_foo_sub_interface_do_baz:
 * @self:
 * @callback: (scope call):
 * @user_data:
 */
void
regress_foo_sub_interface_do_baz (RegressFooSubInterface *self, GCallback callback, gpointer user_data)
{
  REGRESS_FOO_SUBINTERFACE_GET_INTERFACE (self)->do_baz (self, callback, user_data);
}

G_DEFINE_TYPE_EXTENDED (RegressFooObject, regress_foo_object, G_TYPE_OBJECT, 0, G_IMPLEMENT_INTERFACE (REGRESS_FOO_TYPE_INTERFACE, regress_foo_regress_foo_interface_init));

static void
regress_foo_object_set_property (GObject *object,
                                 guint prop_id,
                                 const GValue *value G_GNUC_UNUSED,
                                 GParamSpec *pspec)
{
  switch (prop_id)
    {
    case PROP_STRING:
      break;
    case PROP_HIDDEN:
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
regress_foo_object_get_property (GObject *object,
                                 guint prop_id,
                                 GValue *value G_GNUC_UNUSED,
                                 GParamSpec *pspec)
{
  switch (prop_id)
    {
    case PROP_STRING:
      break;
    case PROP_HIDDEN:
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
regress_foo_object_class_init (RegressFooObjectClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = regress_foo_object_set_property;
  gobject_class->get_property = regress_foo_object_get_property;

  g_object_class_install_property (gobject_class,
                                   PROP_STRING,
                                   g_param_spec_string ("string",
                                                        "String nick",
                                                        "The String Property Blurb",
                                                        NULL,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
  g_object_class_install_property (gobject_class,
                                   PROP_HIDDEN,
                                   g_param_spec_boxed ("hidden",
                                                       "hidden property",
                                                       "should not be exposed",
                                                       regress_foo_hidden_get_type (),
                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
  regress_foo_object_signals[SIGNAL] =
    g_signal_new ("signal",
                  G_OBJECT_CLASS_TYPE (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  (GSignalCMarshaller) g_cclosure_marshal_STRING__OBJECT_POINTER,
                  G_TYPE_STRING, 2, G_TYPE_OBJECT, G_TYPE_POINTER);
}

static void
regress_foo_object_init (RegressFooObject *object G_GNUC_UNUSED)
{
}

RegressFooObject *
regress_foo_object_new (void)
{
  return g_object_new (REGRESS_FOO_TYPE_OBJECT, NULL);
}

GObject *
regress_foo_object_new_as_super (void)
{
  return g_object_new (REGRESS_FOO_TYPE_OBJECT, NULL);
}

/**
 * regress_foo_object_external_type:
 * @object: a #RegressFooObject
 *
 * Returns: (transfer none): %NULL always
 */
UtilityObject *
regress_foo_object_external_type (RegressFooObject *object G_GNUC_UNUSED)
{
  return NULL;
}

void
regress_foo_object_various (RegressFooObject *object G_GNUC_UNUSED,
                            void *data G_GNUC_UNUSED,
                            GType some_type G_GNUC_UNUSED)
{
}

void
regress_foo_object_take_all (RegressFooObject *object G_GNUC_UNUSED,
                             int x G_GNUC_UNUSED,
                             ...)
{
}

void
regress_foo_do_regress_foo (RegressFooInterface *self G_GNUC_UNUSED,
                            int x G_GNUC_UNUSED)
{
}

void
regress_foo_object_is_it_time_yet (RegressFooObject *object G_GNUC_UNUSED,
                                   time_t time G_GNUC_UNUSED)
{
}

void
regress_foo_object_seek (RegressFooObject *object G_GNUC_UNUSED, off_t offset G_GNUC_UNUSED)
{
}

/**
 * regress_foo_object_new_cookie: (skip)
 * @object:
 * @target:
 *
 * Not sure why this test is here...
 */
RegressFooObjectCookie
regress_foo_object_new_cookie (RegressFooObject *object G_GNUC_UNUSED,
                               const char *target G_GNUC_UNUSED)
{
  return NULL;
}

const char *
regress_foo_object_get_name (RegressFooObject *object G_GNUC_UNUSED)
{
  return "regress_foo";
}

char *
regress_foo_object_dup_name (RegressFooObject *object G_GNUC_UNUSED)
{
  return g_strdup ("regress_foo");
}

void
regress_foo_object_handle_glyph (RegressFooObject *object G_GNUC_UNUSED, UtilityGlyph glyph G_GNUC_UNUSED)
{
}

gboolean
regress_foo_object_virtual_method (RegressFooObject *object, int first_param)
{
  RegressFooObjectClass *klass = REGRESS_FOO_OBJECT_GET_CLASS (object);
  if (!klass->virtual_method)
    return FALSE;
  return klass->virtual_method (object, first_param);
}

/**
 * regress_foo_object_read: (virtual read_fn)
 * @object: obj
 * @offset: offset
 * @length: length
 *
 * Read some stuff.
 */
void
regress_foo_object_read (RegressFooObject *object G_GNUC_UNUSED,
                         int offset G_GNUC_UNUSED,
                         int length G_GNUC_UNUSED)
{
}

/**
 * regress_foo_object_static_meth:
 *
 * Returns:
 */
int
regress_foo_object_static_meth (void)
{
  return 77;
}

/**
 * regress_foo_object_skipped_method: (skip)
 * @object: obj
 *
 * This is only useful from C.
 */
void
regress_foo_object_skipped_method (RegressFooObject *object G_GNUC_UNUSED)
{
}

G_DEFINE_ABSTRACT_TYPE (RegressFooSubobject, regress_foo_subobject, REGRESS_FOO_TYPE_OBJECT);

static void
regress_foo_subobject_class_init (RegressFooSubobjectClass *klass G_GNUC_UNUSED)
{
}

static void
regress_foo_subobject_init (RegressFooSubobject *object G_GNUC_UNUSED)
{
}

/**
 * regress_foo_object_get_default:
 *
 * This function is intended to match clutter_stage_get_default which
 * uses a C sugar return type.
 *
 * Return value: (type RegressFooSubobject) (transfer none): The global #RegressFooSubobject
 */
RegressFooObject *
regress_foo_object_get_default (void)
{
  return NULL;
}

int
regress_foo_init (void)
{
  return REGRESS_FOO_SUCCESS_INT;
}

/**
 * regress_foo_init_argv:
 * @argc:
 * @argv: (array length=argc) (allow-none):
 */
int
regress_foo_init_argv (int argc G_GNUC_UNUSED,
                       char **argv G_GNUC_UNUSED)
{
  return REGRESS_FOO_SUCCESS_INT;
}

/**
 * regress_foo_init_argv_address:
 * @argc: (inout):
 * @argv: (array length=argc) (inout) (allow-none):
 */
int
regress_foo_init_argv_address (int *argc G_GNUC_UNUSED,
                               char ***argv G_GNUC_UNUSED)
{
  return REGRESS_FOO_SUCCESS_INT;
}

GType
regress_foo_enum_type_get_type (void)
{
  static GType etype = 0;
  if (G_UNLIKELY (etype == 0))
    {
      static const GEnumValue values[] = {
        { REGRESS_FOO_ENUM_ALPHA, "REGRESS_FOO_ENUM_ALPHA", "alpha" },
        { REGRESS_FOO_ENUM_BETA, "REGRESS_FOO_ENUM_BETA", "beta" },
        { REGRESS_FOO_ENUM_DELTA, "REGRESS_FOO_ENUM_DELTA", "delta" },
        { 0, NULL, NULL }
      };
      etype = g_enum_register_static (g_intern_static_string ("RegressFooEnumType"), values);
    }
  return etype;
}

int
regress_foo_enum_method (RegressFooEnumType regress_foo_enum G_GNUC_UNUSED)
{
  return 0;
}

int
regress_foo_enum_type_method (RegressFooEnumType regress_foo_enum)
{
  return (regress_foo_enum + 1) % 3;
}

RegressFooEnumType
regress_foo_enum_type_returnv (int x)
{
  return (x + 1) % 3;
}

GType
regress_foo_flags_type_get_type (void)
{
  static GType etype = 0;
  if (G_UNLIKELY (etype == 0))
    {
      static const GFlagsValue values[] = {
        { REGRESS_FOO_FLAGS_FIRST, "REGRESS_FOO_FLAGS_FIRST", "first" },
        { REGRESS_FOO_FLAGS_SECOND, "REGRESS_FOO_FLAGS_SECOND", "second" },
        { REGRESS_FOO_FLAGS_THIRD, "REGRESS_FOO_FLAGS_THIRD", "third" },
        { 0, NULL, NULL }
      };
      etype = g_flags_register_static (g_intern_static_string ("RegressFooFlagsType"), values);
    }
  return etype;
}

struct _RegressFooBoxed
{
  int private;
};

RegressFooBoxed *
regress_foo_boxed_copy (const RegressFooBoxed *boxed)
{
#if GLIB_CHECK_VERSION(2, 67, 5)
  return (RegressFooBoxed *) g_memdup2 (boxed, sizeof (RegressFooBoxed));
#else
  return (RegressFooBoxed *) g_memdup (boxed, sizeof (RegressFooBoxed));
#endif
}

void
regress_foo_boxed_free (RegressFooBoxed *boxed)
{
  g_slice_free (RegressFooBoxed, boxed);
}

GType
regress_foo_boxed_get_type (void)
{
  static GType our_type = 0;

  if (our_type == 0)
    our_type = g_boxed_type_register_static ("RegressFooBoxed",
                                             (GBoxedCopyFunc) regress_foo_boxed_copy,
                                             (GBoxedFreeFunc) regress_foo_boxed_free);
  return our_type;
}

RegressFooBoxed *
regress_foo_boxed_new (void)
{
  return g_slice_new0 (RegressFooBoxed);
}

void
regress_foo_boxed_method (RegressFooBoxed *boxed G_GNUC_UNUSED)
{
}

/* RegressFooDbus */
struct _RegressFooDBusData
{
  double private;
};

RegressFooDBusData *
regress_foo_dbus_data_copy (const RegressFooDBusData *boxed)
{
#if GLIB_CHECK_VERSION(2, 67, 5)
  return (RegressFooDBusData *) g_memdup2 (boxed, sizeof (RegressFooDBusData));
#else
  return (RegressFooDBusData *) g_memdup (boxed, sizeof (RegressFooDBusData));
#endif
}

void
regress_foo_dbus_data_free (RegressFooDBusData *boxed)
{
  g_slice_free (RegressFooDBusData, boxed);
}

GType
regress_foo_dbus_data_get_type (void)
{
  static GType our_type = 0;

  if (our_type == 0)
    our_type = g_boxed_type_register_static ("RegressFooDBusData",
                                             (GBoxedCopyFunc) regress_foo_dbus_data_copy,
                                             (GBoxedFreeFunc) regress_foo_dbus_data_free);
  return our_type;
}

static RegressFooBRect *
regress_foo_brect_copy (const RegressFooBRect *boxed)
{
#if GLIB_CHECK_VERSION(2, 67, 5)
  return (RegressFooBRect *) g_memdup2 (boxed, sizeof (RegressFooBRect));
#else
  return (RegressFooBRect *) g_memdup (boxed, sizeof (RegressFooBRect));
#endif
}

GType
regress_foo_brect_get_type (void)
{
  static GType our_type = 0;

  if (our_type == 0)
    our_type = g_boxed_type_register_static ("RegressFooBRect",
                                             (GBoxedCopyFunc) regress_foo_brect_copy,
                                             (GBoxedFreeFunc) g_free);
  return our_type;
}

RegressFooBRect *
regress_foo_brect_new (double x, double y)
{
  RegressFooBRect *retval = g_new(RegressFooBRect, 1);
  retval->x = x;
  retval->y = y;
  return retval;
}

void
regress_foo_brect_add (RegressFooBRect *b1 G_GNUC_UNUSED, RegressFooBRect *b2 G_GNUC_UNUSED)
{
}

static RegressFooBUnion *
regress_foo_bunion_copy (const RegressFooBUnion *boxed)
{
#if GLIB_CHECK_VERSION(2, 67, 5)
  return (RegressFooBUnion *) g_memdup2 (boxed, sizeof (RegressFooBUnion));
#else
  return (RegressFooBUnion *) g_memdup (boxed, sizeof (RegressFooBUnion));
#endif
}

GType
regress_foo_bunion_get_type (void)
{
  static GType our_type = 0;

  if (our_type == 0)
    our_type = g_boxed_type_register_static ("RegressFooBUnion",
                                             (GBoxedCopyFunc) regress_foo_bunion_copy,
                                             (GBoxedFreeFunc) g_free);
  return our_type;
}

RegressFooBUnion *
regress_foo_bunion_new (void)
{
  return g_new0(RegressFooBUnion, 1);
}

void
regress_foo_test_unsigned (unsigned int uint G_GNUC_UNUSED)
{
}

/**
 * regress_foo_test_string_array:
 * @array: (array zero-terminated=1):
 */
void
regress_foo_test_string_array (char **array G_GNUC_UNUSED)
{
}

/**
 * regress_foo_test_string_array_with_g:
 * @array: (array zero-terminated=1):
 */
void
regress_foo_test_string_array_with_g (gchar **array G_GNUC_UNUSED)
{
}

/**
 * regress_foo_test_array:
 *
 * Returns: (element-type utf8) (transfer container):
 */
GArray *
regress_foo_test_array (void)
{
  return NULL;
}

/**
 * regress_foo_rectangle_new: (skip)
 *
 * This is a C convenience constructor; we have to (skip)
 * it because it's not a boxed type.
 */
RegressFooRectangle *
regress_foo_rectangle_new (int x, int y, int width, int height)
{
  RegressFooRectangle *r = g_slice_new (RegressFooRectangle);
  r->x = x;
  r->y = y;
  r->width = width;
  r->height = height;
  return r;
}

/**
 * regress_foo_rectangle_add:
 * @r1: add to this rect
 * @r2: source rectangle
 */
void
regress_foo_rectangle_add (RegressFooRectangle *r1 G_GNUC_UNUSED,
                           const RegressFooRectangle *r2 G_GNUC_UNUSED)
{
}

void
regress_foo_method_external_references (UtilityObject *object G_GNUC_UNUSED,
                                        UtilityEnumType e G_GNUC_UNUSED,
                                        UtilityFlagType f G_GNUC_UNUSED,
                                        UtilityStruct s G_GNUC_UNUSED)
{
}

/* RegressFooHidden */

struct _RegressFooHidden
{
  char *frob;
};

RegressFooHidden *
regress_foo_hidden_copy (const RegressFooHidden *boxed)
{
#if GLIB_CHECK_VERSION(2, 67, 5)
  return (RegressFooHidden *) g_memdup2 (boxed, sizeof (RegressFooHidden));
#else
  return (RegressFooHidden *) g_memdup (boxed, sizeof (RegressFooHidden));
#endif
}

void
regress_foo_hidden_free (RegressFooHidden *boxed)
{
  g_slice_free (RegressFooHidden, boxed);
}

GType
regress_foo_hidden_get_type (void)
{
  static GType our_type = 0;

  if (our_type == 0)
    our_type = g_boxed_type_register_static ("RegressFooHidden",
                                             (GBoxedCopyFunc) regress_foo_hidden_copy,
                                             (GBoxedFreeFunc) regress_foo_hidden_free);
  return our_type;
}

GType
regress_foo_error_get_type (void)
{
  static GType etype = 0;
  if (G_UNLIKELY (etype == 0))
    {
      static const GEnumValue values[] = {
        { REGRESS_FOO_ERROR_GOOD, "REGRESS_FOO_ERROR_GOOD", "good" },
        { REGRESS_FOO_ERROR_BAD, "REGRESS_FOO_ERROR_BAD", "bad" },
        { REGRESS_FOO_ERROR_UGLY, "REGRESS_FOO_ERROR_UGLY", "ugly" },
        { 0, NULL, NULL }
      };
      etype = g_enum_register_static (g_intern_static_string ("RegressFooError"), values);
    }
  return etype;
}

GQuark
regress_foo_error_quark (void)
{
  return g_quark_from_static_string ("regress_foo-error-quark");
}

void
regress_foo_some_variant (guint x G_GNUC_UNUSED,
                          va_list args G_GNUC_UNUSED)
{
}

GType regress_foo_tile_handler_get_type (void);

/* This setup of a totally hidden parent class matches
 * http://bugzilla.gnome.org/show_bug.cgi?id=561360
 */
#define REGRESS_FOO_TYPE_TILE_HANDLER (regress_foo_tile_handler_get_type ())
#define REGRESS_FOO_TILE_HANDLER(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), REGRESS_FOO_TYPE_TILE_HANDLER, RegressFooTileHandler))
#define REGRESS_FOO_IS_TILE_HANDLER(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), REGRESS_FOO_TYPE_TILE_HANDLER))

struct _RegressFooTileHandler
{
  RegressFooObject parent_instance;
};

struct _RegressFooTileHandlerClass
{
  RegressFooObjectClass parent_class;
};

typedef struct _RegressFooTileHandler RegressFooTileHandler;
typedef struct _RegressFooTileHandlerClass RegressFooTileHandlerClass;

G_DEFINE_TYPE (RegressFooTileHandler, regress_foo_tile_handler, REGRESS_FOO_TYPE_OBJECT);

static void
regress_foo_tile_handler_class_init (RegressFooTileHandlerClass *klass G_GNUC_UNUSED)
{
}

static void
regress_foo_tile_handler_init (RegressFooTileHandler *object G_GNUC_UNUSED)
{
}

struct _RegressFooBuffer
{
  RegressFooTileHandler parent_instance;
};

struct _RegressFooBufferClass
{
  RegressFooTileHandlerClass parent_class;
};

G_DEFINE_TYPE (RegressFooBuffer, regress_foo_buffer, REGRESS_FOO_TYPE_TILE_HANDLER);

static void
regress_foo_buffer_class_init (RegressFooBufferClass *klass G_GNUC_UNUSED)
{
}

static void
regress_foo_buffer_init (RegressFooBuffer *object G_GNUC_UNUSED)
{
}

void
regress_foo_buffer_some_method (RegressFooBuffer *buffer G_GNUC_UNUSED)
{
}

struct _RegressFooOtherObject
{
  GObject parent_instance;
};

struct _RegressFooOtherObjectClass
{
  GObjectClass parent_class;
};

G_DEFINE_TYPE (RegressFooOtherObject, regress_foo_other_object, G_TYPE_OBJECT);

static void
regress_foo_other_object_class_init (RegressFooOtherObjectClass *klass G_GNUC_UNUSED)
{
}

static void
regress_foo_other_object_init (RegressFooOtherObject *object G_GNUC_UNUSED)
{
}

#define REGRESS_FOO_DEFINE_SHOULD_NOT_BE_EXPOSED "should not be exposed"

/**
 * regress_foo_skip_me: (skip)
 * @fs: a #RegressFooSkippable
 *
 * Does something that's only interesting from C and should not be
 * exposed to language bindings.
 */
void
regress_foo_skip_me (RegressFooSkippable fs G_GNUC_UNUSED)
{
}

/**
 * RegressFooForeignStruct: (foreign)
 *
 */

RegressFooForeignStruct *
regress_foo_foreign_struct_new (void)
{
  return g_slice_new0 (RegressFooForeignStruct);
}

RegressFooForeignStruct *
regress_foo_foreign_struct_copy (RegressFooForeignStruct *original)
{
  RegressFooForeignStruct *copy;
  copy = regress_foo_foreign_struct_new ();
  copy->regress_foo = original->regress_foo;
  return copy;
}

/**
 * regress_foo_test_varargs_callback: (skip)
 *
 */
void
regress_foo_test_varargs_callback (gint i G_GNUC_UNUSED,
                                   RegressFooVarargsCallback callback G_GNUC_UNUSED)
{
}

/**
 * regress_foo_test_varargs_callback2: (skip)
 *
 */
void
regress_foo_test_varargs_callback2 (RegressFooVarargsCallback callback G_GNUC_UNUSED)
{
}

/**
 * regress_foo_test_varargs_callback3: (skip)
 *
 */
void
regress_foo_test_varargs_callback3 (RegressFooVarargsCallback callback G_GNUC_UNUSED,
                                    RegressFooVarargsCallback callback2 G_GNUC_UNUSED)
{
}

/**
 * regress_foo_object_a_global_method:
 * @obj: a #UtilityObject
 *
 * This shouldn't be scanned as a method of UtilityObject.
 */
void
regress_foo_object_a_global_method (UtilityObject *obj G_GNUC_UNUSED)
{
}

/**
 * regress_foo_object_append_new_stack_layer:
 *
 * This shouldn't be scanned as a constructor.
 *
 * Returns: (transfer none):
 */
RegressFooOtherObject *
regress_foo_object_append_new_stack_layer (RegressFooObject *obj G_GNUC_UNUSED,
                                           int x G_GNUC_UNUSED)
{
  return NULL;
}

/**
 * regress_foo_not_a_constructor_new:
 *
 * This should be scanned as a top-level function, and shouldn't cause
 * a "Can't find matching type for constructor" warning.
 *
 * Returns: (transfer none):
 */
RegressFooObject *
regress_foo_not_a_constructor_new (void)
{
  return NULL;
}
