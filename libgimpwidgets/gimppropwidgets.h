/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimppropwidgets.h
 * Copyright (C) 2002 Michael Natterer <mitch@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GIMP_PROP_WIDGETS_H__
#define __GIMP_PROP_WIDGETS_H__


/*  GParamBoolean  */

GtkWidget     * gimp_prop_check_button_new        (GObject      *config,
                                                   const gchar  *property_name,
                                                   const gchar  *label);
GtkWidget     * gimp_prop_boolean_combo_box_new   (GObject      *config,
                                                   const gchar  *property_name,
                                                   const gchar  *true_text,
                                                   const gchar  *false_text);
GtkWidget     * gimp_prop_boolean_radio_frame_new (GObject      *config,
                                                   const gchar  *property_name,
                                                   const gchar  *title,
                                                   const gchar  *true_text,
                                                   const gchar  *false_text);

GtkWidget     * gimp_prop_expander_new            (GObject      *config,
                                                   const gchar  *property_name,
                                                   const gchar  *label);


/*  GParamInt  */

GtkWidget     * gimp_prop_int_combo_box_new       (GObject      *config,
                                                   const gchar  *property_name,
                                                   GimpIntStore *store);


/*  GParamEnum  */

GtkWidget     * gimp_prop_enum_combo_box_new      (GObject      *config,
                                                   const gchar  *property_name,
                                                   gint          minimum,
                                                   gint          maximum);

GtkWidget     * gimp_prop_enum_check_button_new   (GObject      *config,
                                                   const gchar  *property_name,
                                                   const gchar  *label,
                                                   gint          false_value,
                                                   gint          true_value);

GtkWidget     * gimp_prop_enum_radio_frame_new    (GObject      *config,
                                                   const gchar  *property_name,
                                                   const gchar  *title,
                                                   gint          minimum,
                                                   gint          maximum);
GtkWidget     * gimp_prop_enum_radio_box_new      (GObject      *config,
                                                   const gchar  *property_name,
                                                   gint          minimum,
                                                   gint          maximum);
GtkWidget     * gimp_prop_enum_stock_box_new      (GObject      *config,
                                                   const gchar  *property_name,
                                                   const gchar  *stock_prefix,
                                                   gint          minimum,
                                                   gint          maximum);


/*  GParamInt, GParamUInt, GParamLong, GParamULong, GParamDouble  */

GtkWidget     * gimp_prop_spin_button_new         (GObject      *config,
                                                   const gchar  *property_name,
                                                   gdouble       step_increment,
                                                   gdouble       page_increment,
                                                   gint          digits);

GtkObject     * gimp_prop_scale_entry_new         (GObject      *config,
                                                   const gchar  *property_name,
                                                   GtkTable     *table,
                                                   gint          column,
                                                   gint          row,
                                                   const gchar  *label,
                                                   gdouble       step_increment,
                                                   gdouble       page_increment,
                                                   gint          digits,
                                                   gboolean      restrict_scale,
                                                   gdouble       restricted_lower,
                                                   gdouble       restricted_upper);

/*  special form of gimp_prop_scale_entry_new() for GParamDouble  */

GtkObject     * gimp_prop_opacity_entry_new       (GObject       *config,
                                                   const gchar   *property_name,
                                                   GtkTable      *table,
                                                   gint           column,
                                                   gint           row,
                                                   const gchar   *label);


/*  GimpParamMemsize  */

GtkWidget     * gimp_prop_memsize_entry_new       (GObject       *config,
                                                   const gchar   *property_name);


/*  GParamString  */

GtkWidget     * gimp_prop_label_new               (GObject       *config,
                                                   const gchar   *property_name);
GtkWidget     * gimp_prop_entry_new               (GObject       *config,
                                                   const gchar   *property_name,
                                                   gint           max_len);
GtkTextBuffer * gimp_prop_text_buffer_new         (GObject       *config,
                                                   const gchar   *property_name,
                                                   gint           max_len);


/*  GimpParamPath  */

GtkWidget     * gimp_prop_file_entry_new          (GObject       *config,
                                                   const gchar   *property_name,
                                                   const gchar   *filesel_title,
                                                   gboolean       dir_only,
                                                   gboolean       check_valid);
GtkWidget     * gimp_prop_path_editor_new         (GObject       *config,
                                                   const gchar   *path_property_name,
                                                   const gchar   *writable_property_name,
                                                   const gchar   *filesel_title);

GtkWidget     * gimp_prop_file_chooser_button_new (GObject              *config,
                                                   const gchar          *property_name,
                                                   const gchar          *title,
                                                   GtkFileChooserAction  action);


/*  GParamInt, GParamUInt, GParamDouble   unit: GimpParamUnit  */

GtkWidget     * gimp_prop_size_entry_new          (GObject       *config,
                                                   const gchar   *property_name,
                                                   const gchar   *unit_property_name,
                                                   const gchar   *unit_format,
                                                   GimpSizeEntryUpdatePolicy  update_policy,
                                                   gdouble        resolution);


/*  x,y: GParamInt, GParamDouble   unit: GimpParamUnit  */

GtkWidget     * gimp_prop_coordinates_new         (GObject       *config,
                                                   const gchar   *x_property_name,
                                                   const gchar   *y_property_name,
                                                   const gchar   *unit_property_name,
                                                   const gchar   *unit_format,
                                                   GimpSizeEntryUpdatePolicy  update_policy,
                                                   gdouble        xresolution,
                                                   gdouble        yresolution,
                                                   gboolean       has_chainbutton);
gboolean        gimp_prop_coordinates_connect     (GObject       *config,
                                                   const gchar   *x_property_name,
                                                   const gchar   *y_property_name,
                                                   const gchar   *unit_property_name,
                                                   GtkWidget     *sizeentry,
                                                   GtkWidget     *chainbutton,
                                                   gdouble        xresolution,
                                                   gdouble        yresolution);


/*  GimpParamColor  */

GtkWidget     * gimp_prop_color_area_new          (GObject       *config,
                                                   const gchar   *property_name,
                                                   gint           width,
                                                   gint           height,
                                                   GimpColorAreaType  type);

/*  GimpParamUnit  */

GtkWidget     * gimp_prop_unit_menu_new           (GObject       *config,
                                                   const gchar   *property_name,
                                                   const gchar   *unit_format);


/*  GParamString (stock_id)  */

GtkWidget     * gimp_prop_stock_image_new         (GObject       *config,
                                                   const gchar   *property_name,
                                                   GtkIconSize    icon_size);


#endif /* __GIMP_PROP_WIDGETS_H__ */
