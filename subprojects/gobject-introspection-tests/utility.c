/*
SPDX-FileCopyrightText: 2008 Johan Dahlin
SPDX-FileCopyrightText: 2009 Andreas Rottmann <a.rottmann@gmx.at>
*/

#include "utility.h"

G_DEFINE_TYPE (UtilityObject, utility_object, G_TYPE_OBJECT);

/**
 * UtilityBuffer:
 * @data: (type gpointer): the data
 *
 **/

static void
utility_object_class_init (UtilityObjectClass *klass G_GNUC_UNUSED)
{
}

static void
utility_object_init (UtilityObject *object G_GNUC_UNUSED)
{
}

/**
 * utility_object_watch_dir:
 * @object:
 * @path:
 * @func: (destroy destroy):
 * @user_data:
 * @destroy:
 */
void
utility_object_watch_dir (UtilityObject *object G_GNUC_UNUSED,
                          const char *path G_GNUC_UNUSED,
                          UtilityFileFunc func G_GNUC_UNUSED,
                          gpointer user_data G_GNUC_UNUSED,
                          GDestroyNotify destroy G_GNUC_UNUSED)
{
}

/**
 * utility_dir_foreach:
 * @path::
 * @func: (scope call):
 * @user_data::
 *
 */
void
utility_dir_foreach (const char *path G_GNUC_UNUSED,
                     UtilityFileFunc func G_GNUC_UNUSED,
                     gpointer user_data G_GNUC_UNUSED)
{
}
