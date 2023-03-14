#include "renderer.h"

#include "opengl.h"

#include <stdlib.h>





FH_API struct fh_renderer *fh_ren_create(u32 w, u32 h, struct fh_shader *shd)
{
	struct fh_renderer *ren;

	/* Allocate memory for renderer */
	if(!(ren = smalloc(sizeof(struct fh_renderer)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for renderer");
		goto err_return;
	}

	ren->width = w;
	ren->height = h;
	ren->shader = shd;

	/* Create SDL surface */
	if(!(ren->surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0))) {
		ALARM(ALARM_ERR, "Failed to create RGB surface");
		goto err_free_ren;
	}

	return ren;


err_free_ren:
	sfree(ren);

err_return:
	ALARM(ALARM_ERR, "Failed to create renderer");
	return NULL;
}


FH_API void fh_ren_destroy(struct fh_renderer *ren)
{
	if(!ren)
		return;

	/* Destroy the surface */
	SDL_FreeSurface(ren->surface);

	/* Free the data buffer if there is one */
	if(ren->data)
		sfree(ren->data);


	/* Finally free the renderer struct itself */
	sfree(ren);
}


FH_API s8 fh_ren_resize(struct fh_renderer *ren, u32 w, u32 h)
{
	SDL_Surface *newsurf;

	if(!ren) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Check if the surface really has to be resized.
	 */
	if(ren->width == w && ren->height == h) {
		return 0;
	}

	/*
	 * Create a new surface.
	 */
	if(!(newsurf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0))) {
		ALARM(ALARM_ERR, "Failed to create new RGB surface");
		goto err_return;
	}

	/*
	 * Free the old surface.
	 */
	SDL_FreeSurface(ren->surface);

	/*
	 * And replace with the new surface.
	 */
	ren->surface = newsurf;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to resize surface");
	return -1;
}


FH_API void fh_ren_render(struct fh_window *win, struct fh_renderer *ren)
{
	u32 tex;
	SDL_Surface *surf;

	surf = ren->surface;

	/*
	 * Select the context
	 */
	SDL_GL_MakeCurrent(win->handle, win->context->context);

	/*
	 * Create an OpenGL texture from the SDL surface.
	 */
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, surf->pixels);

	/*
	 * Set up the texture parameters.
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	 * Draw the texture onto the context
	 */
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1);
	glTexCoord2f(1, 0); glVertex2f(1, -1);
	glTexCoord2f(1, 1); glVertex2f(1, 1);
	glTexCoord2f(0, 1); glVertex2f(-1, 1);
	glEnd();

	/*
	 * Clean up the texture.
	 */
	glDeleteTextures(1, &tex);	
}


FH_API s8 fh_ren_copy(struct fh_renderer *dst, SDL_Rect rdst, struct fh_renderer *src,
		SDL_Rect rsrc)
{
	if(!dst || !src) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if((SDL_BlitSurface(src->surface, &rsrc, dst->surface, &rdst)) < 0) {
		ALARM(ALARM_ERR, "Failed to blit surface");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to copy from one renderer to another");
	return -1;
}

FH_API void fh_ren_fill_circle(struct fh_renderer *ren, s32 x, s32 y,
		s32 radius, struct fh_color color)
{
	s32 i;
	s32 j;
	s32 rSquared;
	s32 distanceSquared;

	u32 *px;
	u32 off;

	SDL_Surface *surf;

	u32 cval;

	surf = ren->surface;
	cval = fh_color_get(color);

	/* Lock the surface to access pixel data */
	SDL_LockSurface(surf);

	/* Calculate the squared radius to avoid using sqrt() function */
	rSquared = radius * radius;

	/* Iterate over all pixels in the bounding rectangle of the circle */
	for(i = x - radius; i <= x + radius; i++) {
		for(j = y - radius; j <= y + radius; j++) {
			/* Calculate the distance from the current pixel to the center of the circle */
			distanceSquared = (i - x) * (i - x) + (j - y) * (j - y);

			/* If the pixel is within the circle, set its color */
			if(distanceSquared <= rSquared) {
				off = j * surf->pitch + i * surf->format->BytesPerPixel;
				px = (u32 *)((u8 *)surf->pixels + off);
				*px = cval;
			}
		}
	}

	/* Unlock the surface */
	SDL_UnlockSurface(surf);
}

FH_API void fh_ren_rounded_rect(struct fh_renderer *ren, SDL_Rect rect,
		u32 *radii, struct fh_color color)
{
	s32 w;
	s32 h;
	u32 maxRadius;
	s32 i;
	SDL_Surface *surf;

	s32 xSign;
	s32 ySign;
	s32 radius;
	s32 x0;
	s32 y0;
	s32 x1;
	s32 y1;


	s32 dy;
	f32 d;
	s32 x2;
	s32 x3;
	s32 y2;
	s32 y3;

	SDL_Rect mainRect;



	surf = ren->surface;

	w = rect.w;
	h = rect.h;

	maxRadius = (w < h) ? w / 2 : h / 2;

	/* Ensure corner radii are within bounds */
	for (i = 0; i < 4; i++) {
		radii[i] = (radii[i] > maxRadius) ? maxRadius : radii[i];
	}

	/* Draw main rectangle */
	mainRect = rect;
	mainRect.x += radii[0];
	mainRect.y += radii[0];
	mainRect.w -= radii[0] + radii[1];
	mainRect.h -= radii[0] + radii[2];
	SDL_FillRect(surf, &mainRect, fh_color_get(color));

	/* Draw corners */
	for (i = 0; i < 4; i++) {
		if (radii[i] > 0) {
			xSign = (i == 1 || i == 2) ? -1 : 1;
			ySign = (i == 2 || i == 3) ? -1 : 1;
			radius = radii[i];
			x0 = rect.x + radius * xSign;
			y0 = rect.y + radius * ySign;
			x1 = x0;
			y1 = y0 + radius;

			while (x0 <= x1) {
				dy = y1 - y0;
				if (dy <= 0) {
					y1++;
				}

				d = sqrt(radius * radius - dy * dy);
				x2 = (s32) round(x0 + d);
				x3 = (s32) round(x1 + d);
				y2 = (s32) round(y0 + dy);
				y3 = (s32) round(y1 + dy);

				/* Draw quadrants of circle using filledCircleColor function */
				fh_ren_fill_circle(ren, x0, y0, radius, color);
				fh_ren_fill_circle(ren, x1, y0, radius, color);
				fh_ren_fill_circle(ren, x0, y1, radius, color);
				fh_ren_fill_circle(ren, x1, y1, radius, color);

				x0++;
				y0 = y2;
				x1--;
				y1 = y3;
			}
		}
	}
}


FH_API void fh_ren_rect(struct fh_renderer *ren, SDL_Rect rect,
		struct fh_color color)
{
	if(!ren) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	SDL_FillRect(ren->surface, &rect, fh_color_get(color));
}
