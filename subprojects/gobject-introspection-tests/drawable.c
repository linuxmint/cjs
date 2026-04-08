/*
SPDX-License-Identifier: GPL-2.0-or-later AND LGPL-2.0-or-later AND MIT
SPDX-FileCopyrightText: 2008 Colin Walters <walters@verbum.org>
SPDX-FileCopyrightText: 2008 Johan Bilien
SPDX-FileCopyrightText: 2008 Johan Dahlin
*/

#include <gio/gio.h>

#include "drawable.h"

G_DEFINE_ABSTRACT_TYPE (RegressTestInheritDrawable, regress_test_inherit_drawable, G_TYPE_OBJECT);

static void
regress_test_inherit_drawable_class_init (RegressTestInheritDrawableClass *klass G_GNUC_UNUSED)
{
}

static void
regress_test_inherit_drawable_init (RegressTestInheritDrawable *drawable G_GNUC_UNUSED)
{
}

void
regress_test_inherit_drawable_do_foo (RegressTestInheritDrawable *drawable G_GNUC_UNUSED,
                                      int x G_GNUC_UNUSED)
{
}

/**
 * regress_test_inherit_drawable_get_origin:
 * @drawable:
 * @x: (out):
 * @y: (out):
 */
void
regress_test_inherit_drawable_get_origin (RegressTestInheritDrawable *drawable G_GNUC_UNUSED,
                                          int *x,
                                          int *y)
{
  *x = 0;
  *y = 0;
}

/**
 * regress_test_inherit_drawable_get_size:
 * @drawable:
 * @width: (out):
 * @height: (out):
 */
void
regress_test_inherit_drawable_get_size (RegressTestInheritDrawable *drawable G_GNUC_UNUSED,
                                        guint *width,
                                        guint *height)
{
  *width = 42;
  *height = 42;
}

void
regress_test_inherit_drawable_do_foo_maybe_throw (RegressTestInheritDrawable *drawable G_GNUC_UNUSED,
                                                  int x,
                                                  GError **error)
{
  if (x != 42)
    g_set_error (error, G_IO_ERROR, 12, "The answer should be 42!");
}
