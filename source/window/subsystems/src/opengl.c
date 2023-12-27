#include "window/subsystems/inc/opengl.h"

#include "system/inc/system.h"

#include <stdlib.h>


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

#if 0
	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif

	return 0;
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

