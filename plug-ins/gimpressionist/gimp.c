#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>

#include <libgimp/gimp.h>

#include "ppmtool.h"
#include "gimpressionist.h"

#include "libgimp/stdplugins-intl.h"

static void query(void);
static void gimpressionist_main(void);
static void run(char *, int, GimpParam *, int *, GimpParam **);
void repaint(ppm_t *p, ppm_t *a);

int create_gimpressionist(void);

gboolean img_has_alpha = FALSE;

GimpPlugInInfo PLUG_IN_INFO = {
        NULL,   /* init_proc */
        NULL,   /* quit_proc */
        query,  /* query_proc */
        run     /* run_proc */
}; /* PLUG_IN_INFO */

gimpressionist_vals_t pcvals;

gimpressionist_vals_t defaultpcvals = {
  4,
  0.0,
  60.0,
  0,
  12.0,
  20.0,
  20.0,
  1.0,
  1,
  0.1,
  0.0,
  30.0,
  0,
  0,
  "defaultbrush.pgm",
  "defaultpaper.pgm",
  {0,0,0,1.0},
  1,
  0,
  { { 0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 0 } },
  1,
  0,
  0.0,
  0.0,
  1.0,
  0,
  0,
  0,
  0,
  0,
  20.0,
  1,
  10.0,
  20.0,
  0,
  0.1,

  { { 0.5, 0.5, 50.0, 1.0 } },
  1,
  1.0,
  0,

  10,
  4,

  0, 0.0
};

static GimpDrawable *drawable;

static void
query(void)
{
  static GimpParamDef args[] = {
    { GIMP_PDB_INT32,    "run_mode",  "Interactive" },
    { GIMP_PDB_IMAGE,    "image",     "Input image" },
    { GIMP_PDB_DRAWABLE, "drawable",  "Input drawable" },
  }; /* args */

  static GimpParamDef *return_vals = NULL;
  static int        nreturn_vals = 0;

  gimp_install_procedure (PLUG_IN_NAME,
                          "Performs various artistic operations on an image",
                          "Performs various artistic operations on an image",
                          "Vidar Madsen <vidar@prosalg.no>",
                          "Vidar Madsen",
                          PLUG_IN_VERSION,
                          N_("<Image>/Filters/Artistic/GIMPressionist..."),
                          "RGB*, GRAY*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args),
                          nreturn_vals,
                          args,
                          return_vals);
} /* query */

static void
gimpressionist_get_data(char *name, void *ptr)
{
  pcvals = defaultpcvals;
  gimp_get_data(name, ptr);
}

static void
run (gchar      *name,
     gint        nparams,
     GimpParam  *param,
     gint       *nreturn_vals,
     GimpParam **return_vals)
{
  static GimpParam values[1];

  GimpRunMode run_mode;
  GimpPDBStatusType  status;

  status = GIMP_PDB_SUCCESS;
  run_mode = param[0].data.d_int32;

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  *nreturn_vals = 1;
  *return_vals = values;

  INIT_I18N ();

  /* Get the active drawable info */

  drawable = gimp_drawable_get(param[2].data.d_drawable);
  img_has_alpha = gimp_drawable_has_alpha(drawable->drawable_id);

  switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
      gimpressionist_get_data(PLUG_IN_NAME, &pcvals);
      if(!create_gimpressionist())
        return;
      break;
    case GIMP_RUN_NONINTERACTIVE:
      g_message("GIMPressionist: GIMP_RUN_NONINTERACTIVE not implemented yet!\n");
      status = GIMP_PDB_EXECUTION_ERROR;
      break;
    case GIMP_RUN_WITH_LAST_VALS:
      gimpressionist_get_data(PLUG_IN_NAME, &pcvals);
      break;
    default:
      g_message("Huh?!\n");
      status = GIMP_PDB_EXECUTION_ERROR;
      break;
    }
  if ((status == GIMP_PDB_SUCCESS) &&
      (gimp_drawable_is_rgb(drawable->drawable_id) ||
       gimp_drawable_is_gray(drawable->drawable_id)))
    {
      gimpressionist_main();
      gimp_displays_flush ();
      
      if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_set_data(PLUG_IN_NAME, &pcvals, sizeof(gimpressionist_vals_t));
      
    }
  else if (status == GIMP_PDB_SUCCESS)
    {
      status = GIMP_PDB_EXECUTION_ERROR;
    }

  values[0].data.d_status = status;
  
  gimp_drawable_detach(drawable);
}

