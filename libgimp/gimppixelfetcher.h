/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimppixelfetcher.h
 * Contains all kinds of miscellaneous routines factored out from different
 * plug-ins. They stay here until their API has crystalized a bit and we can
 * put them into the file where they belong (Maurits Rijk
 * <lpeek.mrijk@consunet.nl> if you want to blame someone for this mess)
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

#ifndef __GIMP_PIXEL_FETCHER_H__
#define __GIMP_PIXEL_FETCHER_H__

G_BEGIN_DECLS

/* For information look into the C source or the html documentation */

typedef enum
{
  GIMP_PIXEL_FETCHER_EDGE_NONE,
  GIMP_PIXEL_FETCHER_EDGE_WRAP,
  GIMP_PIXEL_FETCHER_EDGE_SMEAR,
  GIMP_PIXEL_FETCHER_EDGE_BLACK
} GimpPixelFetcherEdgeMode;


typedef struct _GimpPixelFetcher GimpPixelFetcher;

GimpPixelFetcher * gimp_pixel_fetcher_new           (GimpDrawable     *drawable);
void 		   gimp_pixel_fetcher_set_edge_mode (GimpPixelFetcher *pf,
                                                     GimpPixelFetcherEdgeMode mode);

void               gimp_pixel_fetcher_set_bg_color  (GimpPixelFetcher *pf);
void		   gimp_pixel_fetcher_set_shadow    (GimpPixelFetcher *pf,
						     gboolean          shadow);
void               gimp_pixel_fetcher_get_pixel     (GimpPixelFetcher *pf,
						     gint              x,
						     gint              y,
						     guchar           *pixel);
void               gimp_pixel_fetcher_put_pixel     (GimpPixelFetcher *pf,
						     gint              x,
						     gint              y,
						     const guchar     *pixel);
void               gimp_pixel_fetcher_destroy       (GimpPixelFetcher *pf);


void		   gimp_get_bg_guchar               (GimpDrawable     *drawable,
						     gboolean          transparent,
						     guchar           *bg);
void		   gimp_get_fg_guchar               (GimpDrawable     *drawable,
						     gboolean          transparent,
						     guchar           *fg);

G_END_DECLS

#endif /* __GIMP_PIXEL_FETCHER_H__ */
