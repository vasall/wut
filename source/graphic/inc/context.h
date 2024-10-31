#ifndef _WUT_GRAPHIC_CONTEXT_H
#define _WUT_GRAPHIC_CONTEXT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "graphic/inc/graphic.h"
#include "graphic/inc/batch.h"

#include "widget/inc/view.h"

#include "utility/inc/static_list.h"

struct wut_Context {
	/* Pointer to the window this context belongs to */
	struct wut_Window *window;

	/* Pointer to the underlying OpenGL-context */
	void *gl_context;	
	
	/* A reference to the size of the window */
	wut_iRect *shape_ref;

	/*
	 * RESOURCE-TABLES
	 */
	struct wut_Table *shaders;
	struct wut_StatList *textures;
	struct wut_StatList *fonts;
	struct wut_Table *objects;

	/*
	 * A list of all active batch renderers.
	 */
	struct wut_StatList 	*batches;

	/*
	 * Predefined resources.
	 */
        struct wut_Shader       *def_scrollbar_shader;
	struct wut_Shader	*def_block_shader;
	struct wut_Shader	*def_texture_shader;
	struct wut_Shader	*def_text_shader;
};


enum wut_eContextTable {
	WUT_CONTEXT_SHADERS,
	WUT_CONTEXT_TEXTURES,
	WUT_CONTEXT_FONTS,
	WUT_CONTEXT_OBJECTS
};


/*
 * Create a new OpenGL context, which can be used for standalone rendering.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the newly created context or NULL if an error occurred
 */
WUT_API struct wut_Context *wut_CreateContext(struct wut_Window *win);


/*
 * Destroy a context and free the allocated memory.
 *
 * @ctx: Pointer to the context
 */
WUT_API void wut_DestroyContext(struct wut_Context *ctx);


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
WUT_API s8 wut_ContextAdd(struct wut_Context *ctx, enum wut_eContextTable opt, 
		char *name, u32 size, void **p);


/*
 * Remove an element from one of the elements.
 *
 * @ctx: Pointer to the context
 * @opt: A flag indicating the table to select
 * @name: The name of the entry to remove
 */
WUT_API void wut_ContextRemove(struct wut_Context *ctx, enum wut_eContextTable opt,
		char *name);


/*
 * Add a new batch to the context.
 *
 * @ctx: Pointer to the context
 * @ren: A pointer to the pointer of a batch renderer :3
 *
 * Returns: The id of the batch in the context or -1 if an error occurred
 */
WUT_API s16 wut_ContextAddBatch(struct wut_Context *ctx, struct wut_Batch **ren);


/*
 * Remove and destroy a batch renderer.
 *
 * @ctx: Pointer to the context
 * @id: The id of the batch renderer
 */
WUT_API void wut_ContextRemoveBatch(struct wut_Context *ctx, s16 id);


/*
 * Get a batch renderer from the context.
 *
 * @ctx: Pointer to the context
 * @id: The id of the batch renderer
 *
 * Returns: Either a pointer to the batch renderer or NULL if an error occurred
 */
WUT_API struct wut_Batch *wut_ContextGetBatch(struct wut_Context *ctx, s16 id);


/*
 * Flush all batch renderers.
 *
 * @ctx: Pointer to the context
 */
WUT_API void wut_ContextRenderBatches(struct wut_Context *ctx);


/*
 * Set the rendering area in the window to the given rectangle.
 *
 * @ctx: Pointer to the context
 * @rect: The rendering area
 */
WUT_API void wut_SetViewport(struct wut_Context *ctx, wut_iRect rect);


/*
 * Reset the rendering area.
 *
 * @ctx: Pointer to the context
 */
WUT_API void wut_ResetViewport(struct wut_Context *ctx);


/*
 * Enable the scissor function to only allow rendering in the specified
 * rectangle.
 *
 * @ctx: Pointer to the context
 * @rect: The rectangle in pixels
 */
WUT_API void wut_ContextEnableScissor(struct wut_Context *ctx, wut_iRect rect);


/*
 * Disable the scissor function.
 *
 * @ctx: Pointer to the context
 */
WUT_API void wut_ContextDisableScissor(struct wut_Context *ctx);



#endif /* _WUT_DOCUMENT_CONTEXT_H */
