/* THIS FILE IS AUTOMATICALLY GENERATED.  DO NOT EDIT */
/* REGENERATE BY USING make-installer.py */
#include "config.h"
#include <glib-object.h>
#include <stdlib.h>
#include <stdio.h>
#include "base/base-types.h"
#include "gimp-composite.h"

#include "gimp-composite-sse2.h"

static struct install_table {
  GimpCompositeOperation mode;
  GimpPixelFormat A;
  GimpPixelFormat B;
  GimpPixelFormat D;
  void (*function)(GimpCompositeContext *);
} _gimp_composite_sse2[] = {
#if (__GNUC__ >= 3) && defined(USE_SSE)     && defined(ARCH_X86)
 { GIMP_COMPOSITE_DIFFERENCE, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, gimp_composite_difference_rgba8_rgba8_rgba8_sse2 }, 
 { GIMP_COMPOSITE_ADDITION, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, gimp_composite_addition_rgba8_rgba8_rgba8_sse2 }, 
 { GIMP_COMPOSITE_SUBTRACT, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, gimp_composite_subtract_rgba8_rgba8_rgba8_sse2 }, 
 { GIMP_COMPOSITE_SWAP, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, gimp_composite_swap_rgba8_rgba8_rgba8_sse2 }, 
#endif
 { 0, 0, 0, 0, NULL }
};

void
gimp_composite_sse2_install (void)
{
  static struct install_table *t = _gimp_composite_sse2;

  for (t = &_gimp_composite_sse2[0]; t->function != NULL; t++) {
    gimp_composite_function[t->mode][t->A][t->B][t->D] = t->function;
  }

  gimp_composite_sse2_init ();
}
