/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#include <string.h>

#include <gtk/gtk.h>

#include "libgimpcolor/gimpcolor.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "gui-types.h"

#include "core/gimpimage.h"

#include "widgets/gimpcolormapeditor.h"

#include "color-notebook.h"
#include "colormap-editor-commands.h"

#include "libgimp/gimpintl.h"


/*  local function prototypes  */

static void   colormap_editor_color_notebook_callback (ColorNotebook      *color_notebook,
                                                       const GimpRGB      *color,
                                                       ColorNotebookState  state,
                                                       gpointer            data);


/*  public functions  */

void
colormap_editor_add_color_cmd_callback (GtkWidget *widget,
                                        gpointer   data,
                                        guint      action)
{
  GimpColormapEditor *editor;
  GimpImage          *gimage;

  editor = GIMP_COLORMAP_EDITOR (data);

  gimage = editor->gimage;

  if (! gimage)
    return;

  memcpy (&gimage->cmap[gimage->num_cols * 3],
	  &gimage->cmap[editor->col_index * 3],
	  3);
  gimage->num_cols++;

  gimp_image_colormap_changed (gimage, -1);
}

void
colormap_editor_edit_color_cmd_callback (GtkWidget *widget,
                                         gpointer   data,
                                         guint      action)
{
  GimpColormapEditor *editor;
  GimpImage          *gimage;
  GimpRGB             color;

  editor = GIMP_COLORMAP_EDITOR (data);

  gimage = editor->gimage;

  if (! gimage)
    return;

  gimp_rgba_set_uchar (&color,
                       gimage->cmap[editor->col_index * 3],
		       gimage->cmap[editor->col_index * 3 + 1],
		       gimage->cmap[editor->col_index * 3 + 2],
		       OPAQUE_OPACITY);

  if (! editor->color_notebook)
    {
      editor->color_notebook =
	color_notebook_viewable_new (GIMP_VIEWABLE (gimage),
                                     _("Edit Indexed Color"),
                                     GIMP_STOCK_CONVERT_INDEXED,
                                     _("Edit Indexed Image Palette Color"),
                                     (const GimpRGB *) &color,
                                     colormap_editor_color_notebook_callback,
                                     editor,
                                     FALSE, FALSE);
    }
  else
    {
      color_notebook_set_viewable (editor->color_notebook,
                                   GIMP_VIEWABLE (gimage));
      color_notebook_show (editor->color_notebook);
      color_notebook_set_color (editor->color_notebook, &color);
    }
}


/*  private functions  */

static void
colormap_editor_color_notebook_callback (ColorNotebook      *color_notebook,
                                         const GimpRGB      *color,
                                         ColorNotebookState  state,
                                         gpointer            data)
{
  GimpColormapEditor *editor;
  GimpImage          *gimage;

  editor = GIMP_COLORMAP_EDITOR (data);

  gimage = editor->gimage;

  switch (state)
    {
    case COLOR_NOTEBOOK_UPDATE:
      break;
    case COLOR_NOTEBOOK_OK:
      gimp_rgb_get_uchar (color,
			  &gimage->cmap[editor->col_index * 3 + 0],
			  &gimage->cmap[editor->col_index * 3 + 1],
			  &gimage->cmap[editor->col_index * 3 + 2]);

      gimp_image_colormap_changed (gimage, editor->col_index);
      gimp_image_flush (gimage);
      /* Fall through */
    case COLOR_NOTEBOOK_CANCEL:
      color_notebook_hide (editor->color_notebook);
    }
}
