/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2000 Spencer Kimball and Peter Mattis
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

#include "libgimpbase/gimpbasetypes.h"

#include "pdb-types.h"
#include "procedural_db.h"

#include "base/base-enums.h"
#include "core/core-enums.h"
#include "core/gimpdrawable-blend.h"
#include "core/gimpdrawable-bucket-fill.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage-mask.h"
#include "core/gimpimage-pick-color.h"
#include "core/gimpimage.h"

#include "libgimpcolor/gimpcolor.h"
#include "libgimpmath/gimpmath.h"

static ProcRecord blend_proc;
static ProcRecord bucket_fill_proc;
static ProcRecord color_picker_proc;

void
register_misc_tools_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &blend_proc);
  procedural_db_register (gimp, &bucket_fill_proc);
  procedural_db_register (gimp, &color_picker_proc);
}

static Argument *
blend_invoker (Gimp     *gimp,
               Argument *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  gint32 blend_mode;
  gint32 paint_mode;
  gint32 gradient_type;
  gdouble opacity;
  gdouble offset;
  gint32 repeat;
  gboolean supersample;
  gint32 max_depth;
  gdouble threshold;
  gdouble x1;
  gdouble y1;
  gdouble x2;
  gdouble y2;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  blend_mode = args[1].value.pdb_int;
  if (blend_mode < GIMP_FG_BG_RGB_MODE || blend_mode > GIMP_CUSTOM_MODE)
    success = FALSE;

  paint_mode = args[2].value.pdb_int;
  if (paint_mode < GIMP_NORMAL_MODE || paint_mode > GIMP_COLOR_ERASE_MODE)
    success = FALSE;

  gradient_type = args[3].value.pdb_int;
  if (gradient_type < GIMP_LINEAR || gradient_type > GIMP_SPIRAL_ANTICLOCKWISE)
    success = FALSE;

  opacity = args[4].value.pdb_float;
  if (opacity < 0.0 || opacity > 100.0)
    success = FALSE;

  offset = args[5].value.pdb_float;
  if (offset < 0.0)
    success = FALSE;

  repeat = args[6].value.pdb_int;
  if (repeat < GIMP_REPEAT_NONE || repeat > GIMP_REPEAT_TRIANGULAR)
    success = FALSE;

  supersample = args[7].value.pdb_int ? TRUE : FALSE;

  max_depth = args[8].value.pdb_int;
  if (supersample && (max_depth < 1 || max_depth > 9))
    success = FALSE;

  threshold = args[9].value.pdb_float;
  if (supersample && (threshold < 0.0 || threshold > 4.0))
    success = FALSE;

  x1 = args[10].value.pdb_float;

  y1 = args[11].value.pdb_float;

  x2 = args[12].value.pdb_float;

  y2 = args[13].value.pdb_float;

  if (success)
    {
      if (! gimp_item_get_image (GIMP_ITEM (drawable)))
	{
	  success = FALSE;
	}
      else
	{
	  gimp_drawable_blend (drawable,
			       blend_mode,
			       paint_mode,
			       gradient_type,
			       opacity / 100.0,
			       offset, repeat,
			       supersample, max_depth,
			       threshold,
			       x1, y1, x2, y2,
			       NULL, NULL);
	}
    }

  return procedural_db_return_args (&blend_proc, success);
}

