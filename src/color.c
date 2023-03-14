#include "color.h"

#include <stdlib.h>


FH_API struct fh_color fh_col_set(u8 red, u8 green, u8 blue, u8 alpha)
{
	struct fh_color col;
	col.red = red;
	col.green = green;
	col.blue = blue;
	col.alpha = alpha;
	return col;
}


FH_API u32 fh_color_get(struct fh_color color)
{
	return *((u32 *)&color);
}
