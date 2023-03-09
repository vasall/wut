#include "opengl.h"

#include <stdlib.h>


XWIN_API s8 xwin_gl_init(void)
{
	/* 
	 * Request opengl 4.2 context.
	 * SDL doesn't have the ability to choose which profile at this time of writing,
	 * but it should default to the core profile.
	 */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	return 0;
}


XWIN_API s8 xwin_gl_create(struct xwin_window *win)
{
	SDL_GLContext ctx;

	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}


	if(!(ctx = SDL_GL_CreateContext(win->handle))) {
		ALARM(ALARM_ERR, "Failed to create GL context");
		goto err_return;
	}


	glClearColor(0.094, 0.094, 0.094, 1.0);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);


	win->context = ctx;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to create GL context");
	return -1;
}


XWIN_API void xwin_gl_destroy(struct xwin_window *win)
{
	if(!win) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}


	SDL_GL_DeleteContext(win->context);
}
