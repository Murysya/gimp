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

#include <glib-object.h>

#include "libgimpmath/gimpmath.h"

#include "paint-types.h"

#include "base/gimplut.h"
#include "base/pixel-region.h"
#include "base/temp-buf.h"

#include "paint-funcs/paint-funcs.h"

#include "core/gimp.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"

#include "gimpdodgeburn.h"
#include "gimpdodgeburnoptions.h"

#include "gimp-intl.h"


static void     gimp_dodge_burn_class_init (GimpDodgeBurnClass *klass);
static void     gimp_dodge_burn_init       (GimpDodgeBurn      *dodgeburn);

static void     gimp_dodge_burn_finalize   (GObject            *object);

static void     gimp_dodge_burn_paint      (GimpPaintCore      *paint_core,
                                            GimpDrawable       *drawable,
                                            GimpPaintOptions   *paint_options,
                                            GimpPaintCoreState  paint_state);
static void     gimp_dodge_burn_motion     (GimpPaintCore      *paint_core,
                                            GimpDrawable       *drawable,
                                            GimpPaintOptions   *paint_options);

static void     gimp_dodge_burn_make_luts  (GimpDodgeBurn      *dodgeburn,
                                            gdouble             db_exposure,
                                            GimpDodgeBurnType   type,
                                            GimpTransferMode    mode,
                                            GimpDrawable       *drawable);

static gfloat   gimp_dodge_burn_highlights_lut_func (gpointer   user_data,
                                                     gint       nchannels,
                                                     gint       channel,
                                                     gfloat     value);
static gfloat   gimp_dodge_burn_midtones_lut_func   (gpointer   user_data,
                                                     gint       nchannels,
                                                     gint       channel,
                                                     gfloat     value);
static gfloat   gimp_dodge_burn_shadows_lut_func    (gpointer   user_data,
                                                     gint       nchannels,
                                                     gint       channel,
                                                     gfloat     value);


static GimpPaintCoreClass *parent_class = NULL;


void
gimp_dodge_burn_register (Gimp                      *gimp,
                          GimpPaintRegisterCallback  callback)
{
  (* callback) (gimp,
                GIMP_TYPE_DODGE_BURN,
                GIMP_TYPE_DODGE_BURN_OPTIONS,
                _("Dodge/Burn"));
}

GType
gimp_dodge_burn_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo info =
      {
        sizeof (GimpDodgeBurnClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_dodge_burn_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpDodgeBurn),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_dodge_burn_init,
      };

      type = g_type_register_static (GIMP_TYPE_PAINT_CORE,
                                     "GimpDodgeBurn",
                                     &info, 0);
    }

  return type;
}

static void
gimp_dodge_burn_class_init (GimpDodgeBurnClass *klass)
{
  GObjectClass       *object_class;
  GimpPaintCoreClass *paint_core_class;

  object_class     = G_OBJECT_CLASS (klass);
  paint_core_class = GIMP_PAINT_CORE_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize  = gimp_dodge_burn_finalize;

  paint_core_class->paint = gimp_dodge_burn_paint;
}

static void
gimp_dodge_burn_init (GimpDodgeBurn *dodgeburn)
{
  GimpPaintCore *paint_core;

  paint_core = GIMP_PAINT_CORE (dodgeburn);

  paint_core->flags |= CORE_HANDLES_CHANGING_BRUSH;
}

