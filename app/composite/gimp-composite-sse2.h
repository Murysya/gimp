#ifndef gimp_composite_sse2_h
#define gimp_composite_sse2_h

extern void gimp_composite_sse2_init (void);

/*
	* The function gimp_composite_*_install() is defined in the code generated by make-install.py
	* I hate to create a .h file just for that declaration, so I do it here (for now).
 */
extern void gimp_composite_sse2_install (void);

extern void gimp_composite_addition_rgba8_rgba8_rgba8_sse2 (GimpCompositeContext *);
extern void gimp_composite_difference_rgba8_rgba8_rgba8_sse2 (GimpCompositeContext *);
extern void gimp_composite_subtract_rgba8_rgba8_rgba8_sse2 (GimpCompositeContext *);
extern void gimp_composite_swap_rgba8_rgba8_rgba8_sse2 (GimpCompositeContext *);
#endif
