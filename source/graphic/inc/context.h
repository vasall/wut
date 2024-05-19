#ifndef _WT_GRAPHIC_CONTEXT_H
#define _WT_GRAPHIC_CONTEXT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "graphic/inc/graphic.h"
#include "graphic/inc/batch.h"

#include "widget/inc/view.h"

#include "utility/inc/static_list.h"

struct wt_context {
	/* Pointer to the window this context belongs to */
	struct wt_window *window;

	/* Pointer to the underlying OpenGL-context */
	void *gl_context;	
	
	/* A reference to the size of the window */
	struct wt_rect *shape_ref;

	/*
	 * RESOURCE-TABLES
	 */
	struct wt_table *shaders;
	struct wt_statlist *textures;
	struct wt_statlist *fonts;
	struct wt_table *objects;

	/*
	 * A list of all active batch renderers.
	 */
	struct wt_statlist 	*batches;

	/*
	 * Predefined resources.
	 */
	struct wt_shader	*def_block_shader;
	struct wt_shader	*def_texture_shader;
	struct wt_shader	*def_text_shader;
};


enum wt_context_table {
	WT_CONTEXT_SHADERS,
	WT_CONTEXT_TEXTURES,
	WT_CONTEXT_FONTS,
	WT_CONTEXT_OBJECTS
};


/*
 * Create a new OpenGL context, which can be used for standalone rendering.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the newly created context or NULL if an error occurred
 */
WT_API struct wt_context *wt_CreateContext(struct wt_window *win);


/*
 * Destroy a context and free the allocated memory.
 *
 * @ctx: Pointer to the context
 */
WT_API void wt_DestroyContext(struct wt_context *ctx);


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
WT_API s8 wt_ContextAdd(struct wt_context *ctx, enum wt_context_table opt, 
		char *name, u32 size, void **p);


/*
 * Remove an element from one of the elements.
 *
 * @ctx: Pointer to the context
 * @opt: A flag indicating the table to select
 * @name: The name of the entry to remove
 */
WT_API void wt_ContextRemove(struct wt_context *ctx, enum wt_context_table opt,
		char *name);


/*
 * Add a new batch to the context.
 *
 * @ctx: Pointer to the context
 * @ren: A pointer to the pointer of a batch renderer :3
 *
 * Returns: The id of the batch in the context or -1 if an error occurred
 */
WT_API s16 wt_ContextAddBatch(struct wt_context *ctx, struct wt_batch **ren);


/*
 * Remove and destroy a batch renderer.
 *
 * @ctx: Pointer to the context
 * @id: The id of the batch renderer
 */
WT_API void wt_ContextRmvBatch(struct wt_context *ctx, s16 id);


/*
 * Get a batch renderer from the context.
 *
 * @ctx: Pointer to the context
 * @id: The id of the batch renderer
 *
 * Returns: Either a pointer to the batch renderer or NULL if an error occurred
 */
WT_API struct wt_batch *wt_ContextGetBatch(struct wt_context *ctx, s16 id);


/*
 * Flush all batch renderers.
 *
 * @ctx: Pointer to the context
 */
WT_API void wt_ContextRenderBatches(struct wt_context *ctx);


/*
 * Set the rendering area in the window to the given rectangle.
 *
 * @ctx: Pointer to the context
 * @rect: The rendering area
 */
WT_API void wt_SetViewport(struct wt_context *ctx, struct wt_rect *rect);


/*
 * Reset the rendering area.
 *
 * @ctx: Pointer to the context
 */
WT_API void wt_ResetViewport(struct wt_context *ctx);


/*
 * Enable the scissor function to only allow rendering in the specified
 * rectangle.
 *
 * @ctx: Pointer to the context
 * @rect: The rectangle in pixels
 */
WT_API void wt_ContextEnableScissor(struct wt_context *ctx, struct wt_rect *rect);


/*
 * Disable the scissor function.
 *
 * @ctx: Pointer to the context
 */
WT_API void wt_ContextDisableScissor(struct wt_context *ctx);



#endif /* _WT_DOCUMENT_CONTEXT_H */
