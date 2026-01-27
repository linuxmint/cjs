/*
SPDX-License-Identifier: GPL-2.0-or-later AND LGPL-2.0-or-later AND MIT
SPDX-FileCopyrightText: 2008 Colin Walters <walters@verbum.org>
SPDX-FileCopyrightText: 2008 Johan Bilien
SPDX-FileCopyrightText: 2008 Johan Dahlin
*/

#pragma once

#include <glib-object.h>
#include <glib.h>

#include "gitestmacros.h"

typedef struct _RegressTestInheritDrawable RegressTestInheritDrawable;
typedef struct _RegressTestInheritDrawableClass RegressTestInheritDrawableClass;

struct _RegressTestInheritDrawable
{
  GObject parent_instance;
};

struct _RegressTestInheritDrawableClass
{
  GObjectClass parent_class;
};

GI_TEST_EXTERN
GType regress_test_inherit_drawable_get_type (void) G_GNUC_CONST;

GI_TEST_EXTERN
void regress_test_inherit_drawable_do_foo (RegressTestInheritDrawable *drawable, int x);

GI_TEST_EXTERN
void regress_test_inherit_drawable_get_origin (RegressTestInheritDrawable *drawable, int *x, int *y);

GI_TEST_EXTERN
void regress_test_inherit_drawable_get_size (RegressTestInheritDrawable *drawable, guint *width, guint *height);

GI_TEST_EXTERN
void regress_test_inherit_drawable_do_foo_maybe_throw (RegressTestInheritDrawable *drawable, int x, GError **error);

typedef struct _RegressTestInheritPixmapObjectClass RegressTestInheritPixmapObjectClass;

struct _RegressTestInheritPixmapObjectClass
{
  RegressTestInheritDrawableClass parent_class;
};
