#include "graphic/inc/context.h"

#include "utility/inc/alarm.h"

#include "core/inc/predefined.h"

#include "system/inc/system.h"

#include <stdlib.h>


WT_INTERN void GLAPIENTRY gl_callback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar *message,
				const void *userParam)
{
	char *type_string;
	char *severity_string;

	(void) source;
	(void) id;
	(void) length;
	(void) userParam;

	if(severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		type_string = "GL ERROR"; 
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		type_string = "GL DEPRECATED"; 
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		type_string = "GL UNDEFINED"; 
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		type_string = "GL PORTABILITY"; 
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		type_string = "GL PERFORMANCE"; 
		break;
	case GL_DEBUG_TYPE_OTHER:
		type_string = "GL MESSAGE"; 
		break;
	case GL_DEBUG_TYPE_MARKER:
		type_string = "GL MARKER"; 
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		type_string = "GL PUSH"; 
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		type_string = "GL POP"; 
		break;
	default:
		type_string = "";
		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		severity_string = "\033[31mHIGH SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		severity_string = "\033[33mMEDIUM SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		severity_string = "\033[36mLOW SEVERITY";
		break;
	default:
		severity_string = "";
		break;
	}

	fprintf(stderr, "%s: %s %s\033[0m\n", severity_string, type_string,
						message);
}

/*
 * Load the predefined resources necessary for the basic functions of the
 * context.
 *
 * TODO: Add error handling
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_INTERN s8 ctx_load_predef(struct wt_context *ctx)
{

	ctx->def_block_shader = wt_CreateShader(
			ctx, 
			"__def_block_shader", 
			(const char *)wt_ps_shd_def_block_v,
			(const char *)wt_ps_shd_def_block_f
			);

	ctx->def_texture_shader = wt_CreateShader(
			ctx,
			"__def_texture_shader",
			(const char *)wt_ps_shd_def_texture_v,
			(const char *)wt_ps_shd_def_texture_f
			);

	printf("Load font shader...\n");
	ctx->def_text_shader = wt_CreateShader(
			ctx,
			"__def_text_shader",
			(const char *)wt_ps_shd_def_text_v,
			(const char *)wt_ps_shd_def_text_f
			);
	printf("done\n");
	return 0;
}


WT_INTERN s8 ctx_cfnc_render_batch(void *ptr,  s16 idx, void *p)
{
	struct wt_batch *ren = (struct wt_batch *)(*(long *)ptr);

	wt_Ignore(idx);
	wt_Ignore(p);

	wt_batch_flush(ren);

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_API struct wt_context *wt_CreateContext(struct wt_window *win)
{
	struct wt_context *ctx;

	if(!(ctx = wt_malloc(sizeof(struct wt_context))))
		goto err_return;

	/* Copy the window reference */
	ctx->window = win;
	
	/* Set the reference to the window shape */
	ctx->shape_ref = &win->shape;

	/*
	 * Create and initialize the resource tables.
	 */
	if(wt_InitShaderTable(ctx) < 0) goto err_free_ctx;
	if(wt_InitTextureTable(ctx) < 0) goto err_close_shd;
	if(wt_InitFontTable(ctx) < 0) goto err_close_tex;
	if(wt_InitObjectTable(ctx) < 0) goto err_close_font;

	/*
	 * Initialize the batch list.
	 */
	if(!(ctx->batches = wt_statlist_create(sizeof(struct wt_batch *), 10)))
		goto err_close_obj;

	/*
	 * Create the underlying OpenGL-context.
	 */
	if(!(ctx->gl_context = SDL_GL_CreateContext(win->handle))) {
		WT_ALARM(WT_ERROR, "Failed to create GL context");
		goto err_destroy_batches;
	}

	/*
	 * Load the predefined resources.
	 */
	if(ctx_load_predef(ctx) < 0)
		goto err_delete_context;

	glClearColor(1, 1, 1, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_callback, 0);

	return ctx;

err_delete_context:
	SDL_GL_DeleteContext(ctx->gl_context);

err_destroy_batches:
	wt_statlist_destroy(ctx->batches);

err_close_obj:
	wt_CloseObjectTable(ctx);

err_close_font:
	wt_CloseFontTable(ctx);

err_close_tex:
	wt_CloseTextureTable(ctx);

err_close_shd:
	wt_CloseShaderTable(ctx);

err_free_ctx:
	wt_free(ctx);
	
err_return:
	WT_ALARM(WT_ERROR, "Failed to create context");
	return NULL;
}


