#ifndef _FH_COLOR_H
#define _FH_COLOR_H


#include "define.h"
#include "datatype.h"
#include "import.h"

/*
 * A pixel is defined by 4 attributes representing the RGBA-values.
 * Each value can be set in range of 0 t0 255.
 */
#define FH_COLOR_SIZE	(4*sizeof(u8))
struct fh_color {	
	u8 r;
	u8 g;
	u8 b;
	u8 a;
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
 * Get the color as an 32 bit unsigned integer.
 *
 * @color: The color
 *
 * Returns: The color code
 */
FH_API u32 fh_color_get(struct fh_color color);


/*
 * Blend two RGBA values.
 * 
 * @a: The first color
 * @b: The second color
 *
 * Returns: The blended color
 */
FH_API struct fh_color fh_BlendColor(struct fh_color a, struct fh_color b);

#endif /* FH_COLOR_H */
