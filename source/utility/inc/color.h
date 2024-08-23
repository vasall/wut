#ifndef _WUT_UTILITY_COLOR_H
#define _WUT_UTILITY_COLOR_H


#include "core/inc/define.h"
#include "core/inc/import.h"

/*
 * A pixel is defined by 4 attributes representing the RGBA-values.
 * Each value can be set in range of 0 t0 255.
 */
#define WUT_COLOR_SIZE	(4*sizeof(u8))

struct wut_Color {	
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
WUT_API struct wut_Color wut_SetColor(u8 red, u8 green, u8 blue, u8 alpha);


/*
 * Create a new color object from the given HEX color.
 *
 * @hex: The color in hex format
 *
 * Returns: A color object
 */
WUT_API struct wut_Color wut_SetColorHex(u32 hex);


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
WUT_API u32 wut_SetColorU32(u8 red, u8 green, u8 blue, u8 alpha);


/*
 * Get the color as an 32 bit unsigned integer.
 *
 * @color: The color struct
 *
 * Returns: The color code
 */
WUT_API u32 wut_GetColorU32(struct wut_Color color);


/*
 * Get the color as a float vector. This is used when sending data to OpenGL.
 *
 * @color: The color struct
 * @fv: A float array to write the output color to
 */
WUT_API void wut_GetColorFV(struct wut_Color color, float *fv);


/*
 * Blend two RGBA values.
 * 
 * @a: The first color
 * @b: The second color
 *
 * Returns: The blended color
 */
WUT_API struct wut_Color wut_BlendColor(struct wut_Color a,
                struct wut_Color b);


/*
 * Reverse the order of the bytes.
 */
WUT_API struct wut_Color wut_InvColor(struct wut_Color in);
WUT_API u32 wut_InvColorU32(u32 in);

#endif /* WUT_UTILITY_COLOR_H */
