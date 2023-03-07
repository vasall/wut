#include "window.h"


#include <stdlib.h>


XWIN_API struct xwin_window *xwin_win_create(char *name, s16 w, s16 h)
{
	struct xwin_window *win;
	SDL_Window *hdl;
	SDL_GLContext ctx;
	s8 i;

	if(!name || w <= 0 || h <= 0) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Allocate the memory for the window struct */
	if(!(win = smalloc(sizeof(struct xwin_window)))) {
		ALARM(ALARM_ERR, "Failed to allocate the window struct");
		goto err_return;
	}

	/* Create the SDL window */
	if(!(hdl = SDL_CreateWindow(name, 0, 0, w, h, SDL_WINDOW_OPENGL))) {
		ALARM(ALARM_ERR, "Failed to create SDL window");
		goto err_free_win;
	}

	/* Create the SDL-GLContext-handle */
	if(!(ctx = SDL_GL_CreateContext(hdl))) {
		ALARM(ALARM_ERR, "Failed to create SDL-GLContext");
		goto err_destroy_hdl;
	}


	/* Set the attributes of the window struct */
	strcpy(win->name, name);
	win->width = w;
	win->height = h;
	win->handle = hdl; 
	win->context = ctx;

	win->parent = NULL;
	win->children_num = 0;
	for(i = 0; i < XWIN_WIN_CHILDREN_LIM; i++)
		win->children[i] = NULL;

	return win;

err_destroy_hdl:
	SDL_DestroyWindow(hdl);

err_free_win:
	sfree(win);

err_return:
	ALARM(ALARM_ERR, "Failed to create window");
	return NULL;
}


XWIN_API void xwin_win_destroy(struct xwin_window *win)
{
	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Delete the SDL-GLContext */	
	SDL_GL_DeleteContext(win->context);

	/* Destroy the SDL window */
	SDL_DestroyWindow(win->handle);

	/* Free the memory used by the window struct */
	sfree(win);


err_return:
	ALARM(ALARM_ERR, "Failed to destroy window");
}


XWIN_API s8 xwin_win_attach(struct xwin_window *parent,
		struct xwin_window *window)
{
	if(!parent || !window) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(parent->children_num + 1 > XWIN_WIN_CHILDREN_LIM) {
		ALARM(ALARM_ERR, "Can not add more children to parent window");
		goto err_return;
	}

	/* Add window to parent */
	parent->children[parent->children_num] = window;
	parent->children_num++;

	/* Set parent attribute of window */
	window->parent = parent;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to attach window");
	return -1;
}



XWIN_API void xwin_win_detach(struct xwin_window *window)
{
	s8 i;
	struct xwin_window *parent;

	if(!window) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!window->parent) {
		ALARM(ALARM_ERR, "Window does not have a parent window");
		goto err_return;
	}

	parent = window->parent;

	/* Go through all children windows to find the one */
	for(i = 0; i < XWIN_WIN_CHILDREN_LIM; i++) {
		/* ...and remove if found */
		if(parent->children[i]->id == window->id) {
			parent->children[i] = NULL;
			parent->children_num--;

			window->parent = NULL;
			return;
		}
	}

err_return:
	ALARM(ALARM_ERR, "Failed to detach window");
}


XWIN_API void xwin_win_hlfup(struct xwin_window *str,
		void (*cfnc)(struct xwin_window *w, void *data), void *data)
{
	s8 i;

	if(!str) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* Call this function on all children */
	for(i = 0; i < XWIN_WIN_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		xwin_win_hlfup(str->children[i], cfnc, data);
	}

	/* Then apply the callback function to this window struct */
	cfnc(str, data);

	return;
}
