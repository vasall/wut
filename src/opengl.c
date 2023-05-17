#include "opengl.h"

#include "alarm.h"
#include "system.h"

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


FH_API s8 fh_gl_init(void)
{
	/* 
	 * Request opengl 4.6 context.
	 * SDL doesn't have the ability to choose which profile at this time of writing,
	 * but it should default to the core profile.
	 */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#if 1
	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif

	return 0;
}


FH_API struct fh_context *fh_gl_create(struct fh_window *win)
{
	struct fh_context *ctx;

	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Allocate memory for the new context */
	if(!(ctx = fh_malloc(sizeof(struct fh_context)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for new glcontext");
		goto err_return;
	}

	if(!(ctx->gl_context = SDL_GL_CreateContext(win->handle))) {
		ALARM(ALARM_ERR, "Failed to create GL context");
		goto err_return;
	}


	glClearColor(1, 1, 1, 1.0);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_callback, 0);

	return ctx;

err_return:
	ALARM(ALARM_ERR, "Failed to create GL context");
	return NULL;
}


FH_API void fh_gl_destroy(struct fh_context *ctx)
{
	if(!ctx) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}


	SDL_GL_DeleteContext(ctx->gl_context);

	fh_free(ctx);
}

FH_API void fh_gl_print_info(void)
{
	const unsigned char *ver;
	const unsigned char *vendor;
	const unsigned char *gpu;
	const unsigned char *glsl_ver;
	
	ver = glGetString(GL_VERSION);
	vendor = glGetString(GL_VENDOR);
	gpu = glGetString(GL_RENDERER);
	glsl_ver = glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("------------------ OpenGL Info -------------------\n");
	printf("GL Version: %s\n", ver);
	printf("Vendor: %s\n", vendor);
	printf("GPU: %s\n", gpu);
	printf("GLSL Version: %s\n", glsl_ver);
	printf("--------------------------------------------------\n");
}

