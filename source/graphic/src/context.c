#include "source/graphic/inc/context.h"

#include "source/utility/inc/alarm.h"

#include "source/core/inc/predefined.h"

#include "source/system/inc/system.h"

#include <stdlib.h>


WUT_INTERN void GLAPIENTRY gl_callback(GLenum source,
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
WUT_INTERN s8 ctx_load_predef(struct wut_Context *ctx)
{

        ctx->def_scrollbar_shader = wut_CreateShader(
                        ctx,
                        "__def_scrollbar_shader",
                        (const char *)_wut_prScrollbarVtxShd,
                        (const char *)_wut_prScrollbarFragShd
                        );

	ctx->def_block_shader = wut_CreateShader(
			ctx, 
			"__def_block_shader", 
			(const char *)_wut_prBlockVtxShd,
			(const char *)_wut_prBlockFragShd
			);

	ctx->def_texture_shader = wut_CreateShader(
			ctx,
			"__def_texture_shader",
			(const char *)_wut_prTextureVtxShd,
			(const char *)_wut_prTextureFragShd
			);

	ctx->def_text_shader = wut_CreateShader(
			ctx,
			"__def_text_shader",
			(const char *)_wut_prTextVtxShd,
			(const char *)_wut_prTextFragShd
			);

	return 0;
}


WUT_INTERN s8 ctx_cfnc_render_batch(void *ptr,  s16 idx, void *p)
{
	struct wut_Batch *ren = (struct wut_Batch *)(*(long *)ptr);

	WUT_IGNORE(idx);
	WUT_IGNORE(p);

	wut_bat_flush(ren);

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_API struct wut_Context *wut_CreateContext(struct wut_Window *win)
{
	struct wut_Context *ctx;

	if(!(ctx = wut_malloc(sizeof(struct wut_Context))))
		goto err_return;

	/* Copy the window reference */
	ctx->window = win;
	
	/* Set the reference to the window shape */
	ctx->shape_ref = &win->shape;

	/*
	 * Create and initialize the resource tables.
	 */
	if(wut_InitShaderTable(ctx) < 0) goto err_free_ctx;
	if(wut_InitTextureTable(ctx) < 0) goto err_close_shd;
	if(wut_InitFontTable(ctx) < 0) goto err_close_tex;
	if(wut_InitObjectTable(ctx) < 0) goto err_close_font;

	/*
	 * Initialize the batch list.
	 */
	if(!(ctx->batches = wut_CreateStatList(sizeof(struct wut_Batch *), 10)))
		goto err_close_obj;

	printf("Create OpenGL context!\n");

	/*
	 * Create the underlying OpenGL-context.
	 */
	if(!(ctx->gl_context = SDL_GL_CreateContext(win->handle))) {
		WUT_ALARM(WUT_ERROR, "Failed to create GL context");
		goto err_destroy_batches;
	}

	/*
	 * Load the predefined resources.
	 */
	if(ctx_load_predef(ctx) < 0)
		goto err_delete_context;

	glClearColor(1, 1, 1, 1);

	/*
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	*/

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_callback, 0);

	return ctx;

err_delete_context:
	SDL_GL_DeleteContext(ctx->gl_context);

err_destroy_batches:
	wut_DestroyStatList(ctx->batches);

err_close_obj:
	wut_CloseObjectTable(ctx);

err_close_font:
	wut_CloseFontTable(ctx);

err_close_tex:
	wut_CloseTextureTable(ctx);

err_close_shd:
	wut_CloseShaderTable(ctx);

err_free_ctx:
	wut_free(ctx);
	
err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create context");
	return NULL;
}


WUT_API void wut_DestroyContext(struct wut_Context *ctx)
{
	if(!ctx) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	/*
	 * Destroy the batch list.
	 */
	wut_DestroyStatList(ctx->batches);

	/* 
	 * Second, close the resource tables.
	 */
	wut_CloseObjectTable(ctx);
	wut_CloseFontTable(ctx);
	wut_CloseTextureTable(ctx);
	wut_CloseShaderTable(ctx);

	wut_free(ctx);
}


WUT_API s8 wut_ContextAdd(struct wut_Context *ctx, enum wut_eContextTable opt, 
		char *name, u32 size, void **p)
{
	struct wut_Table *tbl;

	if(!ctx || !name || !p) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		goto err_return;
	}

	switch(opt) {
		case WUT_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case WUT_CONTEXT_OBJECTS: tbl = ctx->objects; break;
		default: WUT_ALARM(WUT_ERROR, "Table not found"); goto err_return;
	}

	if(wut_AddTable(tbl, name, size, p) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to insert element into table");
		goto err_return;
	}

	return 0;

err_return:
	return -1;
}


WUT_API void wut_ContextRemove(struct wut_Context *ctx, enum wut_eContextTable opt,
		char *name)
{
	struct wut_Table *tbl;

	if(!ctx || !name) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	switch(opt) {
		case WUT_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case WUT_CONTEXT_OBJECTS: tbl = ctx->objects; break;
		default: WUT_ALARM(WUT_WARNING, "Table not found"); return;
	}

	wut_RemoveTable(tbl, name);
}


WUT_API s16 wut_ContextAddBatch(struct wut_Context *ctx, struct wut_Batch **ren)
{
	if(!ctx || !ren) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return -1;
	}

	return wut_AddStatList(ctx->batches, ren);
}


WUT_API void wut_ContextRemoveBatch(struct wut_Context *ctx, s16 idx)
{
	if(!ctx)
		return;

	wut_RemoveStatList(ctx->batches, idx);
}


WUT_API struct wut_Batch *wut_ContextGetBatch(struct wut_Context *ctx, s16 id)
{
	struct wut_Batch *ren;

	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return NULL;
	}

	if(wut_GetStatList(ctx->batches, id, &ren) != 1) {
		printf("Failed to get render batch\n");
		return NULL;
	}

	return ren;
}


WUT_API void wut_ContextRenderBatches(struct wut_Context *ctx)
{
	if(!ctx)
		return;

	wut_ApplyStatList(ctx->batches, &ctx_cfnc_render_batch, NULL);		
}


WUT_API void wut_SetViewport(struct wut_Context *ctx, wut_iRect rect)
{
	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return;
	}

	glViewport(rect[0], rect[1], rect[2], rect[3]);
}


WUT_API void wut_ResetViewport(struct wut_Context *ctx)
{
	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return;
	}

	glViewport(
			(*ctx->shape_ref)[0], 
			(*ctx->shape_ref)[1], 
			(*ctx->shape_ref)[2], 
			(*ctx->shape_ref)[3]
		);
}


WUT_API void wut_ContextEnableScissor(struct wut_Context *ctx, wut_iRect rect)
{
	if(!ctx)
		return;

	glEnable(GL_SCISSOR_TEST);
	glScissor(rect[0], rect[1], rect[2], rect[3]);

}


WUT_API void wut_ContextDisableScissor(struct wut_Context *ctx)
{
	if(!ctx)
		return;

	glDisable(GL_SCISSOR_TEST);
}
