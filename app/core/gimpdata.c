/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpdata.c
 * Copyright (C) 2001 Michael Natterer <mitch@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"

#include "core-types.h"

#include "gimpdata.h"
#include "gimpmarshal.h"

#include "libgimp/gimpintl.h"


enum
{
  DIRTY,
  LAST_SIGNAL
};


static void    gimp_data_class_init   (GimpDataClass *klass);
static void    gimp_data_init         (GimpData      *data);

static void    gimp_data_finalize     (GObject       *object);

static void    gimp_data_name_changed (GimpObject    *object);
static gsize   gimp_data_get_memsize  (GimpObject    *object);

static void    gimp_data_real_dirty   (GimpData      *data);


static guint data_signals[LAST_SIGNAL] = { 0 };

static GimpViewableClass *parent_class = NULL;


GType
gimp_data_get_type (void)
{
  static GType data_type = 0;

  if (! data_type)
    {
      static const GTypeInfo data_info =
      {
        sizeof (GimpDataClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_data_class_init,
	NULL,		/* class_finalize */
	NULL,		/* class_data     */
	sizeof (GimpData),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_data_init,
      };

      data_type = g_type_register_static (GIMP_TYPE_VIEWABLE,
					  "GimpData",
					  &data_info, 0);
  }

  return data_type;
}

static void
gimp_data_class_init (GimpDataClass *klass)
{
  GObjectClass    *object_class;
  GimpObjectClass *gimp_object_class;

  object_class      = G_OBJECT_CLASS (klass);
  gimp_object_class = GIMP_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  data_signals[DIRTY] = 
    g_signal_new ("dirty",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpDataClass, dirty),
		  NULL, NULL,
		  gimp_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  object_class->finalize          = gimp_data_finalize;

  gimp_object_class->name_changed = gimp_data_name_changed;
  gimp_object_class->get_memsize  = gimp_data_get_memsize;

  klass->dirty                    = gimp_data_real_dirty;
  klass->save                     = NULL;
  klass->get_extension            = NULL;
  klass->duplicate                = NULL;
}

static void
gimp_data_init (GimpData *data)
{
  data->filename = NULL;
  data->dirty    = FALSE;
  data->internal = FALSE;
}

static void
gimp_data_finalize (GObject *object)
{
  GimpData *data;

  data = GIMP_DATA (object);

  if (data->filename)
    {
      g_free (data->filename);
      data->filename = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_data_name_changed (GimpObject *object)
{
  if (GIMP_OBJECT_CLASS (parent_class)->name_changed)
    GIMP_OBJECT_CLASS (parent_class)->name_changed (object);

  gimp_data_dirty (GIMP_DATA (object));
}

static gsize
gimp_data_get_memsize (GimpObject *object)
{
  GimpData *data;
  gsize     memsize = 0;

  data = GIMP_DATA (object);

  if (data->filename)
    memsize += strlen (data->filename) + 1;

  return memsize + GIMP_OBJECT_CLASS (parent_class)->get_memsize (object);
}

gboolean
gimp_data_save (GimpData  *data,
                GError   **error)
{
  gboolean success = FALSE;

  g_return_val_if_fail (GIMP_IS_DATA (data), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  if (data->internal)
    {
      data->dirty = FALSE;
      return TRUE;
    }

  g_return_val_if_fail (data->filename != NULL, FALSE);

  if (GIMP_DATA_GET_CLASS (data)->save)
    success = GIMP_DATA_GET_CLASS (data)->save (data, error);

  if (success)
    data->dirty = FALSE;

  return success;
}

void
gimp_data_dirty (GimpData *data)
{
  g_return_if_fail (GIMP_IS_DATA (data));

  g_signal_emit (G_OBJECT (data), data_signals[DIRTY], 0);
}

static void
gimp_data_real_dirty (GimpData *data)
{
  data->dirty = TRUE;

  gimp_viewable_invalidate_preview (GIMP_VIEWABLE (data));
}

gboolean
gimp_data_delete_from_disk (GimpData  *data,
                            GError   **error)
{
  g_return_val_if_fail (GIMP_IS_DATA (data), FALSE);
  g_return_val_if_fail (data->filename != NULL, FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  if (data->internal)
    return TRUE;

  if (unlink (data->filename) == -1)
    {
      g_set_error (error, GIMP_DATA_ERROR, GIMP_DATA_ERROR_DELETE,
                   _("Could not delete '%s': %s"),
                   data->filename, g_strerror (errno));
      return FALSE;
    }

  return TRUE;
}

const gchar *
gimp_data_get_extension (GimpData *data)
{
  g_return_val_if_fail (GIMP_IS_DATA (data), NULL);

  if (GIMP_DATA_GET_CLASS (data)->get_extension)
    return GIMP_DATA_GET_CLASS (data)->get_extension (data);

  return NULL;
}

void
gimp_data_set_filename (GimpData    *data,
			const gchar *filename)
{
  g_return_if_fail (GIMP_IS_DATA (data));

  if (data->internal)
    return;

  g_free (data->filename);

  data->filename = g_strdup (filename);
}

void
gimp_data_create_filename (GimpData    *data,
			   const gchar *basename,
			   const gchar *data_path)
{
  GList *path;
  gchar *dir;
  gchar *filename;
  gchar *fullpath;
  gchar *safe_name;
  gint   i;
  gint   unum = 1;

  g_return_if_fail (GIMP_IS_DATA (data));
  g_return_if_fail (basename != NULL);
  g_return_if_fail (data_path != NULL);

  path = gimp_path_parse (data_path, 16, TRUE, NULL);
  dir  = gimp_path_get_user_writable_dir (path);
  gimp_path_free (path);

  if (! dir)
    return;

  safe_name = g_strdup (basename);
  if (safe_name[0] == '.')
    safe_name[0] = '_';
  for (i = 0; safe_name[i]; i++)
    if (safe_name[i] == G_DIR_SEPARATOR || g_ascii_isspace (safe_name[i]))
      safe_name[i] = '_';

  filename = g_strdup_printf ("%s%s",
			      safe_name,
			      gimp_data_get_extension (data));

  fullpath = g_build_filename (dir, filename, NULL);

  g_free (filename);

  while (g_file_test (fullpath, G_FILE_TEST_EXISTS))
    {
      g_free (fullpath);

      filename = g_strdup_printf ("%s-%d%s",
				  safe_name,
                                  unum++,
				  gimp_data_get_extension (data));

      fullpath = g_build_filename (dir, filename, NULL);

      g_free (filename);
    }

  g_free (dir);
  g_free (safe_name);

  gimp_data_set_filename (data, fullpath);

  g_free (fullpath);
}

GimpData *
gimp_data_duplicate (GimpData *data,
                     gboolean  stingy_memory_use)
{
  g_return_val_if_fail (GIMP_IS_DATA (data), NULL);

  if (GIMP_DATA_GET_CLASS (data)->duplicate)
    return GIMP_DATA_GET_CLASS (data)->duplicate (data, stingy_memory_use);

  return NULL;
}

GQuark
gimp_data_error_quark (void)
{
  static GQuark q = 0;
  if (q == 0)
    q = g_quark_from_static_string ("gimp-data-error-quark");

  return q;
}
