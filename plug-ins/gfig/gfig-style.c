#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <string.h>

#ifdef __GNUC__
#warning GTK_DISABLE_DEPRECATED
#endif
#undef GTK_DISABLE_DEPRECATED

#include <gtk/gtk.h>

#ifdef G_OS_WIN32
#  include <io.h>
#  ifndef W_OK
#    define W_OK 2
#  endif
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include "libgimp/stdplugins-intl.h"

#include "gfig.h"
#include "gfig-style.h"

static void gfig_read_parameter_string (gchar **text,
                                        gint    nitems,
                                        gchar  *name,
                                        gchar  **style_entry);


static void gfig_read_parameter_int    (gchar **text,
                                        gint    nitems,
                                        gchar  *name,
                                        gint   *style_entry);

static void gfig_read_parameter_double (gchar  **text,
                                        gint     nitems,
                                        gchar   *name,
                                        gdouble *style_entry);

static void gfig_read_parameter_gimp_rgb (gchar  **text,
                                          gint     nitems,
                                          gchar   *name,
                                          GimpRGB *style_entry);

static void
gfig_read_parameter_string (gchar **text,
                            gint    nitems,
                            gchar  *name,
                            gchar  **style_entry)
{
  gint  n = 0;
  gchar *ptr;
  gchar *tmpstr;

  *style_entry = NULL;

  while (n < nitems)
    {
      ptr = strchr (text[n], ':');
      if (ptr)
        {
          tmpstr = g_strndup (text[n], ptr - text[n]);
          ptr++;
          if (!strcmp (tmpstr, name))
            {
              *style_entry = g_strdup (g_strchug (ptr));
              g_free (tmpstr);
              return;
            }
          g_free (tmpstr);
        }
      ++n;
    }

  g_message ("Parameter '%s' not found", name);
}


static void
gfig_read_parameter_int (gchar **text,
                         gint    nitems,
                         gchar  *name,
                         gint   *style_entry)
{
  gint  n = 0;
  gchar *ptr;
  gchar *tmpstr;

  *style_entry = 0;

  while (n < nitems)
    {
      ptr = strchr (text[n], ':');
      if (ptr)
        {
          tmpstr = g_strndup (text[n], ptr - text[n]);
          ptr++;
          if (!strcmp (tmpstr, name))
            {
              *style_entry = atoi (g_strchug (ptr));
              g_free (tmpstr);
              return;
            }
          g_free (tmpstr);
        }
      ++n;
    }
}

static void
gfig_read_parameter_double (gchar  **text,
                            gint     nitems,
                            gchar   *name,
                            gdouble *style_entry)
{
  gint  n = 0;
  gchar *ptr;
  gchar *endptr;
  gchar *tmpstr;

  *style_entry = 0.;

  while (n < nitems)
    {
      ptr = strchr (text[n], ':');
      if (ptr)
        {
          tmpstr = g_strndup (text[n], ptr - text[n]);
          ptr++;
          if (!strcmp (tmpstr, name))
            {
              *style_entry = g_ascii_strtod (g_strchug (ptr), &endptr);
              g_free (tmpstr);
              return;
            }
          g_free (tmpstr);
        }
      ++n;
    }
}

static void
gfig_read_parameter_gimp_rgb (gchar  **text,
                              gint     nitems,
                              gchar   *name,
                              GimpRGB *style_entry)
{
  gint  n = 0;
  gchar *ptr;
  gchar *tmpstr;
  gchar *endptr;
  gchar colorstr_r[G_ASCII_DTOSTR_BUF_SIZE];
  gchar colorstr_g[G_ASCII_DTOSTR_BUF_SIZE];
  gchar colorstr_b[G_ASCII_DTOSTR_BUF_SIZE];
  gchar colorstr_a[G_ASCII_DTOSTR_BUF_SIZE];

  style_entry->r = style_entry->g = style_entry->b = style_entry->a = 0.;

  while (n < nitems)
    {
      ptr = strchr (text[n], ':');
      if (ptr)
        {
          tmpstr = g_strndup (text[n], ptr - text[n]);
          ptr++;
          if (!strcmp (tmpstr, name))
            {
              sscanf (ptr, "%s %s %s %s", colorstr_r, colorstr_g, colorstr_b, colorstr_a);
              style_entry->r = g_ascii_strtod (colorstr_r, &endptr);
              style_entry->g = g_ascii_strtod (colorstr_g, &endptr);
              style_entry->b = g_ascii_strtod (colorstr_b, &endptr);
              style_entry->a = g_ascii_strtod (colorstr_a, &endptr);
              g_free (tmpstr);
              return;
            }
          g_free (tmpstr);
        }
      ++n;
    }
}



