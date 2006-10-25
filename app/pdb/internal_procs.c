/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"

#include <glib-object.h>

#include "pdb-types.h"

#include "gimppdb.h"

/* Forward declarations for registering PDB procs */

void register_brush_procs              (GimpPDB *pdb);
void register_brush_select_procs       (GimpPDB *pdb);
void register_brushes_procs            (GimpPDB *pdb);
void register_buffer_procs             (GimpPDB *pdb);
void register_channel_procs            (GimpPDB *pdb);
void register_color_procs              (GimpPDB *pdb);
void register_context_procs            (GimpPDB *pdb);
void register_convert_procs            (GimpPDB *pdb);
void register_display_procs            (GimpPDB *pdb);
void register_drawable_procs           (GimpPDB *pdb);
void register_drawable_transform_procs (GimpPDB *pdb);
void register_edit_procs               (GimpPDB *pdb);
void register_fileops_procs            (GimpPDB *pdb);
void register_floating_sel_procs       (GimpPDB *pdb);
void register_font_select_procs        (GimpPDB *pdb);
void register_fonts_procs              (GimpPDB *pdb);
void register_gimprc_procs             (GimpPDB *pdb);
void register_gradient_procs           (GimpPDB *pdb);
void register_gradient_select_procs    (GimpPDB *pdb);
void register_gradients_procs          (GimpPDB *pdb);
void register_grid_procs               (GimpPDB *pdb);
void register_guides_procs             (GimpPDB *pdb);
void register_help_procs               (GimpPDB *pdb);
void register_image_procs              (GimpPDB *pdb);
void register_layer_procs              (GimpPDB *pdb);
void register_message_procs            (GimpPDB *pdb);
void register_misc_procs               (GimpPDB *pdb);
void register_paint_tools_procs        (GimpPDB *pdb);
void register_palette_procs            (GimpPDB *pdb);
void register_palette_select_procs     (GimpPDB *pdb);
void register_palettes_procs           (GimpPDB *pdb);
void register_parasite_procs           (GimpPDB *pdb);
void register_paths_procs              (GimpPDB *pdb);
void register_pattern_procs            (GimpPDB *pdb);
void register_pattern_select_procs     (GimpPDB *pdb);
void register_patterns_procs           (GimpPDB *pdb);
void register_plug_in_procs            (GimpPDB *pdb);
void register_procedural_db_procs      (GimpPDB *pdb);
void register_progress_procs           (GimpPDB *pdb);
void register_selection_procs          (GimpPDB *pdb);
void register_selection_tools_procs    (GimpPDB *pdb);
void register_text_tool_procs          (GimpPDB *pdb);
void register_transform_tools_procs    (GimpPDB *pdb);
void register_undo_procs               (GimpPDB *pdb);
void register_unit_procs               (GimpPDB *pdb);
void register_vectors_procs            (GimpPDB *pdb);

/* 535 procedures registered total */

void
internal_procs_init (GimpPDB *pdb)
{
  g_return_if_fail (GIMP_IS_PDB (pdb));

  register_brush_procs (pdb);
  register_brush_select_procs (pdb);
  register_brushes_procs (pdb);
  register_buffer_procs (pdb);
  register_channel_procs (pdb);
  register_color_procs (pdb);
  register_context_procs (pdb);
  register_convert_procs (pdb);
  register_display_procs (pdb);
  register_drawable_procs (pdb);
  register_drawable_transform_procs (pdb);
  register_edit_procs (pdb);
  register_fileops_procs (pdb);
  register_floating_sel_procs (pdb);
  register_font_select_procs (pdb);
  register_fonts_procs (pdb);
  register_gimprc_procs (pdb);
  register_gradient_procs (pdb);
  register_gradient_select_procs (pdb);
  register_gradients_procs (pdb);
  register_grid_procs (pdb);
  register_guides_procs (pdb);
  register_help_procs (pdb);
  register_image_procs (pdb);
  register_layer_procs (pdb);
  register_message_procs (pdb);
  register_misc_procs (pdb);
  register_paint_tools_procs (pdb);
  register_palette_procs (pdb);
  register_palette_select_procs (pdb);
  register_palettes_procs (pdb);
  register_parasite_procs (pdb);
  register_paths_procs (pdb);
  register_pattern_procs (pdb);
  register_pattern_select_procs (pdb);
  register_patterns_procs (pdb);
  register_plug_in_procs (pdb);
  register_procedural_db_procs (pdb);
  register_progress_procs (pdb);
  register_selection_procs (pdb);
  register_selection_tools_procs (pdb);
  register_text_tool_procs (pdb);
  register_transform_tools_procs (pdb);
  register_undo_procs (pdb);
  register_unit_procs (pdb);
  register_vectors_procs (pdb);
}