static void
gimp_dodge_burn_finalize (GObject *object)
{
  GimpDodgeBurn *dodgeburn;

  dodgeburn = GIMP_DODGE_BURN (object);

  if (dodgeburn->lut)
    {
      gimp_lut_free (dodgeburn->lut);
      dodgeburn->lut = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_dodge_burn_paint (GimpPaintCore      *paint_core,
                       GimpDrawable       *drawable,
                       GimpPaintOptions   *paint_options,
                       GimpPaintCoreState  paint_state)
{
  GimpDodgeBurn        *dodgeburn;
  GimpDodgeBurnOptions *options;

  dodgeburn = GIMP_DODGE_BURN (paint_core);
  options   = GIMP_DODGE_BURN_OPTIONS (paint_options);

  switch (paint_state)
    {
    case INIT_PAINT:
      dodgeburn->lut = gimp_lut_new ();

      gimp_dodge_burn_make_luts (dodgeburn,
                                 options->exposure,
                                 options->type,
                                 options->mode,
                                 drawable);
      break;

    case MOTION_PAINT:
      gimp_dodge_burn_motion (paint_core, drawable, paint_options);
      break;

    case FINISH_PAINT:
      if (dodgeburn->lut)
	{
	  gimp_lut_free (dodgeburn->lut);
	  dodgeburn->lut = NULL;
	}
      break;

    default:
      break;
    }
}

static void
gimp_dodge_burn_motion (GimpPaintCore    *paint_core,
                        GimpDrawable     *drawable,
                        GimpPaintOptions *paint_options)
{
  GimpDodgeBurn        *dodgeburn;
  GimpDodgeBurnOptions *options;
  GimpContext          *context;
  GimpPressureOptions  *pressure_options;
  GimpImage            *gimage;
  TempBuf              *area;
  TempBuf              *orig;
  PixelRegion           srcPR, destPR, tempPR;
  guchar               *temp_data;
  gdouble               opacity;
  gdouble               scale;

  dodgeburn = GIMP_DODGE_BURN (paint_core);
  options   = GIMP_DODGE_BURN_OPTIONS (paint_options);
  context   = GIMP_CONTEXT (paint_options);

  pressure_options = paint_options->pressure_options;

  if (! (gimage = gimp_item_get_image (GIMP_ITEM (drawable))))
    return;

  /*  If the image type is indexed, don't dodgeburn  */
  if ((gimp_drawable_type (drawable) == GIMP_INDEXED_IMAGE) ||
      (gimp_drawable_type (drawable) == GIMP_INDEXEDA_IMAGE))
    return;

  if (pressure_options->size)
    scale = paint_core->cur_coords.pressure;
  else
    scale = 1.0;

  /*  Get a region which can be used to paint to  */
  if (! (area = gimp_paint_core_get_paint_area (paint_core, drawable, scale)))
    return;

  /* Constant painting --get a copy of the orig drawable (with no
   * paint from this stroke yet)
   */
  {
    gint x1, y1, x2, y2;

    x1 = CLAMP (area->x, 0, gimp_drawable_width (drawable));
    y1 = CLAMP (area->y, 0, gimp_drawable_height (drawable));
    x2 = CLAMP (area->x + area->width,  0, gimp_drawable_width (drawable));
    y2 = CLAMP (area->y + area->height, 0, gimp_drawable_height (drawable));

    if (!(x2 - x1) || !(y2 - y1))
      return;

    /*  get the original untouched image  */
    orig = gimp_paint_core_get_orig_image (paint_core, drawable, x1, y1, x2, y2);

    srcPR.bytes     = orig->bytes;
    srcPR.x         = 0; 
    srcPR.y         = 0;
    srcPR.w         = x2 - x1;
    srcPR.h         = y2 - y1;
    srcPR.rowstride = srcPR.bytes * orig->width;
    srcPR.data      = temp_buf_data (orig);
  }

  /* tempPR will hold the dodgeburned region*/
  tempPR.bytes     = srcPR.bytes;
  tempPR.x         = srcPR.x; 
  tempPR.y         = srcPR.y;
  tempPR.w         = srcPR.w;
  tempPR.h         = srcPR.h;
  tempPR.rowstride = tempPR.bytes * tempPR.w;
  tempPR.data      = g_malloc (tempPR.h * tempPR.rowstride);

  temp_data        = tempPR.data;

  /*  DodgeBurn the region  */
  gimp_lut_process (dodgeburn->lut, &srcPR, &tempPR);

  /* The dest is the paint area we got above (= canvas_buf) */ 
  destPR.bytes     = area->bytes;
  destPR.x         = 0;
  destPR.y         = 0;
  destPR.w         = area->width;
  destPR.h         = area->height;
  destPR.rowstride = area->width * destPR.bytes;
  destPR.data      = temp_buf_data (area);

  /* Now add an alpha to the dodgeburned region 
     and put this in area = canvas_buf */ 
  if (! gimp_drawable_has_alpha (drawable))
    add_alpha_region (&tempPR, &destPR);
  else
    copy_region (&tempPR, &destPR);

  opacity = gimp_context_get_opacity (context);

  if (pressure_options->opacity)
    opacity = opacity * 2.0 * paint_core->cur_coords.pressure;

  /* Replace the newly dodgedburned area (canvas_buf) to the gimage*/   
  gimp_paint_core_replace_canvas (paint_core, drawable, 
			          MIN (opacity, GIMP_OPACITY_OPAQUE),
		                  GIMP_OPACITY_OPAQUE,
			          (pressure_options->pressure ? 
                                   GIMP_BRUSH_PRESSURE : GIMP_BRUSH_SOFT),
			          scale,
                                  GIMP_PAINT_CONSTANT);
 
  g_free (temp_data);
}

static void
gimp_dodge_burn_make_luts (GimpDodgeBurn     *dodgeburn,
                           gdouble            db_exposure,
                           GimpDodgeBurnType  type,
                           GimpTransferMode   mode,
                           GimpDrawable      *drawable)
{
  GimpLutFunc   lut_func;
  gint          nchannels = gimp_drawable_bytes (drawable);
  static gfloat exposure;

  exposure = db_exposure / 100.0;

  /* make the exposure negative if burn for luts*/
  if (type == GIMP_BURN)
    exposure = -exposure;

  switch (mode)
    {
    case GIMP_HIGHLIGHTS:
      lut_func = gimp_dodge_burn_highlights_lut_func; 
      break;
    case GIMP_MIDTONES:
      lut_func = gimp_dodge_burn_midtones_lut_func; 
      break;
    case GIMP_SHADOWS:
      lut_func = gimp_dodge_burn_shadows_lut_func; 
      break;
    default:
      lut_func = NULL; 
      break;
    }

  gimp_lut_setup_exact (dodgeburn->lut,
			lut_func, (gpointer) &exposure,
			nchannels);
}

static gfloat
gimp_dodge_burn_highlights_lut_func (gpointer  user_data, 
                                     gint      nchannels, 
                                     gint      channel, 
                                     gfloat    value)
{
  gfloat *exposure_ptr = (gfloat *) user_data;
  gfloat  exposure     = *exposure_ptr;
  gfloat  factor       = 1.0 + exposure * (.333333);

  if ((nchannels == 2 && channel == 1) ||
      (nchannels == 4 && channel == 3))
    return value;

  return factor * value;
}

static gfloat
gimp_dodge_burn_midtones_lut_func (gpointer  user_data, 
                                   gint      nchannels, 
                                   gint      channel, 
                                   gfloat    value)
{
  gfloat *exposure_ptr = (gfloat *) user_data;
  gfloat  exposure     = *exposure_ptr;
  gfloat  factor;

  if ((nchannels == 2 && channel == 1) ||
      (nchannels == 4 && channel == 3))
    return value;

  if (exposure < 0)
    factor = 1.0 - exposure * (.333333);
  else
    factor = 1 / (1.0 + exposure);

  return pow (value, factor); 
}

static gfloat
gimp_dodge_burn_shadows_lut_func (gpointer  user_data, 
                                  gint      nchannels, 
                                  gint      channel, 
                                  gfloat    value)
{
  gfloat *exposure_ptr = (gfloat *) user_data;
  gfloat  exposure     = *exposure_ptr;
  gfloat  new_value;
  gfloat  factor;

  if ((nchannels == 2 && channel == 1) ||
      (nchannels == 4 && channel == 3))
    return value;

  if (exposure >= 0)
    {
      factor = 0.333333 * exposure;
      new_value =  factor + value - factor * value; 
    }
  else /* exposure < 0 */ 
    {
      factor = -0.333333 * exposure;
      if (value < factor)
	new_value = 0;
      else /*factor <= value <=1*/
	new_value = (value - factor)/(1 - factor);
    }

  return new_value; 
}