gboolean 
gfig_load_style (Style *style,
                 FILE    *fp)
{
  gulong offset;
  gchar load_buf2[MAX_LOAD_LINE];
  gchar *style_text[100];
  gint  nitems = 0;
  gint  k;
  gchar name[100];
  offset = ftell (fp);

  get_line (load_buf2, MAX_LOAD_LINE, fp, 0);
  if (1 != sscanf (load_buf2, "<Style %s>", name))
    {
      /* no style data */
      fprintf (stderr, "No style data\n");
      fseek (fp, offset, SEEK_SET);
      return TRUE;
    }

  if (gfig_context->debug_styles)
    fprintf (stderr, "Loading style '%s' -- ", name);

  /* nuke final > in name */
  *strrchr (name, '>') = '\0';

  style->name = g_strdup (name);

  while (TRUE)
    {
      get_line (load_buf2, MAX_LOAD_LINE, fp, 0);
      if (!strcmp (load_buf2, "</Style>") || feof (fp))
        break;

      style_text[nitems] = (gchar *) g_malloc (MAX_LOAD_LINE);
      strcpy (style_text[nitems], load_buf2);
      nitems++;
    }

  if (feof (fp))
    {
      g_message ("Error reading style data");
      return TRUE;
    }

  gfig_read_parameter_string   (style_text, nitems, "BrushName",  &style->brush_name);
  if (! &style->brush_name )
    g_message ("Error loading style: got NULL for brush name.");

  gfig_read_parameter_string   (style_text, nitems, "Pattern",     &style->pattern);
  gfig_read_parameter_string   (style_text, nitems, "Gradient",    &style->gradient);
  gfig_read_parameter_gimp_rgb (style_text, nitems, "Foreground",  &style->foreground);
  gfig_read_parameter_gimp_rgb (style_text, nitems, "Background",  &style->background);
  gfig_read_parameter_int      (style_text, nitems, "FillType",    (gint *)&style->fill_type);
  gfig_read_parameter_double   (style_text, nitems, "FillOpacity", 
                                (gdouble *)&style->fill_opacity);

  for (k = 0; k < nitems; k++)
    {
      g_free (style_text[k]);
    }

  if (gfig_context->debug_styles)
    fprintf (stderr, "done\n");

  return FALSE;
}


gboolean 
gfig_skip_style (Style *style,
                 FILE    *fp)
{
  gulong offset;
  gchar load_buf2[MAX_LOAD_LINE];

  offset = ftell (fp);

  get_line (load_buf2, MAX_LOAD_LINE, fp, 0);
  if (strncmp (load_buf2, "<Style ", 7))
    {
      /* no style data */
      fseek (fp, offset, SEEK_SET);
      return TRUE;
    }

  while (TRUE)
    {
      get_line (load_buf2, MAX_LOAD_LINE, fp, 0);
      if (!strcmp (load_buf2, "</Style>") || feof (fp))
        break;
    }

  if (feof (fp))
    {
      g_message ("Error trying to skip style data");
      return TRUE;
    }

  return FALSE;
}

/*
 * FIXME: need to make this load a list of styles if there are more than one.
 */
gboolean
gfig_load_styles (GFigObj *gfig,
                  FILE *fp)
{
  if (gfig_context->debug_styles)
    fprintf (stderr, "Loading global styles -- ");

  /* currently we only have the default style */
  gfig_load_style (&gfig_context->default_style, fp);

  if (gfig_context->debug_styles)
    fprintf (stderr, "done\n");

  return FALSE;
}