WT_API void wt_DestroyContext(struct wt_context *ctx)
{
	if(!ctx) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	/*
	 * Destroy the batch list.
	 */
	wt_statlist_destroy(ctx->batches);

	/* 
	 * Second, close the resource tables.
	 */
	wt_CloseObjectTable(ctx);
	wt_CloseFontTable(ctx);
	wt_CloseTextureTable(ctx);
	wt_CloseShaderTable(ctx);

	wt_free(ctx);
}


WT_API s8 wt_ContextAdd(struct wt_context *ctx, enum wt_context_table opt, 
		char *name, u32 size, void **p)
{
	struct wt_table *tbl;

	if(!ctx || !name || !p) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		goto err_return;
	}

	switch(opt) {
		case WT_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case WT_CONTEXT_OBJECTS: tbl = ctx->objects; break;
		default: WT_ALARM(WT_ERROR, "Table not found"); goto err_return;
	}

	if(wt_tbl_add(tbl, name, size, p) < 0) {
		WT_ALARM(WT_ERROR, "Failed to insert element into table");
		goto err_return;
	}

	return 0;

err_return:
	return -1;
}


WT_API void wt_ContextRemove(struct wt_context *ctx, enum wt_context_table opt,
		char *name)
{
	struct wt_table *tbl;

	if(!ctx || !name) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	switch(opt) {
		case WT_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case WT_CONTEXT_OBJECTS: tbl = ctx->objects; break;
		default: WT_ALARM(WT_WARNING, "Table not found"); return;
	}

	wt_tbl_rmv(tbl, name);
}


WT_API s16 wt_ContextAddBatch(struct wt_context *ctx, struct wt_batch **ren)
{
	if(!ctx || !ren) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	return wt_statlist_add(ctx->batches, ren);
}


WT_API void wt_ContextRmvBatch(struct wt_context *ctx, s16 idx)
{
	if(!ctx)
		return;

	wt_statlist_rmv(ctx->batches, idx);
}


WT_API struct wt_batch *wt_ContextGetBatch(struct wt_context *ctx, s16 id)
{
	struct wt_batch *ren;

	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return NULL;
	}

	if(wt_statlist_get(ctx->batches, id, &ren) != 1) {
		printf("Failed to get render batch\n");
		return NULL;
	}

	return ren;
}


WT_API void wt_ContextRenderBatches(struct wt_context *ctx)
{
	if(!ctx)
		return;

	wt_statlist_apply(ctx->batches, &ctx_cfnc_render_batch, NULL);		
}


WT_API void wt_SetViewport(struct wt_context *ctx, struct wt_rect *rect)
{
	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return;
	}

	glViewport(rect->x, rect->y, rect->w, rect->h);
}


WT_API void wt_ResetViewport(struct wt_context *ctx)
{
	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return;
	}

	glViewport(
			ctx->shape_ref->x, 
			ctx->shape_ref->y, 
			ctx->shape_ref->w, 
			ctx->shape_ref->h
		);
}


WT_API void wt_ContextEnableScissor(struct wt_context *ctx, struct wt_rect *rect)
{
	if(!ctx)
		return;

	glEnable(GL_SCISSOR_TEST);
	glScissor(rect->x, rect->y, rect->w, rect->h);

}


WT_API void wt_ContextDisableScissor(struct wt_context *ctx)
{
	if(!ctx)
		return;

	glDisable(GL_SCISSOR_TEST);
}