static ProcArg blend_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "blend_mode",
    "The type of blend: { GIMP_FG_BG_RGB_MODE (0), GIMP_FG_BG_HSV_MODE (1), GIMP_FG_TRANSPARENT_MODE (2), GIMP_CUSTOM_MODE (3) }"
  },
  {
    GIMP_PDB_INT32,
    "paint_mode",
    "The paint application mode: { GIMP_NORMAL_MODE (0), GIMP_DISSOLVE_MODE (1), GIMP_BEHIND_MODE (2), GIMP_MULTIPLY_MODE (3), GIMP_SCREEN_MODE (4), GIMP_OVERLAY_MODE (5), GIMP_DIFFERENCE_MODE (6), GIMP_ADDITION_MODE (7), GIMP_SUBTRACT_MODE (8), GIMP_DARKEN_ONLY_MODE (9), GIMP_LIGHTEN_ONLY_MODE (10), GIMP_HUE_MODE (11), GIMP_SATURATION_MODE (12), GIMP_COLOR_MODE (13), GIMP_VALUE_MODE (14), GIMP_DIVIDE_MODE (15), GIMP_DODGE_MODE (16), GIMP_BURN_MODE (17), GIMP_HARDLIGHT_MODE (18), GIMP_SOFTLIGHT_MODE (19), GIMP_GRAIN_EXTRACT_MODE (20), GIMP_GRAIN_MERGE_MODE (21), GIMP_COLOR_ERASE_MODE (22) }"
  },
  {
    GIMP_PDB_INT32,
    "gradient_type",
    "The type of gradient: { GIMP_LINEAR (0), GIMP_BILINEAR (1), GIMP_RADIAL (2), GIMP_SQUARE (3), GIMP_CONICAL_SYMMETRIC (4), GIMP_CONICAL_ASYMMETRIC (5), GIMP_SHAPEBURST_ANGULAR (6), GIMP_SHAPEBURST_SPHERICAL (7), GIMP_SHAPEBURST_DIMPLED (8), GIMP_SPIRAL_CLOCKWISE (9), GIMP_SPIRAL_ANTICLOCKWISE (10) }"
  },
  {
    GIMP_PDB_FLOAT,
    "opacity",
    "The opacity of the final blend (0 <= opacity <= 100)"
  },
  {
    GIMP_PDB_FLOAT,
    "offset",
    "Offset relates to the starting and ending coordinates specified for the blend. This parameter is mode dependent (0 <= offset)"
  },
  {
    GIMP_PDB_INT32,
    "repeat",
    "Repeat mode: { GIMP_REPEAT_NONE (0), GIMP_REPEAT_SAWTOOTH (1), GIMP_REPEAT_TRIANGULAR (2) }"
  },
  {
    GIMP_PDB_INT32,
    "supersample",
    "Do adaptive supersampling (TRUE or FALSE)"
  },
  {
    GIMP_PDB_INT32,
    "max_depth",
    "Maximum recursion levels for supersampling"
  },
  {
    GIMP_PDB_FLOAT,
    "threshold",
    "Supersampling threshold"
  },
  {
    GIMP_PDB_FLOAT,
    "x1",
    "The x coordinate of this blend's starting point"
  },
  {
    GIMP_PDB_FLOAT,
    "y1",
    "The y coordinate of this blend's starting point"
  },
  {
    GIMP_PDB_FLOAT,
    "x2",
    "The x coordinate of this blend's ending point"
  },
  {
    GIMP_PDB_FLOAT,
    "y2",
    "The y coordinate of this blend's ending point"
  }
};

static ProcRecord blend_proc =
{
  "gimp_blend",
  "Blend between the starting and ending coordinates with the specified blend mode and gradient type.",
  "This tool requires information on the paint application mode, the blend mode, and the gradient type. It creates the specified variety of blend using the starting and ending coordinates as defined for each gradient type.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  14,
  blend_inargs,
  0,
  NULL,
  { { blend_invoker } }
};

static Argument *
bucket_fill_invoker (Gimp     *gimp,
                     Argument *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  gint32 fill_mode;
  gint32 paint_mode;
  gdouble opacity;
  gdouble threshold;
  gboolean sample_merged;
  gdouble x;
  gdouble y;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_DRAWABLE (drawable))
    success = FALSE;

  fill_mode = args[1].value.pdb_int;
  if (fill_mode < GIMP_FG_BUCKET_FILL || fill_mode > GIMP_PATTERN_BUCKET_FILL)
    success = FALSE;

  paint_mode = args[2].value.pdb_int;
  if (paint_mode < GIMP_NORMAL_MODE || paint_mode > GIMP_COLOR_ERASE_MODE)
    success = FALSE;

  opacity = args[3].value.pdb_float;
  if (opacity < 0.0 || opacity > 100.0)
    success = FALSE;

  threshold = args[4].value.pdb_float;
  if (threshold < 0.0 || threshold > 255.0)
    success = FALSE;

  sample_merged = args[5].value.pdb_int ? TRUE : FALSE;

  x = args[6].value.pdb_float;

  y = args[7].value.pdb_float;

  if (success)
    {
      if (! gimp_item_get_image (GIMP_ITEM (drawable)))
	{
	  success = FALSE;
	}
      else
	{
	  gboolean do_seed_fill;
    
	  do_seed_fill = gimp_image_mask_is_empty (gimp_item_get_image (GIMP_ITEM (drawable)));
    
	  gimp_drawable_bucket_fill (drawable, fill_mode,
				     paint_mode, opacity / 100.0,
				     do_seed_fill,
				     FALSE /* don't fill transparent */,
				     threshold, sample_merged, x, y);
	}
    }

  return procedural_db_return_args (&bucket_fill_proc, success);
}

