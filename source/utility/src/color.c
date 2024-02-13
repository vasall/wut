#include "utility/inc/color.h"

#include "utility/inc/alarm.h"

#include <stdlib.h>


FH_API struct fh_color fh_color_set(u8 red, u8 green, u8 blue, u8 alpha)
{
	struct fh_color col;
	col.r = red;
	col.g = green;
	col.b = blue;
	col.a = alpha;
	return col;
}


FH_API u32 fh_color_set_u32(u8 red, u8 green, u8 blue, u8 alpha)
{
	struct fh_color col;
	col.r = red;
	col.g = green;
	col.b = blue;
	col.a = alpha;
	return *((u32 *)(&col));

}


FH_API u32 fh_color_get(struct fh_color color)
{
	return *((u32 *)&color);
}


FH_API void fh_color_get_fv(struct fh_color color, float *fv)
{
	if(!fv) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fv[0] = (f32)color.r / 255.0;
	fv[1] = (f32)color.g / 255.0;
	fv[2] = (f32)color.b / 255.0;
	fv[3] = (f32)color.a / 255.0;
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


FH_API struct fh_color fh_col_conv_itos(u32 v)
{
	return *(struct fh_color *)&v;
}


FH_API struct fh_color fh_col_invform(struct fh_color in)
{
	struct fh_color out;

	out.r = in.a;
	out.g = in.b;
	out.b = in.g;
	out.a = in.r;

	return out;
}


FH_API u32 fh_col_invform_s32(u32 in)
{
	u8 b[4];

	b[0] = (u8) (in >>  0);
	b[1] = (u8) (in >>  8);
	b[2] = (u8) (in >> 16);
	b[3] = (u8) (in >> 24);

	return *(u32 *)b;
}