void grabarea(void)
{
  GimpPixelRgn src_rgn;
  guchar *src_row;
  guchar *src;
  gint alpha, has_alpha, bpp;
  gint x, y;
  ppm_t *p;
  gint x1, y1, x2, y2;
  gint row, col;
  int rowstride;

  if(standalone) {
    loadppm(standalone, &infile);
    return;
  }

  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);

  bpp = gimp_drawable_bpp (drawable->drawable_id);
  has_alpha = gimp_drawable_has_alpha (drawable->drawable_id);
  alpha = (has_alpha) ? bpp - 1 : bpp;
  
  newppm(&infile, x2-x1, y2-y1);
  p = &infile;
  if(has_alpha) {
    newppm(&inalpha, x2-x1, y2-y1);
  }

  rowstride = p->width * 3;

  src_row = g_new (guchar, (x2 - x1) * bpp);

  gimp_pixel_rgn_init (&src_rgn, drawable, 0, 0, x2-x1, y2-y1, FALSE, FALSE);

  if(bpp == 3) { /* RGB */
    int bpr = (x2-x1) * 3;
    for(row = 0, y = y1; y < y2; row++, y++) {
      gimp_pixel_rgn_get_row (&src_rgn, src_row, x1, y, (x2 - x1));
      memcpy(p->col + row * rowstride, src_row, bpr);
    }
  } else if(bpp > 3) { /* RGBA */
    for(row = 0, y = y1; y < y2; row++, y++) {
      guchar *tmprow = p->col + row * rowstride;
      guchar *tmparow = inalpha.col + row * rowstride;
      gimp_pixel_rgn_get_row (&src_rgn, src_row, x1, y, (x2 - x1));
      src = src_row;
      for (col = 0, x = x1; x < x2; col++, x++) {
	int k = col * 3;
	tmprow[k+0] = src[0];
	tmprow[k+1] = src[1];
	tmprow[k+2] = src[2];
	tmparow[k] = 255 - src[3];
	src += src_rgn.bpp;
      }
    }
  } else if(bpp == 2) { /* GrayA */
    for(row = 0, y = y1; y < y2; row++, y++) {
      guchar *tmprow = p->col + row * rowstride;
      guchar *tmparow = inalpha.col + row * rowstride;
      gimp_pixel_rgn_get_row (&src_rgn, src_row, x1, y, (x2 - x1));
      src = src_row;
      for (col = 0, x = x1; x < x2; col++, x++) {
	int k = col * 3;
	tmprow[k+0] = src[0];
	tmprow[k+1] = src[0];
	tmprow[k+2] = src[0];
	tmparow[k] = 255 - src[1];
	src += src_rgn.bpp;
      }
    }
  } else { /* Gray */
    for(row = 0, y = y1; y < y2; row++, y++) {
      guchar *tmprow = p->col + row * rowstride;
      gimp_pixel_rgn_get_row (&src_rgn, src_row, x1, y, (x2 - x1));
      src = src_row;
      for (col = 0, x = x1; x < x2; col++, x++) {
	int k = col * 3;
	tmprow[k+0] = src[0];
	tmprow[k+1] = src[0];
	tmprow[k+2] = src[0];
	src += src_rgn.bpp;
      }
    }
  }
  g_free (src_row);
}

static void gimpressionist_main(void)
{
  GimpPixelRgn dest_rgn;
  guchar *dest_row;
  guchar *dest;
  gint alpha, has_alpha, bpp;
  gint x, y;
  ppm_t *p;
  gint x1, y1, x2, y2;
  gint row, col;
  int rowstride;

  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);

  bpp = gimp_drawable_bpp (drawable->drawable_id);
  has_alpha = gimp_drawable_has_alpha (drawable->drawable_id);
  alpha = (has_alpha) ? bpp - 1 : bpp;

  dest_row = g_new (guchar, (x2 - x1) * bpp);

  gimp_progress_init( _("Painting..."));

  if(!infile.col) {
    grabarea();
  }

  repaint(&infile, (img_has_alpha) ? &inalpha : NULL);

  gimp_pixel_rgn_init (&dest_rgn, drawable, 0, 0, x2-x1, y2-y1, TRUE, TRUE);

  p = &infile;

  rowstride = p->width * 3;

  if(bpp == 3) {

    int bpr = (x2 - x1) * 3;

    for(row = 0, y = y1; y < y2; row++, y++) {
      if(row % 10 == 0)
	gimp_progress_update(0.8 + 0.2*((double)row / (y2-y1)));
      memcpy(dest_row, p->col + row * rowstride, bpr);
      gimp_pixel_rgn_set_row (&dest_rgn, dest_row, x1, y, (x2 - x1));
    }

  } else if(bpp == 4) {

    for(row = 0, y = y1; y < y2; row++, y++) {
      guchar *tmprow = p->col + row * rowstride;
      if(row % 10 == 0)
	gimp_progress_update(0.8 + 0.2*((double)row / (y2-y1)));
      dest = dest_row;
      for(col = 0, x = x1; x < x2; col++, x++) {
	int k = col * 3;
	dest[0] = tmprow[k+0];
	dest[1] = tmprow[k+1];
	dest[2] = tmprow[k+2];
	dest[3] = 255 - inalpha.col[row * rowstride + k];
	dest += dest_rgn.bpp;
      }
      gimp_pixel_rgn_set_row (&dest_rgn, dest_row, x1, y, (x2 - x1));
    }

  } else if(bpp == 2) {

    for(row = 0, y = y1; y < y2; row++, y++) {
      guchar *tmprow = p->col + row * rowstride;
      if(row % 10 == 0)
	gimp_progress_update(0.8 + 0.2*((double)row / (y2-y1)));
      dest = dest_row;
      for(col = 0, x = x1; x < x2; col++, x++) {
	int k = col * 3;
	dest[0] = (tmprow[k] + tmprow[k+1] + tmprow[k+2]) / 3;
	dest[1] = 255 - inalpha.col[row*rowstride + k];
	dest += dest_rgn.bpp;
      }
      gimp_pixel_rgn_set_row (&dest_rgn, dest_row, x1, y, (x2 - x1));
    }

  } else {

    for(row = 0, y = y1; y < y2; row++, y++) {
      guchar *tmprow = p->col + row * rowstride;
      if(row % 10 == 0)
	gimp_progress_update(0.8 + 0.2*((double)row / (y2-y1)));
      dest = dest_row;
      for(col = 0, x = x1; x < x2; col++, x++) {
	int k = col * 3;
	dest[0] = (tmprow[k] + tmprow[k+1] + tmprow[k+2]) / 3;
	dest += dest_rgn.bpp;
      }
      gimp_pixel_rgn_set_row (&dest_rgn, dest_row, x1, y, (x2 - x1));
    }
  }

  g_free (dest_row);

  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  gimp_drawable_update (drawable->drawable_id, x1, y1, (x2 - x1), (y2 - y1));
}