void 
gfig_save_style (Style *style, 
                 GString *string)
{
  gchar buffer[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_r[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_g[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_b[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_a[G_ASCII_DTOSTR_BUF_SIZE];
  gint  blen = G_ASCII_DTOSTR_BUF_SIZE;

  if (gfig_context->debug_styles)
    fprintf (stderr, "Saving style %s, brush name '%s'\n", style->name, style->brush_name);

  g_string_append_printf (string, "<Style %s>\n", style->name);
  g_string_append_printf (string, "BrushName:      %s\n",          style->brush_name);
  if (!style->brush_name)
    g_message ("Error saving style %s: saving NULL for brush name", style->name);

  g_string_append_printf (string, "FillType:       %d\n",          style->fill_type);

  g_string_append_printf (string, "FillOpacity:    %s\n", 
                          g_ascii_dtostr (buffer, blen, style->fill_opacity));

  g_string_append_printf (string, "Pattern:        %s\n",          style->pattern);

  g_string_append_printf (string, "Gradient:       %s\n",          style->gradient);

  g_string_append_printf (string, "Foreground: %s %s %s %s\n", 
                          g_ascii_dtostr (buffer_r, blen, style->foreground.r),
                          g_ascii_dtostr (buffer_g, blen, style->foreground.g), 
                          g_ascii_dtostr (buffer_b, blen, style->foreground.b), 
                          g_ascii_dtostr (buffer_a, blen, style->foreground.a));

  g_string_append_printf (string, "Background: %s %s %s %s\n", 
                          g_ascii_dtostr (buffer_r, blen, style->background.r),
                          g_ascii_dtostr (buffer_g, blen, style->background.g), 
                          g_ascii_dtostr (buffer_b, blen, style->background.b), 
                          g_ascii_dtostr (buffer_a, blen, style->background.a));

  g_string_append_printf (string, "</Style>\n");
}

void
gfig_style_save_as_attributes (Style   *style,
                               GString *string)
{
  gchar buffer[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_r[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_g[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_b[G_ASCII_DTOSTR_BUF_SIZE];
  gchar buffer_a[G_ASCII_DTOSTR_BUF_SIZE];
  gint  blen = G_ASCII_DTOSTR_BUF_SIZE;

  if (gfig_context->debug_styles)
    fprintf (stderr, "Saving style %s as attributes\n", style->name);
  g_string_append_printf (string, "BrushName=\"%s\" ",  style->brush_name);

  g_string_append_printf (string, "Foreground=\"%s %s %s %s\" ",  
                          g_ascii_dtostr (buffer_r, blen, style->foreground.r),
                          g_ascii_dtostr (buffer_g, blen, style->foreground.g), 
                          g_ascii_dtostr (buffer_b, blen, style->foreground.b), 
                          g_ascii_dtostr (buffer_a, blen, style->foreground.a));

  g_string_append_printf (string, "Background=\"%s %s %s %s\" ",  
                          g_ascii_dtostr (buffer_r, blen, style->background.r),
                          g_ascii_dtostr (buffer_g, blen, style->background.g), 
                          g_ascii_dtostr (buffer_b, blen, style->background.b), 
                          g_ascii_dtostr (buffer_a, blen, style->background.a));

  g_string_append_printf (string, "FillType=%d ", style->fill_type);

  g_string_append_printf (string, "FillOpacity=%s ", 
                          g_ascii_dtostr (buffer, blen, style->fill_opacity));


}

void
gfig_save_styles (GString *string)
{
  gint k;

  if (gfig_context->debug_styles)
    fprintf (stderr, "Saving global styles.\n");

  gfig_style_copy (&gfig_context->default_style, gfig_context->current_style, "object");

  for (k = 1; k < gfig_context->num_styles; k++) 
    gfig_save_style (gfig_context->style[k], string);
}

/*
 * set_foreground_callback() is the callback for the Foreground color select
 * widget.  It reads the color from the widget, and applies this color
 * to both the default style and the current style.  It then produces a
 * repaint (which will be suppressed if gfig_context->enable_repaint is
 * FALSE).
 */
void
set_foreground_callback (GimpColorButton *button,
                         gpointer         data)
{
  GimpRGB color2;

  if (gfig_context->debug_styles)
    fprintf (stderr, "Setting foreground color from color selector\n");

  gimp_color_button_get_color (button, &color2);
  gimp_rgba_set (&gfig_context->default_style.foreground,
                 color2.r, color2.g, color2.b, color2.a);

  gimp_rgba_set (&gfig_context->current_style->foreground,
                 color2.r, color2.g, color2.b, color2.a);

  gfig_paint_callback ();
}

void
set_background_callback (GimpColorButton *button,
                         gpointer         data)
{
  GimpRGB color2;

  gimp_color_button_get_color (button, &color2);
  gimp_rgba_set (&gfig_context->default_style.background,
                 color2.r, color2.g, color2.b, color2.a);
  gimp_rgba_set (&gfig_context->current_style->background,
                 color2.r, color2.g, color2.b, color2.a);

  gfig_paint_callback ();
}

/*
 * gfig_brush_changed_callback() is the callback for the brush
 * selector widget.  It reads the brush name from the widget, and
 * applies this to both the default style and the current style,
 * as well as the gfig_context->bdesc values.  It then produces a
 * repaint (which will be suppressed if gfig_context->enable_repaint is
 * FALSE).
 */
void 
gfig_brush_changed_callback (const gchar *brush_name,
                             gdouble opacity,
                             gint spacing,
                             GimpLayerModeEffects paint_mode,
                             gint width,
                             gint height,
                             const guchar *mask_data,
                             gboolean dialog_closing,
                             gpointer user_data)
{
  if (!brush_name)
    g_message ("Error: setting brush name to NULL in color selector callback.");

  gfig_context->current_style->brush_name = (gchar *) brush_name;
  gfig_context->default_style.brush_name = (gchar *) brush_name;

  /* this will soon be unneeded */
  gfig_context->bdesc.name = (gchar *) brush_name;
  gfig_context->bdesc.width = width;
  gfig_context->bdesc.height = height;
  gimp_brushes_set_brush (brush_name);

  gfig_paint_callback ();
}

void 
gfig_pattern_changed_callback (const gchar *pattern_name,
                               gint width,
                               gint height,
                               gint bpp,
                               const guchar *mask_data,
                               gboolean dialog_closing,
                               gpointer user_data)
{
  gfig_context->current_style->pattern = g_strdup (pattern_name);
  gfig_context->default_style.pattern = g_strdup (pattern_name);

  gfig_paint_callback ();
}

void 
gfig_gradient_changed_callback (const gchar *gradient_name,
                                gint width,
                                const gdouble *grad_data,
                                gboolean dialog_closing,
                                gpointer user_data)
{
  gfig_context->current_style->gradient = g_strdup (gradient_name);
  gfig_context->default_style.gradient = g_strdup (gradient_name);

  gfig_paint_callback ();
}

void
gfig_rgba_copy (GimpRGB *color1,
                GimpRGB *color2)
{
  color1->r = color2->r;
  color1->g = color2->g;
  color1->b = color2->b;
  color1->a = color2->a;
}

void 
gfig_style_copy (Style *style1, 
                 Style *style0,
                 const gchar *name)
{
  if (name)
    style1->name = g_strdup (name);
  else
    g_message ("Eror: name is NULL in gfig_style_copy.");

  if (gfig_context->debug_styles)
    fprintf (stderr, "Copying style %s as style %s\n", style0->name, name);

  gfig_rgba_copy (&style1->foreground, &style0->foreground);
  gfig_rgba_copy (&style1->background, &style0->background);

  if (!style0->brush_name)
    g_message ("Error copying style %s: brush name is NULL.", style0->name);

  style1->brush_name    = g_strdup (style0->brush_name);
  style1->gradient      = g_strdup (style0->gradient);
  style1->pattern       = g_strdup (style0->pattern);
  style1->fill_type    = style0->fill_type;
  style1->fill_opacity = style0->fill_opacity;
}

/*
 * gfig_style_apply() applies the settings from the specified style to
 * the Gimp core.  It does not change any widgets, and does not cause
 * a repaint.
 */
void
gfig_style_apply (Style *style)
{
  if (gfig_context->debug_styles)
    fprintf (stderr, "Applying style '%s' -- ", style->name);

  gimp_palette_set_foreground (&style->foreground);

  gimp_palette_set_background (&style->background);

  if (!gimp_brushes_set_brush (style->brush_name))
    g_message ("Style apply: Failed to set brush to '%s' in style '%s'", 
               style->brush_name, style->name);

  gimp_patterns_set_pattern (style->pattern);

  gimp_gradients_set_gradient (style->gradient);

  gfig_context->current_style = style;

  if (gfig_context->debug_styles)
    fprintf (stderr, "done.\n");
}

void
gfig_style_append (Style *style)
{
  gfig_context->style[gfig_context->num_styles] = style;
  ++gfig_context->num_styles;
}

/*
 * gfig_read_gimp_style() reads the style settings from the Gimp core,
 * and applies them to the specified style, giving that style the
 * specified name.  This is mainly useful as a way of initializing
 * a style.  The function does not cause a repaint.
 */
void
gfig_read_gimp_style (Style *style,
                      const gchar *name)
{
  gint w, h;

  if (!name)
    g_message ("Error: name is NULL in gfig_read_gimp_style.");

  if (gfig_context->debug_styles)
    fprintf (stderr, "Reading Gimp settings as style %s\n", name);
  style->name = g_strdup (name);

  gimp_palette_get_foreground (&style->foreground);
  gimp_palette_get_background (&style->background);
  style->brush_name = (gchar *) gimp_brushes_get_brush (&style->brush_width,
                                                        &style->brush_height,
                                                        &style->brush_spacing);
  style->gradient = gimp_gradients_get_gradient ();
  style->pattern = gimp_patterns_get_pattern (&w, &h);

  style->fill_opacity = 100.;

  gfig_context->bdesc.name = style->brush_name;
  gfig_context->bdesc.width = style->brush_width;
  gfig_context->bdesc.height = style->brush_height;
}

/*
 * gfig_style_set_content_from_style() sets all of the style control widgets
 * to values from the specified style.  This in turn sets the Gimp core's
 * values to the same things.  Repainting is suppressed while this happens,
 * so calling this function will not produce a repaint.
 *
 */
void
gfig_style_set_context_from_style (Style *style)
{
  gboolean enable_repaint;

  if (gfig_context->debug_styles)
    fprintf (stderr, "Setting context from style '%s' -- ", style->name);

  enable_repaint = gfig_context->enable_repaint;
  gfig_context->enable_repaint = FALSE;

  gfig_context->current_style = style;

  gimp_color_button_set_color (GIMP_COLOR_BUTTON (gfig_context->fg_color_button),
                               &style->foreground);
  gimp_color_button_set_color (GIMP_COLOR_BUTTON (gfig_context->bg_color_button),
                               &style->background);
  if (!gimp_brushes_set_brush (style->brush_name))
    g_message ("Style from context: Failed to set brush to '%s'", style->brush_name);

  gimp_brush_select_widget_set (gfig_context->brush_select,
                                style->brush_name,
                                -1., -1, -1);  /* FIXME */

  gimp_pattern_select_widget_set (gfig_context->pattern_select,
                                  style->pattern);

  gimp_gradient_select_widget_set (gfig_context->gradient_select,
                                  style->gradient);

  gfig_context->bdesc.name = style->brush_name;
  if (gfig_context->debug_styles)
    fprintf (stderr, "done.\n");

  gimp_int_combo_box_set_active (GIMP_INT_COMBO_BOX (gfig_context->fillstyle_combo),
                                 (gint) style->fill_type);

  gfig_context->enable_repaint = enable_repaint;
}

/*
 * gfig_style_set_style_from_context() sets the values in the specified
 * style to those that appear in the style control widgets f
 */
void
gfig_style_set_style_from_context (Style *style)
{
  GimpRGB color;

  style->name = "object";

  gimp_color_button_get_color (GIMP_COLOR_BUTTON (gfig_context->fg_color_button),
                               &color);
  if (gfig_context->debug_styles)
    fprintf (stderr, "Setting foreground color to %lg %lg %lg\n", color.r, color.g, color.b);

  gfig_rgba_copy (&style->foreground, &color);
  gimp_color_button_get_color (GIMP_COLOR_BUTTON (gfig_context->bg_color_button),
                               &color);
  gfig_rgba_copy (&style->background, &color);

  style->brush_name   = gfig_context->current_style->brush_name;

  if (!style->pattern || strcmp (style->pattern, gfig_context->current_style->pattern))
    {
      style->pattern    = g_strdup (gfig_context->current_style->pattern);
    }

  style->gradient     = gfig_context->current_style->gradient;

  gimp_int_combo_box_get_active (GIMP_INT_COMBO_BOX (gfig_context->fillstyle_combo),
                                 (gint *) &style->fill_type);

  /* FIXME when there is an opacity control widget to read */
  style->fill_opacity = 100.;
}

char *
mygimp_brush_get (void)
{
  gint width, height, spacing;

  return gimp_brushes_get_brush (&width, &height, &spacing);
}

void
mygimp_brush_info (gint *width,
                   gint *height)
{
  char *name;
  gint spacing;

  name = gimp_brushes_get_brush (width, height, &spacing);
  if (name)
    {
      *width  = MAX (*width, 32);
      *height = MAX (*height, 32);
      g_free (name);
    }
  else
    {
      g_message ("Failed to get brush info");
      *width = *height = 48;
    }
}

