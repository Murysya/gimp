#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include <glib-object.h>

#include "base/base-types.h"

#include "gimp-composite.h"
#include "gimp-composite-regression.h"
#include "gimp-composite-util.h"
#include "gimp-composite-generic.h"
#include "gimp-composite-sse2.h"

int
gimp_composite_sse2_test (int iterations, int n_pixels)
{
#if (__GNUC__ >= 3) && defined(USE_SSE)     && defined(ARCH_X86)
  GimpCompositeContext generic_ctx;
  GimpCompositeContext special_ctx;
  double ft0;
  double ft1;
  gimp_rgba8_t *rgba8D1;
  gimp_rgba8_t *rgba8D2;
  gimp_rgba8_t *rgba8A;
  gimp_rgba8_t *rgba8B;
  gimp_rgba8_t *rgba8M;
  gimp_va8_t *va8A;
  gimp_va8_t *va8B;
  gimp_va8_t *va8M;
  gimp_va8_t *va8D1;
  gimp_va8_t *va8D2;
  int i;

  rgba8A =  gimp_composite_regression_fixed_rgba8(n_pixels+1);
  rgba8B =  gimp_composite_regression_fixed_rgba8(n_pixels+1);
  rgba8M =  gimp_composite_regression_fixed_rgba8(n_pixels+1);
  rgba8D1 = (gimp_rgba8_t *) calloc(sizeof(gimp_rgba8_t), n_pixels+1);
  rgba8D2 = (gimp_rgba8_t *) calloc(sizeof(gimp_rgba8_t), n_pixels+1);
  va8A =    (gimp_va8_t *)   calloc(sizeof(gimp_va8_t), n_pixels+1);
  va8B =    (gimp_va8_t *)   calloc(sizeof(gimp_va8_t), n_pixels+1);
  va8M =    (gimp_va8_t *)   calloc(sizeof(gimp_va8_t), n_pixels+1);
  va8D1 =   (gimp_va8_t *)   calloc(sizeof(gimp_va8_t), n_pixels+1);
  va8D2 =   (gimp_va8_t *)   calloc(sizeof(gimp_va8_t), n_pixels+1);

  for (i = 0; i < n_pixels; i++) {
    va8A[i].v = i;
    va8A[i].a = 255-i;
    va8B[i].v = i;
    va8B[i].a = i;
    va8M[i].v = i;
    va8M[i].a = i;
  }


  gimp_composite_context_init (&special_ctx, GIMP_COMPOSITE_DIFFERENCE, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D2);
  gimp_composite_context_init (&generic_ctx, GIMP_COMPOSITE_DIFFERENCE, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D1);
  ft0 = gimp_composite_regression_time_function (iterations, gimp_composite_dispatch, &generic_ctx);
  ft1 = gimp_composite_regression_time_function (iterations, gimp_composite_difference_rgba8_rgba8_rgba8_sse2, &special_ctx);
  if (gimp_composite_regression_compare_contexts ("difference", &generic_ctx, &special_ctx)) {
    return (1);
  }
  gimp_composite_regression_timer_report ("difference", ft0, ft1);

  gimp_composite_context_init (&special_ctx, GIMP_COMPOSITE_ADDITION, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D2);
  gimp_composite_context_init (&generic_ctx, GIMP_COMPOSITE_ADDITION, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D1);
  ft0 = gimp_composite_regression_time_function (iterations, gimp_composite_dispatch, &generic_ctx);
  ft1 = gimp_composite_regression_time_function (iterations, gimp_composite_addition_rgba8_rgba8_rgba8_sse2, &special_ctx);
  if (gimp_composite_regression_compare_contexts ("addition", &generic_ctx, &special_ctx)) {
    return (1);
  }
  gimp_composite_regression_timer_report ("addition", ft0, ft1);

  gimp_composite_context_init (&special_ctx, GIMP_COMPOSITE_SUBTRACT, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D2);
  gimp_composite_context_init (&generic_ctx, GIMP_COMPOSITE_SUBTRACT, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D1);
  ft0 = gimp_composite_regression_time_function (iterations, gimp_composite_dispatch, &generic_ctx);
  ft1 = gimp_composite_regression_time_function (iterations, gimp_composite_subtract_rgba8_rgba8_rgba8_sse2, &special_ctx);
  if (gimp_composite_regression_compare_contexts ("subtract", &generic_ctx, &special_ctx)) {
    return (1);
  }
  gimp_composite_regression_timer_report ("subtract", ft0, ft1);

  gimp_composite_context_init (&special_ctx, GIMP_COMPOSITE_SWAP, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D2);
  gimp_composite_context_init (&generic_ctx, GIMP_COMPOSITE_SWAP, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, GIMP_PIXELFORMAT_RGBA8, n_pixels, (unsigned char *) rgba8A, (unsigned char *) rgba8B, (unsigned char *) rgba8B, (unsigned char *) rgba8D1);
  ft0 = gimp_composite_regression_time_function (iterations, gimp_composite_dispatch, &generic_ctx);
  ft1 = gimp_composite_regression_time_function (iterations, gimp_composite_swap_rgba8_rgba8_rgba8_sse2, &special_ctx);
  if (gimp_composite_regression_compare_contexts ("swap", &generic_ctx, &special_ctx)) {
    return (1);
  }
  gimp_composite_regression_timer_report ("swap", ft0, ft1);
#endif
  return (0);
}

int
main (int argc, char *argv[])
{
  int iterations;
  int n_pixels;

  srand (314159);

  putenv ("GIMP_COMPOSITE=0x1");

  iterations = 1;
  n_pixels = 1048577;

  argv++, argc--;
  while (argc >= 2) {
    if (argc > 1 && (strcmp (argv[0], "--iterations") == 0 || strcmp (argv[0], "-i") == 0)) {
      iterations = atoi(argv[1]);
      argc -= 2, argv++; argv++;
    } else if (argc > 1 && (strcmp (argv[0], "--n-pixels") == 0 || strcmp (argv[0], "-n") == 0)) {
      n_pixels = atoi (argv[1]);
      argc -= 2, argv++; argv++;
    } else {
      printf("Usage: gimp-composites-*-test [-i|--iterations n] [-n|--n-pixels n]");
      argc--, argv++;
    }
  }

  gimp_composite_generic_install ();

  return (gimp_composite_sse2_test (iterations, n_pixels));
}
