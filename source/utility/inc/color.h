#ifndef _FH_UTILITY_COLOR_H
#define _FH_UTILITY_COLOR_H


#include "core/inc/define.h"
#include "core/inc/import.h"


#define FH_SWAP32(x) (__builtin_bswap32(x))

/*
 * A pixel is defined by 4 attributes representing the RGBA-values.
 * Each value can be set in range of 0 t0 255.
 */
#define FH_COLOR_SIZE	(4*sizeof(u8))
struct fh_color {	
	u8 a;
	u8 b;
	u8 g;
	u8 r;
};



/*
 * Create a new color object from the given channel values.
 *
 * @red: The red channel value
 * @green: The green channel value
 * @blue: The blue channel value
 * @alpha: The alpha channel value
 *
 * Returns: A color object
 */
FH_API struct fh_color fh_col_set(u8 red, u8 green, u8 blue, u8 alpha);


/*
 * Convert the given color channel values to one final color.
 *
 * @red: the red channel value
 * @green: the green channel value
 * @blue: the blue channel value
 * @alpha: the alpha channel value
 *
 * Returns: The color as an unsigned integer
 */
FH_API u32 fh_col_set_u32(u8 red, u8 green, u8 blue, u8 alpha);


/*
 * Get the color as an 32 bit unsigned integer.
 *
 * @color: The color struct
 *
 * Returns: The color code
 */
FH_API u32 fh_color_get(struct fh_color color);


/*
 * Get the color as a float vector. This is used when sending data to OpenGL.
 *
 * @color: The color struct
 * @fv: A float array to write the output color to
 */
FH_API void fh_color_get_fv(struct fh_color color, float *fv);


/*
 * Blend two RGBA values.
 * 
 * @a: The first color
 * @b: The second color
 *
 * Returns: The blended color
 */
FH_API struct fh_color fh_BlendColor(struct fh_color a, struct fh_color b);


/*
 * Convert a color value to a color struct.
 *
 * @v: The color value
 *
 * Returns: The color struct
 */
FH_API struct fh_color fh_col_conv_itos(u32 v);


/*
 * Reverse the order of the bytes.
 */
FH_API struct fh_color fh_col_invform(struct fh_color in);
FH_API u32 fh_col_invform_s32(u32 in);

#endif /* FH_UTILITY_COLOR_H */
