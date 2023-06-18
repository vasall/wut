#include "color.h"

#include <stdlib.h>


FH_API struct fh_color fh_col_set(u8 red, u8 green, u8 blue, u8 alpha)
{
	struct fh_color col;
	col.r = red;
	col.g = green;
	col.b = blue;
	col.a = alpha;
	return col;
}


FH_API u32 fh_color_get(struct fh_color color)
{
	return *((u32 *)&color);
}


FH_API struct fh_color fh_BlendColor(struct fh_color px, struct fh_color col)
{
	struct fh_color c;
	/* Convert alpha values to the range [0, 1] */
	f32 alphaF = col.a / 255.0f;
	f32  oneMinusAlphaF = 1.0f - alphaF;

	/* Red channel */
	c.r = (u8)((col.r * alphaF) + (px.r * oneMinusAlphaF));

	/* Green channel */
	c.g = (u8)((col.g * alphaF) + (px.g * oneMinusAlphaF));

	/* Blue channel */
	c.b = (u8)((col.b * alphaF) + (px.b * oneMinusAlphaF));

	/* Alpha channel */
	c.a = (u8)((col.a * alphaF) + (px.a * oneMinusAlphaF));

	return c;
}
