#include "graphic/inc/context.h"

#include "utility/alarm/inc/alarm.h"

#include "core/inc/predefined.h"

#include "system/inc/system.h"

#include <stdlib.h>


FH_INTERN void GLAPIENTRY gl_callback(GLenum source,
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
FH_INTERN s8 ctx_load_predef(struct fh_context *ctx)
{

	ctx->def_block_shader = fh_CreateShader(
			ctx, 
			"__def_block_shader", 
			(const char *)fh_ps_shd_def_block_v,
			(const char *)fh_ps_shd_def_block_f
			);

	ctx->def_texture_shader = fh_CreateShader(
			ctx,
			"__def_texture_shader",
			(const char *)fh_ps_shd_def_texture_v,
			(const char *)fh_ps_shd_def_texture_f
			);

	printf("Load font shader...\n");
	ctx->def_font_shader = fh_CreateShader(
			ctx,
			"__def_font_shader",
			(const char *)fh_ps_shd_def_font_v,
			(const char *)fh_ps_shd_def_font_f
			);
	printf("done\n");
	return 0;
}


FH_INTERN s8 ctx_cfnc_render_batch(void *ptr,  s16 idx, void *p)
{
	struct fh_batch *ren = (struct fh_batch *)(*(long *)ptr);

	fh_Ignore(idx);
	fh_Ignore(p);

	fh_batch_flush(ren);

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_context *fh_CreateContext(struct fh_window *win)
{
	struct fh_context *ctx;

	if(!(ctx = fh_malloc(sizeof(struct fh_context))))
		goto err_return;

	/* Copy the window reference */
	ctx->window = win;
	
	/* Set the reference to the window shape */
	ctx->shape_ref = &win->shape;

	/*
	 * Create and initialize the resource tables.
	 */
	if(fh_InitShaderTable(ctx) < 0) goto err_free_ctx;
	if(fh_InitTextureTable(ctx) < 0) goto err_close_shd;
	if(fh_InitFontTable(ctx) < 0) goto err_close_tex;
	if(fh_InitObjectTable(ctx) < 0) goto err_close_font;

	/*
	 * Initialize the batch list.
	 */
	if(!(ctx->batches = fh_statlist_create(sizeof(struct fh_batch *), 10)))
		goto err_close_obj;

	/*
	 * Create the underlying OpenGL-context.
	 */
	if(!(ctx->gl_context = SDL_GL_CreateContext(win->handle))) {
		FH_ALARM(FH_ERROR, "Failed to create GL context");
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
	fh_statlist_destroy(ctx->batches);

err_close_obj:
	fh_CloseObjectTable(ctx);

err_close_font:
	fh_CloseFontTable(ctx);

err_close_tex:
	fh_CloseTextureTable(ctx);

err_close_shd:
	fh_CloseShaderTable(ctx);

err_free_ctx:
	fh_free(ctx);
	
err_return:
	FH_ALARM(FH_ERROR, "Failed to create context");
	return NULL;
}


FH_API void fh_DestroyContext(struct fh_context *ctx)
{
	if(!ctx) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	/*
	 * Destroy the batch list.
	 */
	fh_statlist_destroy(ctx->batches);

	/* 
	 * Second, close the resource tables.
	 */
	fh_CloseObjectTable(ctx);
	fh_CloseFontTable(ctx);
	fh_CloseTextureTable(ctx);
	fh_CloseShaderTable(ctx);

	fh_free(ctx);
}


FH_API s8 fh_ContextAdd(struct fh_context *ctx, enum fh_context_table opt, 
		char *name, u32 size, void **p)
{
	struct fh_table *tbl;

	if(!ctx || !name || !p) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		goto err_return;
	}

	switch(opt) {
		case FH_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case FH_CONTEXT_OBJECTS: tbl = ctx->objects; break;
		default: FH_ALARM(FH_ERROR, "Table not found"); goto err_return;
	}

	if(fh_tbl_add(tbl, name, size, p) < 0) {
		FH_ALARM(FH_ERROR, "Failed to insert element into table");
		goto err_return;
	}

	return 0;

err_return:
	return -1;
}


FH_API void fh_ContextRemove(struct fh_context *ctx, enum fh_context_table opt,
		char *name)
{
	struct fh_table *tbl;

	if(!ctx || !name) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	switch(opt) {
		case FH_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case FH_CONTEXT_OBJECTS: tbl = ctx->objects; break;
		default: FH_ALARM(FH_WARNING, "Table not found"); return;
	}

	fh_tbl_rmv(tbl, name);
}


FH_API s16 fh_ContextAddBatch(struct fh_context *ctx, struct fh_batch **ren)
{
	if(!ctx || !ren) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	return fh_statlist_add(ctx->batches, ren);
}


FH_API void fh_ContextRmvBatch(struct fh_context *ctx, s16 idx)
{
	if(!ctx)
		return;

	fh_statlist_rmv(ctx->batches, idx);
}


FH_API struct fh_batch *fh_ContextGetBatch(struct fh_context *ctx, s16 id)
{
	struct fh_batch *ren;

	if(!ctx) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return NULL;
	}

	if(fh_statlist_get(ctx->batches, id, &ren) != 1) {
		printf("Failed to get render batch\n");
		return NULL;
	}

	return ren;
}


FH_API void fh_ContextRenderBatches(struct fh_context *ctx)
{
	if(!ctx)
		return;

	fh_statlist_apply(ctx->batches, &ctx_cfnc_render_batch, NULL);		
}


FH_API void fh_SetViewport(struct fh_context *ctx, struct fh_rect *rect)
{
	if(!ctx) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return;
	}

	glViewport(rect->x, rect->y, rect->w, rect->h);
}


FH_API void fh_ResetViewport(struct fh_context *ctx)
{
	if(!ctx) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return;
	}

	glViewport(
			ctx->shape_ref->x, 
			ctx->shape_ref->y, 
			ctx->shape_ref->w, 
			ctx->shape_ref->h
		);
}


FH_API void fh_ContextEnableScissor(struct fh_context *ctx, struct fh_rect *rect)
{
	if(!ctx)
		return;

	glEnable(GL_SCISSOR_TEST);
	glScissor(rect->x, rect->y, rect->w, rect->h);

}


FH_API void fh_ContextDisableScissor(struct fh_context *ctx)
{
	if(!ctx)
		return;

	glDisable(GL_SCISSOR_TEST);
}
