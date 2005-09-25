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

#include "tools-types.h"

#include "core/gimpchannel.h"
#include "core/gimpchannel-select.h"
#include "core/gimplayer-floating-sel.h"
#include "core/gimpimage.h"
#include "core/gimppickable.h"
#include "core/gimptoolinfo.h"
#include "core/gimp-utils.h"

#include "widgets/gimpdialogfactory.h"
#include "widgets/gimphelp-ids.h"
#include "widgets/gimpviewabledialog.h"
#include "widgets/gimpwidgets-utils.h"

#include "display/gimpcanvas.h"
#include "display/gimpdisplay.h"
#include "display/gimpdisplayshell.h"
#include "display/gimpdisplayshell-transform.h"

#include "gimpselectiontool.h"
#include "gimpselectionoptions.h"
#include "gimprectangletool.h"
#include "gimprectangleoptions.h"
#include "gimpnewrectselecttool.h"
#include "gimpnewrectselectoptions.h"
#include "gimptoolcontrol.h"

#include "gimp-intl.h"


static void gimp_new_rect_select_tool_class_init     (GimpNewRectSelectToolClass *klass);
static void gimp_new_rect_select_tool_init           (GimpNewRectSelectTool      *rect_tool);
static void gimp_new_rect_select_tool_rectangle_tool_iface_init (GimpRectangleToolInterface *rectangle_iface);

static GObject * gimp_new_rect_select_tool_constructor (GType                       type,
                                                        guint                       n_params,
                                                        GObjectConstructParam      *params);
static void gimp_new_rect_select_tool_finalize       (GObject         *object);
static void gimp_new_rect_select_tool_control        (GimpTool        *tool,
                                                      GimpToolAction   action,
                                                      GimpDisplay     *gdisp);
static void gimp_new_rect_select_tool_oper_update    (GimpTool        *tool,
                                                      GimpCoords      *coords,
                                                      GdkModifierType  state,
                                                      GimpDisplay     *gdisp);
static gboolean gimp_new_rect_select_tool_execute    (GimpRectangleTool          *rect_tool,
                                                      gint                        x,
                                                      gint                        y,
                                                      gint                        w,
                                                      gint                        h);

static GimpSelectionToolClass *parent_class = NULL;


/*  public functions  */

void
gimp_new_rect_select_tool_register (GimpToolRegisterCallback  callback,
                                    gpointer                  data)
{
  (* callback) (GIMP_TYPE_NEW_RECT_SELECT_TOOL,
                GIMP_TYPE_NEW_RECT_SELECT_OPTIONS,
                gimp_new_rect_select_options_gui,
                0,
                "gimp-new-rect-select-tool",
                _("New Rect Select"),
                _("Select a Rectangular part of an image"),
                N_("_New Rect Select"), NULL,
                NULL, GIMP_HELP_TOOL_RECT_SELECT,
                GIMP_STOCK_TOOL_RECT_SELECT,
                data);
}

GType
gimp_new_rect_select_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpNewRectSelectToolClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gimp_new_rect_select_tool_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data     */
        sizeof (GimpNewRectSelectTool),
        0,              /* n_preallocs    */
        (GInstanceInitFunc) gimp_new_rect_select_tool_init,
      };
      static const GInterfaceInfo rectangle_info =
      {
        (GInterfaceInitFunc) gimp_new_rect_select_tool_rectangle_tool_iface_init,           /* interface_init */
        NULL,           /* interface_finalize */
        NULL            /* interface_data */
      };

      tool_type = g_type_register_static (GIMP_TYPE_SELECTION_TOOL,
                                          "GimpNewRectSelectTool",
                                          &tool_info, 0);
      g_type_add_interface_static (tool_type,
                                   GIMP_TYPE_RECTANGLE_TOOL,
                                   &rectangle_info);
    }

  return tool_type;
}

static void
gimp_new_rect_select_tool_class_init (GimpNewRectSelectToolClass *klass)
{
  GObjectClass      *object_class    = G_OBJECT_CLASS (klass);
  GimpToolClass     *tool_class      = GIMP_TOOL_CLASS (klass);
  GimpDrawToolClass *draw_tool_class = GIMP_DRAW_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->constructor  = gimp_new_rect_select_tool_constructor;
  object_class->dispose      = gimp_rectangle_tool_dispose;
  object_class->finalize     = gimp_new_rect_select_tool_finalize;
  object_class->set_property = gimp_rectangle_tool_set_property;
  object_class->get_property = gimp_rectangle_tool_get_property;
  gimp_rectangle_tool_install_properties (object_class);

  tool_class->initialize     = gimp_rectangle_tool_initialize;
  tool_class->control        = gimp_new_rect_select_tool_control;
  tool_class->button_press   = gimp_rectangle_tool_button_press;
  tool_class->button_release = gimp_rectangle_tool_button_release;
  tool_class->motion         = gimp_rectangle_tool_motion;
  tool_class->key_press      = gimp_rectangle_tool_key_press;
  tool_class->modifier_key   = gimp_rectangle_tool_modifier_key;
  tool_class->oper_update    = gimp_new_rect_select_tool_oper_update;
  tool_class->cursor_update  = gimp_rectangle_tool_cursor_update;

  draw_tool_class->draw      = gimp_rectangle_tool_draw;
}

