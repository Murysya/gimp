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

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "gui-types.h"

#include "core/gimpcontext.h"
#include "core/gimpdata.h"

#include "widgets/gimpcontainereditor.h"
#include "widgets/gimpcontainerview.h"
#include "widgets/gimpdatafactoryview.h"
#include "widgets/gimpitemfactory.h"

#include "data-commands.h"
#include "palettes-commands.h"
#include "palettes-menu.h"
#include "menus.h"

#include "libgimp/gimpintl.h"


GimpItemFactoryEntry palettes_menu_entries[] =
{
  { { N_("/New Palette"), NULL,
      data_new_data_cmd_callback, 0,
      "<StockItem>", GTK_STOCK_NEW },
    NULL,
    NULL, NULL },
  { { N_("/Duplicate Palette"), NULL,
      data_duplicate_data_cmd_callback, 0,
      "<StockItem>", GIMP_STOCK_DUPLICATE },
    NULL,
    NULL, NULL },
  { { N_("/Edit Palette..."), NULL,
      data_edit_data_cmd_callback, 0,
      "<StockItem>", GIMP_STOCK_EDIT },
    NULL,
    NULL, NULL },
  { { N_("/Delete Palette..."), NULL,
      data_delete_data_cmd_callback, 0,
      "<StockItem>", GTK_STOCK_DELETE },
    NULL,
    NULL, NULL },

  MENU_SEPARATOR ("/---"),

  { { N_("/Refresh Palettes"), NULL,
      data_refresh_data_cmd_callback, 0,
      "<StockItem>", GTK_STOCK_REFRESH },
    NULL,
    NULL, NULL },

  MENU_SEPARATOR ("/---"),

  { { N_("/Import Palette..."), NULL,
      palettes_import_palette_cmd_callback, 0,
      "<StockItem>", GTK_STOCK_CONVERT },
    NULL,
    NULL, NULL },
  { { N_("/Merge Palettes..."), NULL,
      palettes_merge_palettes_cmd_callback, 0 },
    NULL,
    NULL, NULL }
};

gint n_palettes_menu_entries = G_N_ELEMENTS (palettes_menu_entries);


void
palettes_menu_update (GtkItemFactory *factory,
                      gpointer        data)
{
  GimpContainerEditor *editor;
  GimpPalette         *palette;
  gboolean             internal = FALSE;

  editor = GIMP_CONTAINER_EDITOR (data);

  palette = gimp_context_get_palette (editor->view->context);

  if (palette)
    internal = GIMP_DATA (palette)->internal;

#define SET_SENSITIVE(menu,condition) \
        gimp_item_factory_set_sensitive (factory, menu, (condition) != 0)

  SET_SENSITIVE ("/Duplicate Palette",
		 palette && GIMP_DATA_GET_CLASS (palette)->duplicate);
  SET_SENSITIVE ("/Edit Palette...",
		 palette && GIMP_DATA_FACTORY_VIEW (editor)->data_edit_func);
  SET_SENSITIVE ("/Delete Palette...",
		 palette && ! internal);
  SET_SENSITIVE ("/Merge Palettes...",
		 FALSE); /* FIXME palette && GIMP_IS_CONTAINER_LIST_VIEW (editor->view)); */

#undef SET_SENSITIVE
}
