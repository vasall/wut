#include "renderer.h"

#include "alarm.h"
#include "system.h"
#include "opengl.h"

#include <stdlib.h>



const float FH_STANDARD_CORNERS[18] = {
	1,  1,  0,
	-1,  1,  0, 
	-1, -1,  0,
	-1, -1,  0,  
	1, -1,  0,  
	1,  1,  0
};

const float FH_STANDARD_UV[12] = {
	1,  0,
	0,  0,  
	0,  1,
	0,  1,  
	1,  1,  
	1,  0
};


FH_INTERN s8 fh_ren_init_buffers(struct fh_renderer *ren)
{
	u16 tmp;

	/* Generate a new VAO and bind it */
	glGenVertexArrays(1, &ren->vao);
	glBindVertexArray(ren->vao);

	/*
	 * Create two buffers to store the vertex positions and the
	 * texture-coordinates.
	 */
	glGenBuffers(2, ren->bao);

	/* Write the vertex-positions */
	glBindBuffer(GL_ARRAY_BUFFER, ren->bao[0]);
	tmp = 18 * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, tmp, FH_STANDARD_CORNERS, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/* Write the texture-coordinates */
	glBindBuffer(GL_ARRAY_BUFFER, ren->bao[1]);
	tmp = 12 * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, tmp, FH_STANDARD_UV, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	/* Unbind the VAO */
	glBindVertexArray(0);

	return 0;
}


FH_API struct fh_renderer *fh_ren_create(u32 w, u32 h, struct fh_shader *shd)
{
	struct fh_renderer *ren;

	/* Allocate memory for renderer */
	if(!(ren = fh_malloc(sizeof(struct fh_renderer)))) {
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



	/* Create the GL Texture */
	glGenTextures(1, &ren->texture);
	glBindTexture(GL_TEXTURE_2D, ren->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, ren->surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	return ren;

err_free_surface:
	SDL_FreeSurface(ren->surface);

err_free_ren:
	fh_free(ren);

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
		fh_free(ren->data);


	/* Finally free the renderer struct itself */
	fh_free(ren);
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

	/* TODO: RESIZE TEXTURE */

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to resize surface");
	return -1;
}


FH_API void fh_ren_render(struct fh_renderer *ren, struct fh_window *win, struct fh_context *ctx)
{
	static u8 flg = 0;
	u32 tmp;

	u32 vao;

	u32 vbo;

	u32 loc_position;

	u32 shaderProgram;

	f32 vertices[24] = {
  		0.0, 0.0,
  		0.5, 0.0,
  		0.5, 0.5,
  
  		0.0, 0.0,
  		0.0, 0.5,
  		-0.5, 0.5,
  
 		0.0, 0.0,
 		-0.5, 0.0,
 		-0.5, -0.5,
 
 		0.0, 0.0,
 		0.0, -0.5,
 		0.5, -0.5,
 	};

	if(!ren || !win || !ctx)
		return;


	if(flg != 0)
		return;

	printf("Renderer %p\n", ren);
	printf("Shader: %s\n", ren->shader->name);

	flg = 1;

	shaderProgram = ren->shader->program;


	/* Select context for this window */
	SDL_GL_MakeCurrent(win->handle, win->context);

	/* Clear the window screen */
        glClear(GL_COLOR_BUFFER_BIT);




	/* Create a vertex-array-object (short VAO) */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Create a Vector Buffer Object that will store the vertices on video memory */
	glGenBuffers(1, &vbo);

	/* Allocate space and upload the data from CPU to GPU */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	tmp = sizeof(vertices);
	glBufferData(GL_ARRAY_BUFFER, tmp, vertices, GL_STATIC_DRAW);

	fh_shd_use(ren->shader);

	/* Get the location of the attributes that enters in the vertex shader */
	loc_position = glGetAttribLocation(shaderProgram, "position");

	/* Specify how the data for position can be accessed */
	glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, 0, 0);

	/* Enable the attribute */
	glEnableVertexAttribArray(loc_position);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	/* Swap buffer */
        SDL_GL_SwapWindow(win->handle);
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
	u32 colcode;

	if(!ren) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}


	colcode = fh_color_get(color);
	printf("[%d, %d, %d, %d]: %08x\n", rect.x, rect.y, rect.w, rect.h, colcode);
	SDL_FillRect(ren->surface, &rect, colcode);
}

FH_INTERN void fh_print_surface(SDL_Surface *surf) {
	int i, j;
	Uint32 pixel;
	Uint8 r, g, b, a;

	/* Loop through each pixel in the surface */
	for (i = 0; i < surf->h; i++) {
		for (j = 0; j < surf->w; j++) {
			/* Get the pixel value */
			pixel = ((u32 *)surf->pixels)[i * surf->w + j];

			/* Extract the RGBA components */
			SDL_GetRGBA(pixel, surf->format, &r, &g, &b, &a);

			/* Print the pixel value to the console */
			printf("(%d, %d): %d, %d, %d, %d\n", j, i, r, g, b, a);
		}
	}
}



FH_API void fh_ren_to_texture(struct fh_renderer *ren)
{
	SDL_Surface *surf;

	static int c = 0;

	if(!ren) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}


	/* Copy the suface onto the texture */
	surf = ren->surface;
	glBindTexture(GL_TEXTURE_2D, ren->texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surf->w, surf->h,
			GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);

	if(c == 0) {
		c = 1;

		printf("C!!\n");
		SDL_SaveBMP(ren->surface, "out.bmp");
	}
}

FH_API void fh_ren_red_texture(struct fh_renderer *ren)
{
	GLuint texture;
	u32 width;
	u32 height;

	width = 100;
	height = 100;

	/* Generate texture */
	glGenTextures(1, &texture);

	/* Bind texture */
	glBindTexture(GL_TEXTURE_2D, texture);

	/* Set texture parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/* Create texture data */
	GLubyte* data = (GLubyte*)malloc(width * height * 4);
	memset(data, 255, width * height * 4);

	/* Upload texture data */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	/* Free texture data */
	free(data);

	ren->texture = texture;
}
