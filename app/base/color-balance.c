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

#include "libgimpcolor/gimpcolor.h"
#include "libgimpmath/gimpmath.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"

#include "base/pixel-region.h"

#include "core/gimpdrawable.h"
#include "core/gimpimage.h"

#include "widgets/gimpwidgets-utils.h"

#include "display/gimpdisplay.h"

#include "gimpcolorbalancetool.h"
#include "gimpcolorbalancetool-transfer.h"
#include "tool_manager.h"
#include "tool_options.h"

#include "app_procs.h"
#include "image_map.h"

#include "libgimp/gimpintl.h"


#define CYAN_RED       0x1
#define MAGENTA_GREEN  0x2
#define YELLOW_BLUE    0x4
#define ALL           (CYAN_RED | MAGENTA_GREEN | YELLOW_BLUE)


/*  local function prototypes  */

static void   gimp_color_balance_tool_class_init (GimpColorBalanceToolClass *klass);
static void   gimp_color_balance_tool_init       (GimpColorBalanceTool      *bc_tool);

static void   gimp_color_balance_tool_initialize (GimpTool   *tool,
						  GDisplay   *gdisp);
static void   gimp_color_balance_tool_control    (GimpTool   *tool,
						  ToolAction  action,
						  GDisplay   *gdisp);

static ColorBalanceDialog * color_balance_dialog_new (void);

static void   color_balance_update               (ColorBalanceDialog *cbd,
						  gint                );
static void   color_balance_preview              (ColorBalanceDialog *cbd);
static void   color_balance_reset_callback       (GtkWidget          *widget,
						  gpointer            data);
static void   color_balance_ok_callback          (GtkWidget          *widget,
						  gpointer            data);
static void   color_balance_cancel_callback      (GtkWidget          *widget,
						  gpointer            data);
static void   color_balance_range_callback       (GtkWidget          *widget,
						  gpointer            data);
static void   color_balance_preserve_update      (GtkWidget          *widget,
						  gpointer            data);
static void   color_balance_preview_update       (GtkWidget          *widget,
						  gpointer            data);
static void   color_balance_cr_adjustment_update (GtkAdjustment      *adj,
						  gpointer            data);
static void   color_balance_mg_adjustment_update (GtkAdjustment      *adj,
						  gpointer            data);
static void   color_balance_yb_adjustment_update (GtkAdjustment      *adj,
						  gpointer            data);


/*  the color balance tool options  */
static GimpToolOptions *color_balance_options = NULL;

/*  the color balance dialog  */
static ColorBalanceDialog *color_balance_dialog = NULL;

static GimpImageMapToolClass *parent_class = NULL;


/*  functions  */

void
gimp_color_balance_tool_register (Gimp *gimp)
{
  tool_manager_register_tool (gimp,
			      GIMP_TYPE_COLOR_BALANCE_TOOL,
                              FALSE,
			      "gimp:color_balance_tool",
			      _("Color Balance"),
			      _("Adjust color balance"),
			      N_("/Image/Colors/Color Balance..."), NULL,
			      NULL, "tools/color_balance.html",
			      GIMP_STOCK_TOOL_COLOR_BALANCE);
}

GType
gimp_color_balance_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpColorBalanceToolClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_color_balance_tool_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpColorBalanceTool),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_color_balance_tool_init,
      };

      tool_type = g_type_register_static (GIMP_TYPE_IMAGE_MAP_TOOL,
					  "GimpColorBalanceTool", 
                                          &tool_info, 0);
    }

  return tool_type;
}

static void
gimp_color_balance_tool_class_init (GimpColorBalanceToolClass *klass)
{
  GimpToolClass *tool_class;

  tool_class = GIMP_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  tool_class->initialize = gimp_color_balance_tool_initialize;
  tool_class->control    = gimp_color_balance_tool_control;

  gimp_color_balance_tool_transfer_init ();
}

