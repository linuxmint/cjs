/*
SPDX-License-Identifier: GPL-2.0-or-later AND LGPL-2.0-or-later AND MIT
SPDX-FileCopyrightText: 2012 Colin Walters <walters@verbum.org>
SPDX-FileCopyrightText: 2013 Dieter Verfaillie
*/

#include "warnlib.h"

GQuark
warnlib_unpaired_error_quark (void)
{
  return g_quark_from_static_string ("warnlib-unpaired-error");
}

gboolean
warnlib_throw_unpaired (GError **error)
{
  g_set_error_literal (error, warnlib_unpaired_error_quark (), 0,
                       "Unpaired error");
  return FALSE;
}

typedef WarnLibWhateverIface WarnLibWhateverInterface;
G_DEFINE_INTERFACE (WarnLibWhatever, warnlib_whatever, G_TYPE_OBJECT)

static void
warnlib_whatever_default_init (WarnLibWhateverIface *iface G_GNUC_UNUSED)
{
}

void
warnlib_whatever_do_moo (WarnLibWhatever *self, int x, gpointer y)
{
  WARNLIB_WHATEVER_GET_IFACE (self)->do_moo (self, x, y);
}

/**
 * warnlib_whatever_do_boo:
 * @self: a WarnLibWhatever
 * @x: x parameter
 * @y: y parameter
 *
 * Does boo.
 */
void
warnlib_whatever_do_boo (WarnLibWhatever *self, int x, gpointer y)
{
  WARNLIB_WHATEVER_GET_IFACE (self)->do_boo (self, x, y);
}
