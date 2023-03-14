#ifndef _FH_COLOR_H
#define _FH_COLOR_H


#include "define.h"
#include "import.h"


struct fh_color {	
	u8 red;
	u8 green;
	u8 blue;
	u8 alpha;

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


#endif /* FH_COLOR_H */