static ProcArg bucket_fill_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_INT32,
    "fill_mode",
    "The type of fill: { GIMP_FG_BUCKET_FILL (0), GIMP_BG_BUCKET_FILL (1), GIMP_PATTERN_BUCKET_FILL (2) }"
  },
  {
    GIMP_PDB_INT32,
    "paint_mode",
    "The paint application mode: { GIMP_NORMAL_MODE (0), GIMP_DISSOLVE_MODE (1), GIMP_BEHIND_MODE (2), GIMP_MULTIPLY_MODE (3), GIMP_SCREEN_MODE (4), GIMP_OVERLAY_MODE (5), GIMP_DIFFERENCE_MODE (6), GIMP_ADDITION_MODE (7), GIMP_SUBTRACT_MODE (8), GIMP_DARKEN_ONLY_MODE (9), GIMP_LIGHTEN_ONLY_MODE (10), GIMP_HUE_MODE (11), GIMP_SATURATION_MODE (12), GIMP_COLOR_MODE (13), GIMP_VALUE_MODE (14), GIMP_DIVIDE_MODE (15), GIMP_DODGE_MODE (16), GIMP_BURN_MODE (17), GIMP_HARDLIGHT_MODE (18), GIMP_SOFTLIGHT_MODE (19), GIMP_GRAIN_EXTRACT_MODE (20), GIMP_GRAIN_MERGE_MODE (21), GIMP_COLOR_ERASE_MODE (22) }"
  },
  {
    GIMP_PDB_FLOAT,
    "opacity",
    "The opacity of the final bucket fill (0 <= opacity <= 100)"
  },
  {
    GIMP_PDB_FLOAT,
    "threshold",
    "The threshold determines how extensive the seed fill will be. It's value is specified in terms of intensity levels (0 <= threshold <= 255). This parameter is only valid when there is no selection in the specified image."
  },
  {
    GIMP_PDB_INT32,
    "sample_merged",
    "Use the composite image, not the drawable"
  },
  {
    GIMP_PDB_FLOAT,
    "x",
    "The x coordinate of this bucket fill's application. This parameter is only valid when there is no selection in the specified image."
  },
  {
    GIMP_PDB_FLOAT,
    "y",
    "The y coordinate of this bucket fill's application. This parameter is only valid when there is no selection in the specified image."
  }
};

static ProcRecord bucket_fill_proc =
{
  "gimp_bucket_fill",
  "Fill the area specified either by the current selection if there is one, or by a seed fill starting at the specified coordinates.",
  "This tool requires information on the paint application mode, and the fill mode, which can either be in the foreground color, or in the currently active pattern. If there is no selection, a seed fill is executed at the specified coordinates and extends outward in keeping with the threshold parameter. If there is a selection in the target image, the threshold, sample merged, x, and y arguments are unused. If the sample_merged parameter is non-zero, the data of the composite image will be used instead of that for the specified drawable. This is equivalent to sampling for colors after merging all visible layers. In the case of merged sampling, the x,y coordinates are relative to the image's origin; otherwise, they are relative to the drawable's origin.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  8,
  bucket_fill_inargs,
  0,
  NULL,
  { { bucket_fill_invoker } }
};

static Argument *
color_picker_invoker (Gimp     *gimp,
                      Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  GimpDrawable *drawable;
  gdouble x;
  gdouble y;
  gboolean sample_merged;
  gboolean sample_average;
  gdouble average_radius;
  GimpRGB color;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[1].value.pdb_int);

  x = args[2].value.pdb_float;

  y = args[3].value.pdb_float;

  sample_merged = args[4].value.pdb_int ? TRUE : FALSE;

  sample_average = args[5].value.pdb_int ? TRUE : FALSE;

  average_radius = args[6].value.pdb_float;
  if (sample_average && (average_radius <= 0.0))
    success = FALSE;

  if (success)
    {
      if (!sample_merged)
	if (!drawable || (gimp_item_get_image (GIMP_ITEM (drawable)) != gimage))
	  success = FALSE;
    
      if (success)
	success = gimp_image_pick_color (gimage,
					 drawable,
					 sample_merged,
					 (gint) x, (gint) y,
					 sample_average,
					 average_radius,
					 &color,
					 NULL,
					 NULL);
    }

  return_args = procedural_db_return_args (&color_picker_proc, success);

  if (success)
    return_args[1].value.pdb_color = color;

  return return_args;
}

static ProcArg color_picker_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The affected drawable"
  },
  {
    GIMP_PDB_FLOAT,
    "x",
    "x coordinate of upper-left corner of rectangle"
  },
  {
    GIMP_PDB_FLOAT,
    "y",
    "y coordinate of upper-left corner of rectangle"
  },
  {
    GIMP_PDB_INT32,
    "sample_merged",
    "Use the composite image, not the drawable"
  },
  {
    GIMP_PDB_INT32,
    "sample_average",
    "Average the color of all the pixels in a specified radius"
  },
  {
    GIMP_PDB_FLOAT,
    "average_radius",
    "The radius of pixels to average"
  }
};

static ProcArg color_picker_outargs[] =
{
  {
    GIMP_PDB_COLOR,
    "color",
    "The return color"
  }
};

static ProcRecord color_picker_proc =
{
  "gimp_color_picker",
  "Determine the color at the given drawable coordinates",
  "This tool determines the color at the specified coordinates. The returned color is an RGB triplet even for grayscale and indexed drawables. If the coordinates lie outside of the extents of the specified drawable, then an error is returned. If the drawable has an alpha channel, the algorithm examines the alpha value of the drawable at the coordinates. If the alpha value is completely transparent (0), then an error is returned. If the sample_merged parameter is non-zero, the data of the composite image will be used instead of that for the specified drawable. This is equivalent to sampling for colors after merging all visible layers. In the case of a merged sampling, the supplied drawable is ignored.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  7,
  color_picker_inargs,
  1,
  color_picker_outargs,
  { { color_picker_invoker } }
};
