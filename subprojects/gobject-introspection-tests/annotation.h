/*
SPDX-License-Identifier: GPL-2.0-or-later AND LGPL-2.0-or-later AND MIT
SPDX-FileCopyrightText: 2008-2009 Dan Winship
SPDX-FileCopyrightText: 2008-2010 Colin Walters <walters@verbum.org>
SPDX-FileCopyrightText: 2008-2011 Johan Dahlin
SPDX-FileCopyrightText: 2008 Owen Taylor
SPDX-FileCopyrightText: 2008 Tommi Komulainen
SPDX-FileCopyrightText: 2008 Tristan Van Berkom
SPDX-FileCopyrightText: 2009-2010 Andreas Rottmann <a.rottmann@gmx.at>
SPDX-FileCopyrightText: 2010 litl, LLC
SPDX-FileCopyrightText: 2010 Pavel Holejsovsky
SPDX-FileCopyrightText: 2010 Red Hat, Inc.
SPDX-FileCopyrightText: 2011 Jasper St. Pierre
SPDX-FileCopyrightText: 2013 Stef Walter
SPDX-FileCopyrightText: 2015 Christoph Reiter
SPDX-FileCopyrightText: 2020 Centricular
*/

#pragma once

#include <glib-object.h>
#include <glib.h>

#include "gitestmacros.h"

typedef enum /*< flags,prefix=ANN >*/
{
  ANN_FLAG_FOO = 1,
  ANN_FLAG_BAR = 2,
  /**
   * ANN_FLAG_FOOBAR:
   *
   * Since: 1.4
   */
  ANN_FLAG_FOOBAR = 3,
} RegressAnnotationBitfield;

/**
 * RegressAnnotationCallback:
 * @in: (in) (transfer none): array of ints
 *
 * This is a callback.
 * Return value: (transfer none): array of ints
 */
typedef const gint *(*RegressAnnotationCallback) (const gint *in);

/**
 * RegressAnnotationListCallback:
 * @in: (in) (transfer none) (element-type utf8): list of strings
 *
 * This is a callback taking a list.
 * Return value: (transfer container) (element-type utf8): list of strings
 */
typedef GList *(*RegressAnnotationListCallback) (GList *in);

/**
 * RegressAnnotationNotifyFunc:
 * @data: (closure): The user data
 *
 * This is a callback with a 'closure' argument that is not named
 * 'user_data' and hence has to be annotated.
 */
typedef void (*RegressAnnotationNotifyFunc) (gpointer data);

/**
 * RegressAnnotationObject: (attributes org.example.Test=cows)
 *
 * This is an object used to test annotations.
 */
typedef struct _RegressAnnotationObject RegressAnnotationObject;
typedef struct _RegressAnnotationObjectClass RegressAnnotationObjectClass;

typedef void (*RegressAnnotationForeachFunc) (RegressAnnotationObject *object,
                                              const char *item,
                                              gpointer user_data);

struct _RegressAnnotationObject
{
  GObject parent_instance;
  /*< private >*/
  void *user_data;
  GDestroyNotify destroy_notify;
};

struct _RegressAnnotationObjectClass
{
  GObjectClass parent_class;
};

GI_TEST_EXTERN
GType regress_annotation_object_get_type (void);

GI_TEST_EXTERN
gint regress_annotation_object_method (RegressAnnotationObject *object);

GI_TEST_EXTERN
gint regress_annotation_object_out (RegressAnnotationObject *object,
                                    int *outarg);

GI_TEST_EXTERN
GObject *regress_annotation_object_create_object (RegressAnnotationObject *object);

GI_TEST_EXTERN
GObject *regress_annotation_object_allow_none (RegressAnnotationObject *object,
                                               const gchar *somearg);

GI_TEST_EXTERN
GObject *regress_annotation_object_notrans (RegressAnnotationObject *object);

GI_TEST_EXTERN
gint regress_annotation_object_inout (RegressAnnotationObject *object,
                                      int *inoutarg);

GI_TEST_EXTERN
gint regress_annotation_object_inout2 (RegressAnnotationObject *object,
                                       int *inoutarg);

GI_TEST_EXTERN
gint regress_annotation_object_inout3 (RegressAnnotationObject *object,
                                       int *inoutarg);

GI_TEST_EXTERN
gint regress_annotation_object_in (RegressAnnotationObject *object,
                                   int *inarg);

GI_TEST_EXTERN
gint regress_annotation_object_calleeowns (RegressAnnotationObject *object,
                                           GObject **toown);

GI_TEST_EXTERN
gint regress_annotation_object_calleesowns (RegressAnnotationObject *object,
                                            GObject **toown1,
                                            GObject **toown2);

GI_TEST_EXTERN
GList *regress_annotation_object_get_strings (RegressAnnotationObject *object);

GI_TEST_EXTERN
GHashTable *regress_annotation_object_get_hash (RegressAnnotationObject *object);

GI_TEST_EXTERN
void regress_annotation_object_with_voidp (RegressAnnotationObject *object,
                                           void *data);

GI_TEST_EXTERN
GSList *regress_annotation_object_get_objects (RegressAnnotationObject *object);

GI_TEST_EXTERN
void regress_annotation_object_use_buffer (RegressAnnotationObject *object,
                                           guchar *bytes);

