#include "context.h"

#include "predef_resources.h"

#include <stdlib.h>


static void GLAPIENTRY gl_callback(GLenum source,
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
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_INTERN s8 ctx_load_predef(struct fh_context *ctx)
{
	u8 i;
	char **p;

	/* 				SHADERS 			      */
	for(i = 0; i < fh_ps_num; i++) {
		p = fh_ps_lst[i];
		if(!fh_LoadShader(ctx, p[0], p[1], p[2]))
			goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to load predefined resources");
	return -1;
}


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
	if(fh_InitCameraTable(ctx) < 0) goto err_close_font;
	if(fh_InitModelTable(ctx) < 0) goto err_close_cam;


	/*
	 * Create the underlying OpenGL-context.
	 */
	if(!(ctx->gl_context = SDL_GL_CreateContext(win->handle))) {
		ALARM(ALARM_ERR, "Failed to create GL context");
		goto err_return;
	}

	/*
	 * Load the predefined resources.
	 */
	if(ctx_load_predef(ctx) < 0)
		goto err_close_mdl;


	glClearColor(1, 1, 1, 1.0);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_callback, 0);



	return ctx;

err_close_mdl:
	fh_CloseModelTable(ctx);

err_close_cam:
	fh_CloseCameraTable(ctx);

err_close_font:
	fh_CloseFontTable(ctx);

err_close_tex:
	fh_CloseTextureTable(ctx);

err_close_shd:
	fh_CloseShaderTable(ctx);

err_free_ctx:
	fh_free(ctx);
	
err_return:
	ALARM(ALARM_ERR, "Failed to create context");
	return NULL;
}


FH_API void fh_DestroyContext(struct fh_context *ctx)
{
	if(!ctx) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* 
	 * First close the resource tables
	 */
	fh_CloseModelTable(ctx);
	fh_CloseCameraTable(ctx);
	fh_CloseFontTable(ctx);
	fh_CloseTextureTable(ctx);
	fh_CloseShaderTable(ctx);
}


FH_API s8 fh_ContextAdd(struct fh_context *ctx, enum fh_context_table opt, 
		char *name, u32 size, void **p)
{
	struct fh_table *tbl;

	if(!ctx || !name || !p) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	switch(opt) {
		case FH_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case FH_CONTEXT_TEXTURES: tbl = ctx->textures; break;
		case FH_CONTEXT_FONTS: tbl = ctx->fonts; break;
		case FH_CONTEXT_CAMERAS: tbl = ctx->cameras; break;
		case FH_CONTEXT_MODELS: tbl = ctx->models; break;
		default: ALARM(ALARM_ERR, "Table not found"); goto err_return;
	}

	if(fh_tbl_add(tbl, name, size, p) < 0)
		goto err_return;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert element into table");
	return -1;
}


FH_API void fh_ContextRemove(struct fh_context *ctx, enum fh_context_table opt,
		char *name)
{
	struct fh_table *tbl;

	if(!ctx || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	switch(opt) {
		case FH_CONTEXT_SHADERS: tbl = ctx->shaders; break;
		case FH_CONTEXT_TEXTURES: tbl = ctx->textures; break;
		case FH_CONTEXT_FONTS: tbl = ctx->fonts; break;
		case FH_CONTEXT_CAMERAS: tbl = ctx->cameras; break;
		case FH_CONTEXT_MODELS: tbl = ctx->models; break;
		default: ALARM(ALARM_WARN, "Table not found"); return;
	}

	fh_tbl_rmv(tbl, name);
}


FH_API void fh_SetViewport(struct fh_context *ctx, struct fh_rect *rect)
{
	if(!ctx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	glViewport(rect->x, rect->y, rect->w, rect->h);
}


FH_API void fh_ResetViewport(struct fh_context *ctx)
{
	if(!ctx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
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
