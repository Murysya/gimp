/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpscanner.c
 * Copyright (C) 2002  Sven Neumann <sven@gimp.org>
 *                     Michael Natterer <mitch@gimp.org>
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

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>

#include <glib-object.h>

#ifdef G_OS_WIN32
#include <io.h>
#endif

#include "libgimpbase/gimpbase.h"
#include "libgimpcolor/gimpcolor.h"
#include "libgimpmath/gimpmath.h"

#include "config-types.h"

#include "gimpconfig-error.h"
#include "gimpscanner.h"

#include "gimp-intl.h"


typedef struct
{
  gint      fd;
  gchar    *name;
  GError  **error;
} GimpScannerData;


/*  local function prototypes  */

static GScanner * gimp_scanner_new     (const gchar  *name,
                                        gint          fd,
                                        GError      **error);
static void       gimp_scanner_message (GScanner     *scanner,
                                        gchar        *message,
                                        gboolean      is_error);


/*  public functions  */

GScanner *
gimp_scanner_new_file (const gchar  *filename,
		       GError      **error)
{
  GScanner *scanner;
  gint      fd;

  g_return_val_if_fail (filename != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  fd = open (filename, O_RDONLY);

  if (fd == -1)
    {
      GimpConfigError code;

      code = (errno == ENOENT ?
              GIMP_CONFIG_ERROR_OPEN_ENOENT : GIMP_CONFIG_ERROR_OPEN);

      g_set_error (error, GIMP_CONFIG_ERROR, code,
                   _("Could not open '%s' for reading: %s"),
                   filename, g_strerror (errno));

      return NULL;
    }

  scanner = gimp_scanner_new (filename, fd, error);

  g_scanner_input_file (scanner, fd);

  return scanner;
}

GScanner *
gimp_scanner_new_string (const gchar  *text,
			 gint          text_len,
			 GError      **error)
{
  GScanner *scanner;

  g_return_val_if_fail (text != NULL || text_len == 0, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  if (text_len < 0)
    text_len = strlen (text);

  scanner = gimp_scanner_new (NULL, -1, error);

  g_scanner_input_text (scanner, text, text_len);

  return scanner;
}

static GScanner *
gimp_scanner_new (const gchar  *name,
                  gint          fd,
                  GError      **error)
{
  GScanner        *scanner;
  GimpScannerData *data;

  scanner = g_scanner_new (NULL);

  data = g_new0 (GimpScannerData, 1);

  data->name  = g_strdup (name);
  data->fd    = fd;
  data->error = error;

  scanner->user_data   = data;
  scanner->msg_handler = gimp_scanner_message;

  scanner->config->cset_identifier_first = ( G_CSET_a_2_z G_CSET_A_2_Z );
  scanner->config->cset_identifier_nth   = ( G_CSET_a_2_z G_CSET_A_2_Z
                                             G_CSET_DIGITS "-_" );
  scanner->config->scan_identifier_1char = TRUE;

  scanner->config->store_int64           = TRUE;

  return scanner;
}

void
gimp_scanner_destroy (GScanner *scanner)
{
  GimpScannerData *data;

  g_return_if_fail (scanner != NULL);

  data = scanner->user_data;

  if (data->fd > 0)
    {
      if (close (data->fd))
        g_warning ("%s: could not close file descriptor: %s",
                   G_GNUC_PRETTY_FUNCTION, g_strerror (errno));
    }

  g_free (data->name);
  g_free (data);

  g_scanner_destroy (scanner);
}


gboolean
gimp_scanner_parse_token (GScanner   *scanner,
                          GTokenType  token)
{
  if (g_scanner_peek_next_token (scanner) != token)
    return FALSE;

  g_scanner_get_next_token (scanner);

  return TRUE;
}

gboolean
gimp_scanner_parse_identifier (GScanner    *scanner,
                               const gchar *identifier)
{
  if (g_scanner_peek_next_token (scanner) != G_TOKEN_IDENTIFIER)
    return FALSE;

  g_scanner_get_next_token (scanner);

  if (strcmp (scanner->value.v_identifier, identifier))
    return FALSE;

  return TRUE;
}

gboolean
gimp_scanner_parse_string (GScanner  *scanner,
                           gchar    **dest)
{
  if (g_scanner_peek_next_token (scanner) != G_TOKEN_STRING)
    return FALSE;

  g_scanner_get_next_token (scanner);

  if (*scanner->value.v_string)
    {
      if (! g_utf8_validate (scanner->value.v_string, -1, NULL))
        {
          g_scanner_warn (scanner, _("invalid UTF-8 string"));
          return FALSE;
        }

      *dest = g_strdup (scanner->value.v_string);
    }
  else
    {
      *dest = NULL;
    }

  return TRUE;
}

gboolean
gimp_scanner_parse_string_no_validate (GScanner  *scanner,
                                       gchar    **dest)
{
  if (g_scanner_peek_next_token (scanner) != G_TOKEN_STRING)
    return FALSE;

  g_scanner_get_next_token (scanner);

  if (*scanner->value.v_string)
    *dest = g_strdup (scanner->value.v_string);
  else
    *dest = NULL;

  return TRUE;
}

gboolean
gimp_scanner_parse_int (GScanner *scanner,
                        gint     *dest)
{
  gboolean negate = FALSE;

  if (g_scanner_peek_next_token (scanner) == '-')
    {
      negate = TRUE;
      g_scanner_get_next_token (scanner);
    }

  if (g_scanner_peek_next_token (scanner) != G_TOKEN_INT)
    return FALSE;

  g_scanner_get_next_token (scanner);

  if (negate)
    *dest = -scanner->value.v_int64;
  else
    *dest = scanner->value.v_int64;

  return TRUE;
}

gboolean
gimp_scanner_parse_float (GScanner *scanner,
                          gdouble  *dest)
{
  if (g_scanner_peek_next_token (scanner) != G_TOKEN_FLOAT)
    return FALSE;

  g_scanner_get_next_token (scanner);

  *dest = scanner->value.v_float;

  return TRUE;
}

enum
{
  COLOR_RGB  = 1,
  COLOR_RGBA,
  COLOR_HSV,
  COLOR_HSVA
};

gboolean
gimp_scanner_parse_color (GScanner *scanner,
                          GimpRGB  *dest)
{
  guint      scope_id;
  guint      old_scope_id;
  GTokenType token;
  GimpRGB    color;

  scope_id = g_quark_from_static_string ("gimp_scanner_parse_color");
  old_scope_id = g_scanner_set_scope (scanner, scope_id);

  if (! g_scanner_scope_lookup_symbol (scanner, scope_id, "color-rgb"))
    {
      g_scanner_scope_add_symbol (scanner, scope_id, 
                                  "color-rgb", GINT_TO_POINTER (COLOR_RGB));
      g_scanner_scope_add_symbol (scanner, scope_id, 
                                  "color-rgba", GINT_TO_POINTER (COLOR_RGBA));
      g_scanner_scope_add_symbol (scanner, scope_id, 
                                  "color-hsv", GINT_TO_POINTER (COLOR_HSV));
      g_scanner_scope_add_symbol (scanner, scope_id, 
                                  "color-hsva", GINT_TO_POINTER (COLOR_HSVA));
    }

  token = G_TOKEN_LEFT_PAREN;

  while (g_scanner_peek_next_token (scanner) == token)
    {
      token = g_scanner_get_next_token (scanner);

      switch (token)
        {
        case G_TOKEN_LEFT_PAREN:
          token = G_TOKEN_SYMBOL;
          break;

        case G_TOKEN_SYMBOL:
          {
            gdouble  col[4] = { 0.0, 0.0, 0.0, 1.0 };
            gint     n_channels = 4;
            gboolean is_hsv     = FALSE;
            gint     i;

            switch (GPOINTER_TO_INT (scanner->value.v_symbol))
              {
              case COLOR_RGB:
                n_channels = 3;
                /* fallthrough */
              case COLOR_RGBA:
                break;

              case COLOR_HSV:
                n_channels = 3;
                /* fallthrough */
              case COLOR_HSVA:
                is_hsv = TRUE;
                break;
              }

            token = G_TOKEN_FLOAT;

            for (i = 0; i < n_channels; i++)
              {
                if (! gimp_scanner_parse_float (scanner, &col[i]))
                  goto finish;
              }

            if (is_hsv)
              {
                GimpHSV hsv;

                gimp_hsva_set (&hsv, col[0], col[1], col[2], col[3]);
                gimp_hsv_clamp (&hsv);

                gimp_hsv_to_rgb (&hsv, &color);
              }
            else
              {
                gimp_rgba_set (&color, col[0], col[1], col[2], col[3]);
                gimp_rgb_clamp (&color);
              }

            token = G_TOKEN_RIGHT_PAREN;
          }
          break;

        case G_TOKEN_RIGHT_PAREN:
          token = G_TOKEN_NONE; /* indicates success */
          goto finish;

        default: /* do nothing */
          break;
        }
    }

 finish:

  if (token != G_TOKEN_NONE)
    {
      g_scanner_get_next_token (scanner);
      g_scanner_unexp_token (scanner, token, NULL, NULL, NULL,
                             _("fatal parse error"), TRUE);
    }
  else
    {
      *dest = color;
    }

  g_scanner_set_scope (scanner, old_scope_id);

  return (token == G_TOKEN_NONE);
}

gboolean
gimp_scanner_parse_matrix2 (GScanner    *scanner,
                            GimpMatrix2 *dest)
{
  guint        scope_id;
  guint        old_scope_id;
  GTokenType   token;
  GimpMatrix2  matrix;

  scope_id = g_quark_from_static_string ("gimp_scanner_parse_matrix");
  old_scope_id = g_scanner_set_scope (scanner, scope_id);

  if (! g_scanner_scope_lookup_symbol (scanner, scope_id, "matrix"))
    g_scanner_scope_add_symbol (scanner, scope_id, "matrix", 0);

  token = G_TOKEN_LEFT_PAREN;

  while (g_scanner_peek_next_token (scanner) == token)
    {
      token = g_scanner_get_next_token (scanner);

      switch (token)
        {
        case G_TOKEN_LEFT_PAREN:
          token = G_TOKEN_SYMBOL;
          break;

        case G_TOKEN_SYMBOL:
          {
            token = G_TOKEN_FLOAT;

            if (! gimp_scanner_parse_float (scanner, &matrix.coeff[0][0]))
              goto finish;
            if (! gimp_scanner_parse_float (scanner, &matrix.coeff[0][1]))
              goto finish;
            if (! gimp_scanner_parse_float (scanner, &matrix.coeff[1][0]))
              goto finish;
            if (! gimp_scanner_parse_float (scanner, &matrix.coeff[1][1]))
              goto finish;

            token = G_TOKEN_RIGHT_PAREN;
          }
          break;

        case G_TOKEN_RIGHT_PAREN:
          token = G_TOKEN_NONE; /* indicates success */
          goto finish;

        default: /* do nothing */
          break;
        }
    }

 finish:

  if (token != G_TOKEN_NONE)
    {
      g_scanner_get_next_token (scanner);
      g_scanner_unexp_token (scanner, token, NULL, NULL, NULL,
                             _("fatal parse error"), TRUE);
    }
  else
    {
      *dest = matrix;
    }

  g_scanner_set_scope (scanner, old_scope_id);

  return (token == G_TOKEN_NONE);
}


/*  private functions  */

static void
gimp_scanner_message (GScanner *scanner,
                      gchar    *message,
                      gboolean  is_error)
{
  GimpScannerData *data = scanner->user_data;

  /* we don't expect warnings */
  g_return_if_fail (is_error);

  if (data->name)
    g_set_error (data->error, GIMP_CONFIG_ERROR, GIMP_CONFIG_ERROR_PARSE,
                 _("Error while parsing '%s' in line %d: %s"),
                 data->name, scanner->line, message);
  else
    /*  should never happen, thus not marked for translation  */
    g_set_error (data->error, GIMP_CONFIG_ERROR, GIMP_CONFIG_ERROR_PARSE,
                 "Error parsing internal buffer: %s", message);
}