static void
gimp_new_rect_select_tool_init (GimpNewRectSelectTool *new_rect_select_tool)
{
  GimpTool          *tool      = GIMP_TOOL (new_rect_select_tool);

  gimp_tool_control_set_tool_cursor (tool->control, GIMP_TOOL_CURSOR_RECT_SELECT);
}

static void
gimp_new_rect_select_tool_rectangle_tool_iface_init (GimpRectangleToolInterface *rectangle_iface)
{
  rectangle_iface->execute      = gimp_new_rect_select_tool_execute;
}

static GObject *
gimp_new_rect_select_tool_constructor (GType                  type,
                                       guint                  n_params,
                                       GObjectConstructParam *params)
{
  GObject *object;

  object = G_OBJECT_CLASS (parent_class)->constructor (type, n_params, params);

  gimp_rectangle_tool_constructor (object);

  return object;
}

static void
gimp_new_rect_select_tool_finalize (GObject *object)
{
  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_new_rect_select_tool_control (GimpTool       *tool,
                                   GimpToolAction  action,
                                   GimpDisplay    *gdisp)
{
  gimp_rectangle_tool_control (tool, action, gdisp);

  GIMP_TOOL_CLASS (parent_class)->control (tool, action, gdisp);
}

static void
gimp_new_rect_select_tool_oper_update (GimpTool        *tool,
                                       GimpCoords      *coords,
                                       GdkModifierType  state,
                                       GimpDisplay     *gdisp)
{
  gimp_rectangle_tool_oper_update (tool, coords, state, gdisp);

  GIMP_TOOL_CLASS (parent_class)->oper_update (tool, coords, state, gdisp);
}

/*
 * This function is called if the user clicks and releases the left
 * button without moving it.  There are five things we might want
 * to do here:
 * 1) If there is a floating selection, we anchor it.
 * 2) If there is an existing rectangle and we are inside it, we
 *    convert it into a selection.
 * 3) If there is an existing rectangle and we are outside it, we
 *    clear it.
 * 4) If there is no rectangle and we are inside the selection, we
 *    create a rectangle from the selection bounds.
 * 5) If there is no rectangle and we are outside the selection,
 *    we clear the selection.
 */
static gboolean
gimp_new_rect_select_tool_execute (GimpRectangleTool  *rectangle,
                                   gint                x,
                                   gint                y,
                                   gint                w,
                                   gint                h)
{
  GimpTool             *tool     = GIMP_TOOL (rectangle);
  GimpSelectionOptions *options;
  GimpImage            *gimage;
  gint                  max_x, max_y;
  gboolean              rectangle_exists;
  gboolean              selected;
  gint                  val;
  GimpChannel          *selection_mask;
  gint                  x1, y1;
  gint                  x2, y2;

  options = GIMP_SELECTION_OPTIONS (tool->tool_info->tool_options);

  gimage = tool->gdisp->gimage;
  max_x = gimage->width;
  max_y = gimage->height;
  selection_mask = gimp_image_get_mask (gimage);

  rectangle_exists = (x <= max_x && y <= max_y &&
                      x + w >= 0 && y + h >= 0 &&
                      w > 0 && h > 0);

  if (x < 0)
    {
      w += x;
      x = 0;
    }

  if (y < 0)
    {
      h += y;
      y = 0;
    }

  if (x + w > max_x)
    w = max_x - x;

  if (y + h > max_y)
    h = max_y - y;

  /*  If there is a floating selection, anchor it  */
  if (gimp_image_floating_sel (gimage))
    {
      floating_sel_anchor (gimp_image_floating_sel (gimage));
      return FALSE;
    }

  /* if rectangle exists, turn it into a selection */
  if (rectangle_exists)
    {
      GimpChannel  *selection_mask = gimp_image_get_mask (gimage);

      gimp_channel_select_rectangle (selection_mask,
                                     x, y, w, h,
                                     options->operation,
                                     options->feather,
                                     options->feather_radius,
                                     options->feather_radius);
      return TRUE;
    }


  val = gimp_pickable_get_opacity_at (GIMP_PICKABLE (selection_mask),
                                      gimp_rectangle_tool_get_pressx (rectangle), gimp_rectangle_tool_get_pressy (rectangle));

  selected = (val > 127);

  /* if point clicked is inside selection, set rectangle to  */
  /* selection bounds.                                       */
  if (selected)
    {
      if (! gimp_channel_bounds (selection_mask,
                                 &x1, &y1,
                                 &x2, &y2))
        {
          gimp_rectangle_tool_set_x1 (rectangle, x1);
          gimp_rectangle_tool_set_y1 (rectangle, y1);
          gimp_rectangle_tool_set_x2 (rectangle, x2);
          gimp_rectangle_tool_set_y2 (rectangle, y2);
        }
      else
        {
          gimp_rectangle_tool_set_x1 (rectangle, 0);
          gimp_rectangle_tool_set_y1 (rectangle, 0);
          gimp_rectangle_tool_set_x2 (rectangle, gimage->width);
          gimp_rectangle_tool_set_y2 (rectangle, gimage->height);
        }

      return FALSE;
    }

  /* otherwise clear the selection */
  gimp_channel_clear (selection_mask, NULL, TRUE);

  return TRUE;
}