static void
gimp_color_balance_tool_init (GimpColorBalanceTool *bc_tool)
{
  GimpTool *tool;

  tool = GIMP_TOOL (bc_tool);

  if (! color_balance_options)
    {
      color_balance_options = tool_options_new ();

      tool_manager_register_tool_options (GIMP_TYPE_COLOR_BALANCE_TOOL,
					  (GimpToolOptions *) color_balance_options);
    }
}

static void
gimp_color_balance_tool_initialize (GimpTool *tool,
				    GDisplay *gdisp)
{
  gint i;

  if (! gdisp)
    {
      color_balance_dialog_hide ();
      return;
    }

  if (! gimp_drawable_is_rgb (gimp_image_active_drawable (gdisp->gimage)))
    {
      g_message (_("Color balance operates only on RGB color drawables."));
      return;
    }

  /*  The color balance dialog  */
  if (!color_balance_dialog)
    color_balance_dialog = color_balance_dialog_new ();
  else
    if (!GTK_WIDGET_VISIBLE (color_balance_dialog->shell))
      gtk_widget_show (color_balance_dialog->shell);

  for (i = 0; i < 3; i++)
    {
      color_balance_dialog->cyan_red[i]      = 0.0;
      color_balance_dialog->magenta_green[i] = 0.0;
      color_balance_dialog->yellow_blue[i]   = 0.0;
    }

  color_balance_dialog->drawable = gimp_image_active_drawable (gdisp->gimage);
  color_balance_dialog->image_map =
    image_map_create (gdisp, color_balance_dialog->drawable);

  color_balance_update (color_balance_dialog, ALL);
}

static void
gimp_color_balance_tool_control (GimpTool   *tool,
				 ToolAction  action,
				 GDisplay   *gdisp)
{
  switch (action)
    {
    case PAUSE:
      break;

    case RESUME:
      break;

    case HALT:
      color_balance_dialog_hide ();
      break;

    default:
      break;
    }

  if (GIMP_TOOL_CLASS (parent_class)->control)
    GIMP_TOOL_CLASS (parent_class)->control (tool, action, gdisp);
}


/*  color balance machinery  */

void
color_balance (PixelRegion *srcPR,
	       PixelRegion *destPR,
	       void        *data)
{
  ColorBalanceDialog *cbd;
  guchar             *src, *s;
  guchar             *dest, *d;
  gboolean            alpha;
  gint                r, g, b;
  gint                r_n, g_n, b_n;
  gint                w, h;

  cbd = (ColorBalanceDialog *) data;

  h = srcPR->h;
  src = srcPR->data;
  dest = destPR->data;
  alpha = (srcPR->bytes == 4) ? TRUE : FALSE;

  while (h--)
    {
      w = srcPR->w;
      s = src;
      d = dest;
      while (w--)
	{
	  r = s[RED_PIX];
	  g = s[GREEN_PIX];
	  b = s[BLUE_PIX];

	  r_n = cbd->r_lookup[r];
	  g_n = cbd->g_lookup[g];
	  b_n = cbd->b_lookup[b];

	  if (cbd->preserve_luminosity)
	    {
	      gimp_rgb_to_hls_int (&r_n, &g_n, &b_n);
	      g_n = gimp_rgb_to_l_int (r, g, b);
	      gimp_hls_to_rgb_int (&r_n, &g_n, &b_n);
	    }

	  d[RED_PIX] = r_n;
	  d[GREEN_PIX] = g_n;
 	  d[BLUE_PIX] = b_n;

	  if (alpha)
	    d[ALPHA_PIX] = s[ALPHA_PIX];

	  s += srcPR->bytes;
	  d += destPR->bytes;
	}

      src += srcPR->rowstride;
      dest += destPR->rowstride;
    }
}

void
color_balance_dialog_hide (void)
{
  if (color_balance_dialog)
    color_balance_cancel_callback (NULL, (gpointer) color_balance_dialog);
} 

