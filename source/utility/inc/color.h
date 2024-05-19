#ifndef _WT_UTILITY_COLOR_H
#define _WT_UTILITY_COLOR_H


#include "core/inc/define.h"
#include "core/inc/import.h"

/*
 * A pixel is defined by 4 attributes representing the RGBA-values.
 * Each value can be set in range of 0 t0 255.
 */
#define WT_COLOR_SIZE	(4*sizeof(u8))
struct wt_color {	
	u8 a;
	u8 b;
	u8 g;
	u8 r;
};

typedef struct wt_color wt_color_t;


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
WT_API struct wt_color wt_color_set(u8 red, u8 green, u8 blue, u8 alpha);


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
WT_API u32 wt_color_set_u32(u8 red, u8 green, u8 blue, u8 alpha);


/*
 * Get the color as an 32 bit unsigned integer.
 *
 * @color: The color struct
 *
 * Returns: The color code
 */
WT_API u32 wt_color_get(struct wt_color color);


/*
 * Get the color as a float vector. This is used when sending data to OpenGL.
 *
 * @color: The color struct
 * @fv: A float array to write the output color to
 */
WT_API void wt_color_get_fv(struct wt_color color, float *fv);


/*
 * Blend two RGBA values.
 * 
 * @a: The first color
 * @b: The second color
 *
 * Returns: The blended color
 */
WT_API struct wt_color wt_BlendColor(struct wt_color a, struct wt_color b);


/*
 * Convert a color value to a color struct.
 *
 * @v: The color value
 *
 * Returns: The color struct
 */
WT_API struct wt_color wt_col_conv_itos(u32 v);


/*
 * Reverse the order of the bytes.
 */
WT_API struct wt_color wt_col_invform(struct wt_color in);
WT_API u32 wt_col_invform_s32(u32 in);

#endif /* WT_UTILITY_COLOR_H */
