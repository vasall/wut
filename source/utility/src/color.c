#include "utility/inc/color.h"

#include "utility/inc/alarm.h"

#include <stdlib.h>


WUT_API struct wut_Color wut_SetColor(u8 red, u8 green, u8 blue, u8 alpha)
{
        struct wut_Color col;
        col.r = red;
        col.g = green;
        col.b = blue;
        col.a = alpha;
        return col;
}


WUT_API u32 wut_SetColorU32(u8 red, u8 green, u8 blue, u8 alpha)
{
        struct wut_Color col;
        col.r = red;
        col.g = green;
        col.b = blue;
        col.a = alpha;
        return *((u32 *)(&col));

}


WUT_API u32 wut_GetColorU32(struct wut_Color color)
{
        return *((u32 *)&color);
}


WUT_API void wut_GetColorFV(struct wut_Color color, float *fv)
{
        if(!fv) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        fv[0] = (f32)color.r / 255.0;
        fv[1] = (f32)color.g / 255.0;
        fv[2] = (f32)color.b / 255.0;
        fv[3] = (f32)color.a / 255.0;
}


WUT_API struct wut_Color wut_BlendColor(struct wut_Color px,
                struct wut_Color col)
{
        struct wut_Color c;
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


WUT_API struct wut_Color wut_InvColor(struct wut_Color in)
{
        struct wut_Color out;

        out.r = in.a;
        out.g = in.b;
        out.b = in.g;
        out.a = in.r;

        return out;
}


WUT_API u32 wut_InvColorU32(u32 in)
{
        u8 b[4];

        b[0] = (u8) (in >>  0);
        b[1] = (u8) (in >>  8);
        b[2] = (u8) (in >> 16);
        b[3] = (u8) (in >> 24);

        return *(u32 *)b;
}
