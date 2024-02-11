#ifndef _FH_GRAPHIC_CONTEXT_H
#define _FH_GRAPHIC_CONTEXT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "graphic/inc/graphic.h"
#include "graphic/inc/batch.h"

#include "widget/inc/view.h"

#include "utility/inc/static_list.h"

struct fh_context {
	/* Pointer to the window this context belongs to */
	struct fh_window *window;

	/* Pointer to the underlying OpenGL-context */
	void *gl_context;	
	
	/* A reference to the size of the window */
	struct fh_rect *shape_ref;

	/*
	 * RESOURCE-TABLES
	 */
	struct fh_table *shaders;
	struct fh_statlist *textures;
	struct fh_statlist *fonts;
	struct fh_table *objects;

	/*
	 * A list of all active batch renderers.
	 */
	struct fh_statlist 	*batches;

	/*
	 * Predefined resources.
	 */
	struct fh_shader	*def_block_shader;
	struct fh_shader	*def_texture_shader;
	struct fh_shader	*def_font_shader;
};


enum fh_context_table {
	FH_CONTEXT_SHADERS,
	FH_CONTEXT_TEXTURES,
	FH_CONTEXT_FONTS,
	FH_CONTEXT_OBJECTS
};


/*
 * Create a new OpenGL context, which can be used for standalone rendering.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the newly created context or NULL if an error occurred
 */
FH_API struct fh_context *fh_CreateContext(struct fh_window *win);


/*
 * Destroy a context and free the allocated memory.
 *
 * @ctx: Pointer to the context
 */
FH_API void fh_DestroyContext(struct fh_context *ctx);


/*
 * Insert a resource element into one of the tables.
 *
 * @ctx: Pointer to the context
 * @opt: A flag indicating the table to use
 * @name: The name of the entry
 * @size: The number of bytes used by the element
 * @p: A pointer to the element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ContextAdd(struct fh_context *ctx, enum fh_context_table opt, 
		char *name, u32 size, void **p);


/*
 * Remove an element from one of the elements.
 *
 * @ctx: Pointer to the context
 * @opt: A flag indicating the table to select
 * @name: The name of the entry to remove
 */
FH_API void fh_ContextRemove(struct fh_context *ctx, enum fh_context_table opt,
		char *name);


/*
 * Add a new batch to the context.
 *
 * @ctx: Pointer to the context
 * @ren: A pointer to the pointer of a batch renderer :3
 *
 * Returns: The id of the batch in the context or -1 if an error occurred
 */
FH_API s16 fh_ContextAddBatch(struct fh_context *ctx, struct fh_batch **ren);


/*
 * Remove and destroy a batch renderer.
 *
 * @ctx: Pointer to the context
 * @id: The id of the batch renderer
 */
FH_API void fh_ContextRmvBatch(struct fh_context *ctx, s16 id);


/*
 * Get a batch renderer from the context.
 *
 * @ctx: Pointer to the context
 * @id: The id of the batch renderer
 *
 * Returns: Either a pointer to the batch renderer or NULL if an error occurred
 */
FH_API struct fh_batch *fh_ContextGetBatch(struct fh_context *ctx, s16 id);


/*
 * Flush all batch renderers.
 *
 * @ctx: Pointer to the context
 */
FH_API void fh_ContextRenderBatches(struct fh_context *ctx);


/*
 * Set the rendering area in the window to the given rectangle.
 *
 * @ctx: Pointer to the context
 * @rect: The rendering area
 */
FH_API void fh_SetViewport(struct fh_context *ctx, struct fh_rect *rect);


/*
 * Reset the rendering area.
 *
 * @ctx: Pointer to the context
 */
FH_API void fh_ResetViewport(struct fh_context *ctx);


/*
 * Enable the scissor function to only allow rendering in the specified
 * rectangle.
 *
 * @ctx: Pointer to the context
 * @rect: The rectangle in pixels
 */
FH_API void fh_ContextEnableScissor(struct fh_context *ctx, struct fh_rect *rect);


/*
 * Disable the scissor function.
 *
 * @ctx: Pointer to the context
 */
FH_API void fh_ContextDisableScissor(struct fh_context *ctx);



#endif /* _FH_DOCUMENT_CONTEXT_H */