/**************************/
/*  Color Balance dialog  */
/**************************/

static ColorBalanceDialog *
color_balance_dialog_new (void)
{
  ColorBalanceDialog *cbd;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *table;
  GtkWidget *start_label;
  GtkWidget *end_label;
  GtkWidget *label;
  GtkWidget *spinbutton;
  GtkWidget *slider;
  GtkWidget *toggle;
  GtkWidget *radio_button;
  GtkObject *data;
  GSList *group = NULL;
  gint i;

  gchar *appl_mode_names[] =
  {
    N_("Shadows"),
    N_("Midtones"),
    N_("Highlights")
  };

  cbd = g_new (ColorBalanceDialog, 1);
  cbd->preserve_luminosity = TRUE;
  cbd->preview             = TRUE;
  cbd->application_mode    = GIMP_SHADOWS;

  /*  The shell and main vbox  */
  cbd->shell = gimp_dialog_new (_("Color Balance"), "color_balance",
				tool_manager_help_func, NULL,
				GTK_WIN_POS_NONE,
				FALSE, TRUE, FALSE,

				GTK_STOCK_OK, color_balance_ok_callback,
				cbd, NULL, NULL, TRUE, FALSE,
				GIMP_STOCK_RESET, color_balance_reset_callback,
				cbd, NULL, NULL, TRUE, FALSE,
				GTK_STOCK_CANCEL, color_balance_cancel_callback,
				cbd, NULL, NULL, FALSE, TRUE,

				NULL);

  vbox = gtk_vbox_new (FALSE, 4);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 4);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (cbd->shell)->vbox), vbox);

  /*  Horizontal box for application mode  */
  hbox = gtk_hbox_new (FALSE, 4);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  label = gtk_label_new (_("Color Levels:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  /*  cyan-red spinbutton  */
  data = gtk_adjustment_new (0, -100.0, 100.0, 1.0, 10.0, 0.0);
  cbd->cyan_red_data = GTK_ADJUSTMENT (data);

  spinbutton = gtk_spin_button_new (cbd->cyan_red_data, 1.0, 0);
  gtk_widget_set_usize (spinbutton, 75, -1);
  gtk_box_pack_start (GTK_BOX (hbox), spinbutton, FALSE, FALSE, 0);
  gtk_widget_show (spinbutton);

  /*  magenta-green spinbutton  */
  data = gtk_adjustment_new (0, -100.0, 100.0, 1.0, 10.0, 0.0);
  cbd->magenta_green_data = GTK_ADJUSTMENT (data);

  spinbutton = gtk_spin_button_new (cbd->magenta_green_data, 1.0, 0);
  gtk_widget_set_usize (spinbutton, 75, -1);
  gtk_box_pack_start (GTK_BOX (hbox), spinbutton, FALSE, FALSE, 0);
  gtk_widget_show (spinbutton);

  /*  yellow-blue spinbutton  */
  data = gtk_adjustment_new (0, -100.0, 100.0, 1.0, 10.0, 0.0);
  cbd->yellow_blue_data = GTK_ADJUSTMENT (data);

  spinbutton = gtk_spin_button_new (cbd->yellow_blue_data, 1.0, 0);
  gtk_widget_set_usize (spinbutton, 75, -1);
  gtk_box_pack_start (GTK_BOX (hbox), spinbutton, FALSE, FALSE, 0);
  gtk_widget_show (spinbutton);

  gtk_widget_show (hbox);

  /*  The table containing sliders  */
  table = gtk_table_new (3, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 4);
  gtk_table_set_row_spacings (GTK_TABLE (table), 2);
  gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 0);

  /*  Create the cyan-red scale widget  */
  start_label = gtk_label_new (_("Cyan"));
  gtk_misc_set_alignment (GTK_MISC (start_label), 1.0, 1.0);
  gtk_table_attach (GTK_TABLE (table), start_label, 0, 1, 0, 1,
		    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

  slider = gtk_hscale_new (cbd->cyan_red_data);
  gtk_scale_set_digits (GTK_SCALE (slider), 0);
  gtk_scale_set_value_pos (GTK_SCALE (slider), GTK_POS_TOP);
  gtk_range_set_update_policy (GTK_RANGE (slider), GTK_UPDATE_DELAYED);
  gtk_table_attach_defaults (GTK_TABLE (table), slider, 1, 2, 0, 1);

  g_signal_connect (G_OBJECT (cbd->cyan_red_data), "value_changed",
                    G_CALLBACK (color_balance_cr_adjustment_update),
                    cbd);

  end_label = gtk_label_new (_("Red"));
  gtk_misc_set_alignment (GTK_MISC (end_label), 0.0, 1.0);
  gtk_table_attach (GTK_TABLE (table), end_label, 2, 3, 0, 1,
		    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

  gtk_widget_show (start_label);
  gtk_widget_show (end_label);
  gtk_widget_show (slider);

  /*  Create the magenta-green scale widget  */
  start_label = gtk_label_new (_("Magenta"));
  gtk_misc_set_alignment (GTK_MISC (start_label), 1.0, 1.0);
  gtk_table_attach (GTK_TABLE (table), start_label, 0, 1, 1, 2,
		    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

  slider = gtk_hscale_new (cbd->magenta_green_data);
  gtk_scale_set_digits (GTK_SCALE (slider), 0);
  gtk_scale_set_value_pos (GTK_SCALE (slider), GTK_POS_TOP);
  gtk_range_set_update_policy (GTK_RANGE (slider), GTK_UPDATE_DELAYED);
  gtk_table_attach_defaults (GTK_TABLE (table), slider, 1, 2, 1, 2);

  g_signal_connect (G_OBJECT (cbd->magenta_green_data), "value_changed",
                    G_CALLBACK (color_balance_mg_adjustment_update),
                    cbd);

  end_label = gtk_label_new (_("Green"));
  gtk_misc_set_alignment (GTK_MISC (end_label), 0.0, 1.0);
  gtk_table_attach (GTK_TABLE (table), end_label, 2, 3, 1, 2,
		    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

  gtk_widget_show (start_label);
  gtk_widget_show (end_label);
  gtk_widget_show (slider);

  /*  Create the yellow-blue scale widget  */
  start_label = gtk_label_new (_("Yellow"));
  gtk_misc_set_alignment (GTK_MISC (start_label), 1.0, 1.0);
  gtk_table_attach (GTK_TABLE (table), start_label, 0, 1, 2, 3,
		    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

  slider = gtk_hscale_new (cbd->yellow_blue_data);
  gtk_scale_set_digits (GTK_SCALE (slider), 0);
  gtk_scale_set_value_pos (GTK_SCALE (slider), GTK_POS_TOP);
  gtk_range_set_update_policy (GTK_RANGE (slider), GTK_UPDATE_DELAYED);
  gtk_table_attach_defaults (GTK_TABLE (table), slider, 1, 2, 2, 3);

  g_signal_connect (G_OBJECT (cbd->yellow_blue_data), "value_changed",
                    G_CALLBACK (color_balance_yb_adjustment_update),
                    cbd);

  end_label = gtk_label_new (_("Blue"));
  gtk_misc_set_alignment (GTK_MISC (end_label), 0.0, 1.0);
  gtk_table_attach (GTK_TABLE (table), end_label, 2, 3, 2, 3,
		    GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, 0, 0);

  gtk_widget_show (start_label);
  gtk_widget_show (end_label);
  gtk_widget_show (slider);

  gtk_widget_show (table);

  /*  Horizontal box for application mode  */
  hbox = gtk_hbox_new (TRUE, 4);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  /*  the radio buttons for application mode  */
  for (i = 0; i < 3; i++)
    {
      radio_button =
	gtk_radio_button_new_with_label (group, gettext (appl_mode_names[i]));
      group = gtk_radio_button_group (GTK_RADIO_BUTTON (radio_button));
      gtk_box_pack_start (GTK_BOX (hbox), radio_button, TRUE, FALSE, 0);

      g_object_set_data (G_OBJECT (radio_button), "transfer_mode", 
                         GINT_TO_POINTER (i));
      g_signal_connect (G_OBJECT (radio_button), "toggled",
                        G_CALLBACK (color_balance_range_callback),
                        cbd);

      gtk_widget_show (radio_button);
    }

  gtk_widget_show (hbox);

  /*  Horizontal box for preview and preserve luminosity toggle buttons  */
  hbox = gtk_hbox_new (FALSE, 4);
  gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  /*  The preserve luminosity toggle  */
  toggle = gtk_check_button_new_with_label (_("Preserve Luminosity"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
				cbd->preserve_luminosity);
  gtk_box_pack_start (GTK_BOX (hbox), toggle, FALSE, FALSE, 0);
  g_signal_connect (G_OBJECT (toggle), "toggled",
                    G_CALLBACK (color_balance_preserve_update),
                    cbd);
  gtk_widget_show (toggle);

  /*  The preview toggle  */
  toggle = gtk_check_button_new_with_label (_("Preview"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle), cbd->preview);
  gtk_box_pack_end (GTK_BOX (hbox), toggle, FALSE, FALSE, 0);
  g_signal_connect (G_OBJECT (toggle), "toggled",
                    G_CALLBACK (color_balance_preview_update),
                    cbd);
  gtk_widget_show (toggle);

  gtk_widget_show (hbox);

  gtk_widget_show (vbox);
  gtk_widget_show (cbd->shell);

  return cbd;
}

static void
color_balance_update (ColorBalanceDialog *cbd,
		      gint                update)
{
  if (update & CYAN_RED)
    {
      gtk_adjustment_set_value (cbd->cyan_red_data,
				cbd->cyan_red[cbd->application_mode]);
    }
  if (update & MAGENTA_GREEN)
    {
      gtk_adjustment_set_value (cbd->magenta_green_data,
				cbd->magenta_green[cbd->application_mode]);
    }
  if (update & YELLOW_BLUE)
    {
      gtk_adjustment_set_value (cbd->yellow_blue_data,
				cbd->yellow_blue[cbd->application_mode]);
    }
}

void
color_balance_create_lookup_tables (ColorBalanceDialog *cbd)
{
  gdouble *cyan_red_transfer[3];
  gdouble *magenta_green_transfer[3];
  gdouble *yellow_blue_transfer[3];
  gint i;
  gint32 r_n, g_n, b_n;

  /*  Set the transfer arrays  (for speed)  */
  cyan_red_transfer[GIMP_SHADOWS] = (cbd->cyan_red[GIMP_SHADOWS] > 0) ? shadows_add : shadows_sub;
  cyan_red_transfer[GIMP_MIDTONES] = (cbd->cyan_red[GIMP_MIDTONES] > 0) ? midtones_add : midtones_sub;
  cyan_red_transfer[GIMP_HIGHLIGHTS] = (cbd->cyan_red[GIMP_HIGHLIGHTS] > 0) ? highlights_add : highlights_sub;
  magenta_green_transfer[GIMP_SHADOWS] = (cbd->magenta_green[GIMP_SHADOWS] > 0) ? shadows_add : shadows_sub;
  magenta_green_transfer[GIMP_MIDTONES] = (cbd->magenta_green[GIMP_MIDTONES] > 0) ? midtones_add : midtones_sub;
  magenta_green_transfer[GIMP_HIGHLIGHTS] = (cbd->magenta_green[GIMP_HIGHLIGHTS] > 0) ? highlights_add : highlights_sub;
  yellow_blue_transfer[GIMP_SHADOWS] = (cbd->yellow_blue[GIMP_SHADOWS] > 0) ? shadows_add : shadows_sub;
  yellow_blue_transfer[GIMP_MIDTONES] = (cbd->yellow_blue[GIMP_MIDTONES] > 0) ? midtones_add : midtones_sub;
  yellow_blue_transfer[GIMP_HIGHLIGHTS] = (cbd->yellow_blue[GIMP_HIGHLIGHTS] > 0) ? highlights_add : highlights_sub;

  for (i = 0; i < 256; i++)
    {
      r_n = i;
      g_n = i;
      b_n = i;

      r_n += cbd->cyan_red[GIMP_SHADOWS] * cyan_red_transfer[GIMP_SHADOWS][r_n];
      r_n = CLAMP0255 (r_n);
      r_n += cbd->cyan_red[GIMP_MIDTONES] * cyan_red_transfer[GIMP_MIDTONES][r_n];
      r_n = CLAMP0255 (r_n);
      r_n += cbd->cyan_red[GIMP_HIGHLIGHTS] * cyan_red_transfer[GIMP_HIGHLIGHTS][r_n];
      r_n = CLAMP0255 (r_n);

      g_n += cbd->magenta_green[GIMP_SHADOWS] * magenta_green_transfer[GIMP_SHADOWS][g_n];
      g_n = CLAMP0255 (g_n);
      g_n += cbd->magenta_green[GIMP_MIDTONES] * magenta_green_transfer[GIMP_MIDTONES][g_n];
      g_n = CLAMP0255 (g_n);
      g_n += cbd->magenta_green[GIMP_HIGHLIGHTS] * magenta_green_transfer[GIMP_HIGHLIGHTS][g_n];
      g_n = CLAMP0255 (g_n);

      b_n += cbd->yellow_blue[GIMP_SHADOWS] * yellow_blue_transfer[GIMP_SHADOWS][b_n];
      b_n = CLAMP0255 (b_n);
      b_n += cbd->yellow_blue[GIMP_MIDTONES] * yellow_blue_transfer[GIMP_MIDTONES][b_n];
      b_n = CLAMP0255 (b_n);
      b_n += cbd->yellow_blue[GIMP_HIGHLIGHTS] * yellow_blue_transfer[GIMP_HIGHLIGHTS][b_n];
      b_n = CLAMP0255 (b_n);

      cbd->r_lookup[i] = r_n;
      cbd->g_lookup[i] = g_n;
      cbd->b_lookup[i] = b_n;
    }
}

static void
color_balance_preview (ColorBalanceDialog *cbd)
{
  GimpTool *active_tool;

  active_tool = tool_manager_get_active (the_gimp);

  if (!cbd->image_map)
    {
      g_message ("color_balance_preview(): No image map");
      return;
    }

  active_tool->preserve = TRUE;
  color_balance_create_lookup_tables (cbd);
  image_map_apply (cbd->image_map, color_balance, (void *) cbd);
  active_tool->preserve = FALSE;
}

static void
color_balance_reset_callback (GtkWidget *widget,
			      gpointer   data)
{
  ColorBalanceDialog *cbd;

  cbd = (ColorBalanceDialog *) data;

  cbd->cyan_red[cbd->application_mode]      = 0.0;
  cbd->magenta_green[cbd->application_mode] = 0.0;
  cbd->yellow_blue[cbd->application_mode]   = 0.0;

  color_balance_update (cbd, ALL);

  if (cbd->preview)
    color_balance_preview (cbd);
}

static void
color_balance_ok_callback (GtkWidget *widget,
			   gpointer   data)
{
  ColorBalanceDialog *cbd;
  GimpTool           *active_tool;

  cbd = (ColorBalanceDialog *) data;

  gimp_dialog_hide (cbd->shell);
  
  active_tool = tool_manager_get_active (the_gimp);

  active_tool->preserve = TRUE;

  if (!cbd->preview)
    image_map_apply (cbd->image_map, color_balance, (void *) cbd);

  if (cbd->image_map)
    image_map_commit (cbd->image_map);

  active_tool->preserve = FALSE;

  cbd->image_map = NULL;

  active_tool->gdisp    = NULL;
  active_tool->drawable = NULL;
}

static void
color_balance_cancel_callback (GtkWidget *widget,
			       gpointer   data)
{
  ColorBalanceDialog *cbd;
  GimpTool           *active_tool;

  cbd = (ColorBalanceDialog *) data;

  gimp_dialog_hide (cbd->shell);

  active_tool = tool_manager_get_active (the_gimp);

  if (cbd->image_map)
    {
      active_tool->preserve = TRUE;
      image_map_abort (cbd->image_map);
      active_tool->preserve = FALSE;

      gdisplays_flush ();
      cbd->image_map = NULL;
    }

  active_tool->gdisp    = NULL;
  active_tool->drawable = NULL;
}

static void
color_balance_range_callback (GtkWidget *widget,
			      gpointer   data)
{
  ColorBalanceDialog *cbd;
  GimpTransferMode    range;

  cbd = (ColorBalanceDialog *) data;

  range = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (widget), 
                                              "transfer_mode"));
  cbd->application_mode = range;

  color_balance_update (cbd, ALL);
}

static void
color_balance_preserve_update (GtkWidget *widget,
			       gpointer   data)
{
  ColorBalanceDialog *cbd;

  cbd = (ColorBalanceDialog *) data;

  if (GTK_TOGGLE_BUTTON (widget)->active)
    cbd->preserve_luminosity = TRUE;
  else
    cbd->preserve_luminosity = FALSE;

  if (cbd->preview)
    color_balance_preview (cbd);
}

static void
color_balance_preview_update (GtkWidget *widget,
			      gpointer   data)
{
  ColorBalanceDialog *cbd;
  GimpTool           *active_tool;

  cbd = (ColorBalanceDialog *) data;

  if (GTK_TOGGLE_BUTTON (widget)->active)
    {
      cbd->preview = TRUE;
      color_balance_preview (cbd);
    }
  else
    {
      cbd->preview = FALSE;
      if (cbd->image_map)
	{
	  active_tool = tool_manager_get_active (the_gimp);

	  active_tool->preserve = TRUE;
	  image_map_clear (cbd->image_map);
	  active_tool->preserve = FALSE;
	  gdisplays_flush ();
	}
    }
}

static void
color_balance_cr_adjustment_update (GtkAdjustment *adjustment,
				    gpointer       data)
{
  ColorBalanceDialog *cbd;

  cbd = (ColorBalanceDialog *) data;

  if (cbd->cyan_red[cbd->application_mode] != adjustment->value)
    {
      cbd->cyan_red[cbd->application_mode] = adjustment->value;

      if (cbd->preview)
	color_balance_preview (cbd);
    }
}

static void
color_balance_mg_adjustment_update (GtkAdjustment *adjustment,
				    gpointer       data)
{
  ColorBalanceDialog *cbd;

  cbd = (ColorBalanceDialog *) data;

  if (cbd->magenta_green[cbd->application_mode] != adjustment->value)
    {
      cbd->magenta_green[cbd->application_mode] = adjustment->value;

      if (cbd->preview)
	color_balance_preview (cbd);
    }
}

static void
color_balance_yb_adjustment_update (GtkAdjustment *adjustment,
				    gpointer       data)
{
  ColorBalanceDialog *cbd;

  cbd = (ColorBalanceDialog *) data;

  if (cbd->yellow_blue[cbd->application_mode] != adjustment->value)
    {
      cbd->yellow_blue[cbd->application_mode] = adjustment->value;

      if (cbd->preview)
	color_balance_preview (cbd);
    }
}
