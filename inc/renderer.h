#ifndef _FH_RENDERER_H
#define _FH_RENDERER_H

#include "define.h"
#include "import.h"
#include "window.h"
#include "shader.h"
#include "color.h"

struct fh_renderer {
	/* A reference to the GL context */
	struct fh_context *context;

	/* The size of the canvas */
	u32 width;
	u32 height;

	/* The shader used by this renderer */
	struct fh_shader *shader;

	/*  */
	u32 vao;
	u32 bao[2];
	u32 texture;

	/* The surface to draw on */
	SDL_Surface *surface;

	/* The extension data depending on the type */
	void *data;	
};



/*
 * Create a new renderer.
 *
 * @w: The initial width of the canvas
 * @h: The initial height of the canvas
 * @shd: The shader to use for this renderer
 *
 * Returns: Either a pointer to the renderer or NULL if an error occurred
 */
FH_API struct fh_renderer *fh_ren_create(u32 w, u32 h, struct fh_shader *shd);


/*
 * Destroy a renderer and free the allocated memory.
 *
 * @ren: Pointer to the renderer
 */
FH_API void fh_ren_destroy(struct fh_renderer *ren); 


/*
 * Resize the canvas used by the renderer.
 *
 * @ren: Pointer to the renderer
 * @w: The new width
 * @h: The new height
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ren_resize(struct fh_renderer *ren, u32 w, u32 h);


/*
 * Draw on the window.
 *
 * @ren: Pointer to the renderer
 */
FH_API void fh_ren_render(struct fh_renderer *ren, struct fh_window *win, struct fh_context *ctx);


/*
 * Copy from one renderer to another. Note that for this to work, the size of
 * both rectangles has to be equal.
 *
 * @dst: The tagret renderer
 * @rdst: The rectangle to put into the target
 * @src: The source renderer
 * @rsrc: The rectangle to take from the source
 */
FH_API s8 fh_ren_copy(struct fh_renderer *dst, SDL_Rect rdst,
		struct fh_renderer *source, SDL_Rect rsrc);



FH_API void fh_ren_fill_circle(struct fh_renderer *ren, int x, int y,
		int radius, struct fh_color color);


FH_API void fh_ren_rounded_rect(struct fh_renderer *ren, SDL_Rect rect,
		u32 *radii, struct fh_color color);


FH_API void fh_ren_rect(struct fh_renderer *ren, SDL_Rect rect,
		struct fh_color color);



/*
 * Write the surface to the texture.
 *
 * @ren: Pointer to the texture.
 */
FH_API void fh_ren_to_texture(struct fh_renderer *ren);


FH_API void fh_ren_red_texture(struct fh_renderer *ren);

#endif /* _FH_RENDERER_H */
