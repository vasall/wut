#include "window.h"

#include "core.h"
#include "opengl.h"

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

	/* Set the attributes of the window struct */
	win->id = SDL_GetWindowID(hdl);
	strcpy(win->name, name);
	win->width = w;
	win->height = h;
	win->info = XWIN_WIN_INFO_VISIBLE;
	win->handle = hdl; 

	win->parent = NULL;
	win->children_num = 0;
	for(i = 0; i < XWIN_WIN_CHILDREN_LIM; i++)
		win->children[i] = NULL;

	/* Create the SDL-GLContext-handle */
	if(xwin_gl_create(win) < 0) {
		ALARM(ALARM_ERR, "Failed to add GL context to window");
		goto err_destroy_hdl;
	}


	return win;

err_destroy_ctx:

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
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	/* Delete the SDL-GLContext */	
	SDL_GL_DeleteContext(win->context);

	/* Destroy the SDL window */
	SDL_DestroyWindow(win->handle);

	/* Free the memory used by the window struct */
	sfree(win);

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to destroy window");
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
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	if(!window->parent) {
		ALARM(ALARM_WARN, "Window does not have a parent window");
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
	ALARM(ALARM_WARN, "Failed to detach window");
}


XWIN_INTERN s8 xwin_cfnc_close_windows(struct xwin_window *w, void *data)
{
	/* SILENCE COMPILER */
	if(data) {}

	/* First detach the window... */
	xwin_win_detach(w);

	/* ...then destroy it */
	xwin_win_destroy(w);

	return 0;
}

XWIN_API void xwin_win_close(struct xwin_window *win)
{
	if(!win) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	/* Recursivly close all windows downwards, starting from win */
	xwin_win_hlfup(win, &xwin_cfnc_close_windows, NULL);

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to close window");
}

struct xwin_win_selector {
	s8 state;

	s32 id;

	struct xwin_window *win;
};


XWIN_INTERN s8 xwin_cfnc_find_window(struct xwin_window *w, void *data)
{
	struct xwin_win_selector *sel = (struct xwin_win_selector *)data;

	/* Check if the window has already been found */
	if(sel->state == 1) {
		return 1;
	}

	/* If the current window is a match, return it and stop the recursion */
	if(w->id == sel->id) {
		sel->state = 1;
		sel->win = w;

		return 1;
	}

	return 0;

}

XWIN_API struct xwin_window *xwin_win_get(s32 id)
{
	struct xwin_win_selector sel;
	sel.state = 0;
	sel.id = id;
	sel.win = NULL;

	/* Recursifly search for the window... */
	xwin_win_hlfdown(g_xwin_core.main_window, &xwin_cfnc_find_window, &sel);

	/* ...and if the window was found, return it */
	if(sel.state == 1) {
		return sel.win;
	}

	return NULL;
}



XWIN_API void xwin_win_hlfdown(struct xwin_window *str,
		s8 (*cfnc)(struct xwin_window *w, void *data), void *data)
{
	s8 i;

	if(!str) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* Then apply the callback function to this window struct */
	if(cfnc(str, data) == 1)
		return;

	/* Call this function on all children */
	for(i = 0; i < XWIN_WIN_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		xwin_win_hlfup(str->children[i], cfnc, data);
	}

	return;
}

XWIN_API void xwin_win_hlfup(struct xwin_window *str,
		s8 (*cfnc)(struct xwin_window *w, void *data), void *data)
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
	if(cfnc(str, data) == 1)
		return;

	return;
}