GI_TEST_EXTERN
void regress_annotation_object_compute_sum (RegressAnnotationObject *object,
                                            int *nums);

GI_TEST_EXTERN
void regress_annotation_object_compute_sum_n (RegressAnnotationObject *object,
                                              int *nums,
                                              int n_nums);

GI_TEST_EXTERN
void regress_annotation_object_compute_sum_nz (RegressAnnotationObject *object,
                                               int *nums,
                                               int n_nums);

GI_TEST_EXTERN
void regress_annotation_object_parse_args (RegressAnnotationObject *object,
                                           int *argc,
                                           char ***argv);

GI_TEST_EXTERN
gboolean regress_annotation_object_string_out (RegressAnnotationObject *object,
                                               char **str_out);

GI_TEST_EXTERN
void regress_annotation_object_foreach (RegressAnnotationObject *object,
                                        RegressAnnotationForeachFunc func,
                                        gpointer user_data);

GI_TEST_EXTERN
void regress_annotation_object_set_data (RegressAnnotationObject *object,
                                         const guchar *data,
                                         gsize length);

GI_TEST_EXTERN
void regress_annotation_object_set_data2 (RegressAnnotationObject *object,
                                          const gchar *data,
                                          gsize length);

GI_TEST_EXTERN
void regress_annotation_object_set_data3 (RegressAnnotationObject *object,
                                          gpointer data,
                                          gsize length);

GI_TEST_EXTERN
GObject *regress_annotation_object_do_not_use (RegressAnnotationObject *object);

GI_TEST_EXTERN
void regress_annotation_object_watch (RegressAnnotationObject *object,
                                      RegressAnnotationForeachFunc func,
                                      gpointer user_data);

GI_TEST_EXTERN
void regress_annotation_object_watch_full (RegressAnnotationObject *object,
                                           RegressAnnotationForeachFunc func,
                                           gpointer user_data,
                                           GDestroyNotify destroy);

GI_TEST_EXTERN
void regress_annotation_object_hidden_self (gpointer object);

GI_TEST_EXTERN
void regress_annotation_init (int *argc,
                              char ***argv);

GI_TEST_EXTERN
char **regress_annotation_return_array (int *length);

GI_TEST_EXTERN
void regress_annotation_versioned (void);

GI_TEST_EXTERN
char **regress_annotation_string_zero_terminated (void);

GI_TEST_EXTERN
void regress_annotation_string_zero_terminated_out (char ***out);

GI_TEST_EXTERN
void regress_annotation_string_array_length (guint n_properties, const gchar *const properties[]);

GI_TEST_EXTERN
void regress_annotation_object_extra_annos (RegressAnnotationObject *object);

GI_TEST_EXTERN
void regress_annotation_custom_destroy (RegressAnnotationCallback callback,
                                        RegressAnnotationNotifyFunc destroy,
                                        gpointer data);

GI_TEST_EXTERN
void regress_annotation_custom_destroy_cleanup (void);

GI_TEST_EXTERN
char *regress_annotation_get_source_file (void);

GI_TEST_EXTERN
void regress_annotation_set_source_file (const char *fname);

GI_TEST_EXTERN
gint regress_annotation_attribute_func (RegressAnnotationObject *object,
                                        const gchar *data);

GI_TEST_EXTERN
void regress_annotation_invalid_regress_annotation (int foo);

/**
 * RegressAnnotationStruct:
 *
 * This is a test of an array of object in an field of a struct.
 */
struct RegressAnnotationStruct
{
  RegressAnnotationObject *objects[10];
};

/**
 * RegressAnnotationFields:
 * @field1: Some documentation
 * @arr: (array length=len): an array of length @len
 * @len: the length of array
 *
 * This is a struct for testing field documentation and annotations
 */
struct RegressAnnotationFields
{
  int field1;
  guchar *arr;
  gulong len;
  /**
   * RegressAnnotationFields.field4:
   *
   * A new field, breaking ABI is fun!
   *
   * Since: 1.4
   */
  guint field4;
};

GI_TEST_EXTERN
void regress_annotation_ptr_array (GPtrArray *array);

GI_TEST_EXTERN
GObject *regress_annotation_test_parsing_bug630862 (void);

GI_TEST_EXTERN
void regress_annotation_space_after_comment_bug631690 (void);

GI_TEST_EXTERN
gchar *regress_annotation_return_filename (void);

GI_TEST_EXTERN
GObject *regress_annotation_transfer_floating (GObject *object);

/* This one we can handle properly */
#define REGRESS_ANNOTATION_CALCULATED_DEFINE (10 * 10)

/**
 * REGRESS_ANNOTATION_CALCULATED_LARGE: (value 10000000000UL)
 *
 * Constant to define a calculated large value
 *
 * Since: 1.4
 */
#define REGRESS_ANNOTATION_CALCULATED_LARGE (1000 * G_GINT64_CONSTANT (10000000))

/**
 * REGRESS_ANNOTATION_CALCULATED_LARGE_DIV: (value 1000000UL)
 *
 * Constant to define a calculated large value
 */
#define REGRESS_ANNOTATION_CALCULATED_LARGE_DIV (1000 / G_GINT64_CONSTANT (10000000))
