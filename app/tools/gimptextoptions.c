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

#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"

#include "config/gimpconfig.h"
#include "config/gimpconfig-params.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"
#include "core/gimptoolinfo.h"

#include "text/gimptext.h"

#include "widgets/gimpcolorpanel.h"
#include "widgets/gimpcontainerpopup.h"
#include "widgets/gimpfontselection.h"
#include "widgets/gimppropwidgets.h"
#include "widgets/gimptexteditor.h"
#include "widgets/gimpwidgets-utils.h"

#include "gimptextoptions.h"

#include "gimp-intl.h"


enum
{
  PROP_0,
  PROP_TEXT,
};


static void  gimp_text_options_init       (GimpTextOptions      *options);
static void  gimp_text_options_class_init (GimpTextOptionsClass *options_class);

static void  gimp_text_options_get_property (GObject     *object,
                                             guint        property_id,
                                             GValue      *value,
                                             GParamSpec  *pspec);

static void  gimp_text_options_font_clicked (GtkWidget   *widget, 
                                             GimpContext *context);


static GimpToolOptionsClass *parent_class = NULL;


GType
gimp_text_options_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo info =
      {
        sizeof (GimpTextOptionsClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_text_options_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpTextOptions),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_text_options_init,
      };

      type = g_type_register_static (GIMP_TYPE_TOOL_OPTIONS,
                                     "GimpTextOptions",
                                     &info, 0);
    }

  return type;
}

static void 
gimp_text_options_class_init (GimpTextOptionsClass *klass)
{
  GObjectClass *object_class;

  object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->get_property = gimp_text_options_get_property;

  GIMP_CONFIG_INSTALL_PROP_OBJECT (object_class, PROP_TEXT,
                                   "text", NULL,
                                   GIMP_TYPE_TEXT,
                                   0);
}

static void
gimp_text_options_init (GimpTextOptions *options)
{
  GObject *text;

  text = g_object_new (GIMP_TYPE_TEXT, NULL);

  options->text   = GIMP_TEXT (text);
  options->buffer = gimp_prop_text_buffer_new (text, "text", -1);
}

static void
gimp_text_options_get_property (GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  GimpTextOptions *options;

  options = GIMP_TEXT_OPTIONS (object);

  switch (property_id)
    {
    case PROP_TEXT:
      g_value_set_object (value, options->text);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

GtkWidget *
gimp_text_options_gui (GimpToolOptions *tool_options)
{
  GimpTextOptions *options;
  GObject         *config;
  GtkWidget       *vbox;
  GtkWidget       *table;
  GtkWidget       *button;
  GtkWidget       *preview;
  GtkWidget       *menu;
  GtkWidget       *font_selection;
  GtkWidget       *box;
  GtkWidget       *spinbutton;
  gint             digits;

  options = GIMP_TEXT_OPTIONS (tool_options);
  config = G_OBJECT (options->text);

  vbox = gimp_tool_options_gui (tool_options);

  table = gtk_table_new (7, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 2);
  gtk_table_set_row_spacings (GTK_TABLE (table), 2);
  gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 0);
  gtk_widget_show (table);

  button = gtk_button_new ();
  preview = gimp_prop_preview_new (G_OBJECT (options), "font", 24);
  gtk_container_add (GTK_CONTAINER (button), preview);
  gtk_widget_show (preview);

  gimp_table_attach_aligned (GTK_TABLE (table), 0, 0,
                             _("Font:"), 1.0, 0.5,
                             button, 2, TRUE);

  g_signal_connect (button, "clicked",
                    G_CALLBACK (gimp_text_options_font_clicked),
                    options);

  font_selection = gimp_prop_font_selection_new (config, "font");
  gimp_table_attach_aligned (GTK_TABLE (table), 0, 1,
                             _("_Font:"), 1.0, 0.5,
                             font_selection, 2, FALSE);

  digits = gimp_unit_get_digits (options->text->font_size_unit);
  spinbutton = gimp_prop_spin_button_new (config, "font-size",
					  1.0, 10.0, digits); 
  gimp_table_attach_aligned (GTK_TABLE (table), 0, 2,
                             _("_Size:"), 1.0, 0.5,
                             spinbutton, 1, FALSE);

  menu = gimp_prop_unit_menu_new (config, "font-size-unit", "%a");
  g_object_set_data (G_OBJECT (menu), "set_digits", spinbutton);
  gtk_table_attach (GTK_TABLE (table), menu, 2, 3, 1, 2,
		    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (menu);

  button = gimp_prop_color_button_new (config, "color",
				       _("Text Color"),
				       -1, 24, GIMP_COLOR_AREA_FLAT);
  gimp_color_panel_set_context (GIMP_COLOR_PANEL (button),
                                GIMP_CONTEXT (options));
  gimp_table_attach_aligned (GTK_TABLE (table), 0, 3,
                             _("Color:"), 1.0, 0.5,
			     button, 1, FALSE);

  box = gimp_prop_enum_stock_box_new (config, "justify", "gtk-justify", 0, 0);
  gimp_table_attach_aligned (GTK_TABLE (table), 0, 3,
                             _("Justify:"), 1.0, 0.5,
			     box, 2, TRUE);

  spinbutton = gimp_prop_spin_button_new (config, "indent", 1.0, 10.0, 1);
  gtk_entry_set_width_chars (GTK_ENTRY (spinbutton), 5);
  gimp_table_attach_aligned (GTK_TABLE (table), 0, 4,
                             _("Indent:"), 1.0, 0.5,
                             spinbutton, 1, FALSE);

  spinbutton = gimp_prop_spin_button_new (config, "line-spacing", 1.0, 10.0, 1);
  gtk_entry_set_width_chars (GTK_ENTRY (spinbutton), 5);
  gimp_table_attach_stock (GTK_TABLE (table), 5,
                           _("Line\nSpacing:"), 0.0,
			   spinbutton, 1, GIMP_STOCK_LINE_SPACING);

  return vbox;
}

static void
gimp_text_options_font_clicked (GtkWidget   *widget, 
                                GimpContext *context)
{
  GtkWidget *popup;

  popup = gimp_container_popup_new (context->gimp->fonts, context);

  gimp_container_popup_show (GIMP_CONTAINER_POPUP (popup), widget);
}
