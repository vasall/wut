#ifndef _FH_COLOR_H
#define _FH_COLOR_H


#include "define.h"
#include "import.h"


struct fh_color {	
	u8 data[4];

	union {
		u8 red;
		u8 green;
		u8 blue;
		u8 alpha;
	} codes;
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
FH_API struct fh_color FH_COL_NEW(u8 red, u8 green, u8 blue, u8 alpha);



#endif /* FH_COLOR_H */
