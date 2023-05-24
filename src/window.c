#include "window.h"

#include "alarm.h"
#include "system.h"
#include "predef_resources.h"
#include "core.h"
#include "opengl.h"
#include "document.h"

#include <stdlib.h>



FH_INTERN struct fh_window *win_create(char *name, s16 w, s16 h)
{
	struct fh_window *win;
	SDL_Window *hdl;
	s8 i;
	s8 name_len;

	name_len = strlen(name);
	if(name_len < 1 || name_len > FH_WIN_NAME_LIM) {
		ALARM(ALARM_ERR, "Window name is invalid");
		goto err_return;
	}

	/* Allocate the memory for the window struct */
	if(!(win = fh_malloc(sizeof(struct fh_window)))) {
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
	fh_rect_set(&win->shape, 0, 0, w, h);
	win->info = FH_WIN_INFO_VISIBLE;
	win->handle = hdl; 

	win->parent = NULL;
	win->children_num = 0;
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++)
		win->children[i] = NULL;


	/* Create the rendering context used by the window */
	if(!(win->context = fh_CreateContext(win))) {
		ALARM(ALARM_ERR, "Failed to create rendering context");
		goto err_destroy_hdl;
	}

	/* Create the document contained in the window */
	if(!(win->document = fh_CreateDocument(win))) {
		ALARM(ALARM_ERR, "Failed to create document for window");
		goto err_destroy_ctx;
	}

	return win;

err_destroy_ctx:
	fh_DestroyContext(win->context);

err_destroy_hdl:
	SDL_DestroyWindow(win->handle);

err_free_win:
	fh_free(win);

err_return:
	ALARM(ALARM_ERR, "Failed to create window");
	return NULL;
}


FH_INTERN void win_destroy(struct fh_window *win)
{
	fh_DestroyDocument(win->document);

	fh_DestroyContext(win->context);

	SDL_DestroyWindow(win->handle);

	fh_free(win);
}


FH_INTERN s8 win_attach(struct fh_window *par, struct fh_window *win)
{
	if(par->children_num + 1 > FH_WIN_CHILDREN_LIM) {
		ALARM(ALARM_ERR, "Can not add more children to parent window");
		goto err_return;
	}

	/* Add window to parent */
	par->children[par->children_num] = win;
	par->children_num++;

	/* Set parent attribute of window */
	win->parent = par;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to attach window");
	return -1;
}


FH_INTERN void win_detach(struct fh_window *win)
{
	s8 i;
	struct fh_window *par;

	if(!win->parent) {
		return;
	}

	par = win->parent;

	/* Go through all children windows to find the one */
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++) {
		/* ...and remove if found */
		if(par->children[i]->id == win->id) {
			par->children[i] = NULL;
			par->children_num--;

			win->parent = NULL;
			return;
		}
	}
}


FH_INTERN s8 win_close_windows(struct fh_window *w, void *data)
{
	/* SILENCE COMPILER */
	if(data) {}

	/* First detach the window... */
	win_detach(w);

	/* ...then destroy it */
	win_destroy(w);

	return 0;
}

FH_INTERN s8 win_redraw(struct fh_window *win, void *data)
{
	fh_Ignore(data);

	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Render the Document-UI and views */
	fh_RenderDocument(win->document);

	/* Swap buffer */
        SDL_GL_SwapWindow(win->handle);

	return 0;
}

struct fh_win_selector {
	s8 state;

	s32 id;

	struct fh_window *win;
};


FH_INTERN s8 win_find_window(struct fh_window *w, void *data)
{
	struct fh_win_selector *sel = (struct fh_win_selector *)data;

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


FH_INTERN void win_hlf_down(struct fh_window *str,
		s8 (*cfnc)(struct fh_window *w, void *data), void *data)
{
	s8 i;

	/* Then apply the callback function to this window struct */
	if(cfnc(str, data) == 1)
		return;

	/* Call this function on all children */
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		win_hlf_down(str->children[i], cfnc, data);
	}

	return;
}


FH_INTERN void win_hlf_up(struct fh_window *str,
		s8 (*cfnc)(struct fh_window *w, void *data), void *data)
{
	s8 i;

	/* Call this function on all children */
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		win_hlf_up(str->children[i], cfnc, data);
	}

	/* Then apply the callback function to this window struct */
	if(cfnc(str, data) == 1)
		return;

	return;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_window *fh_CreateWindow(struct fh_window *parent, char *name,
		s32 width, s32 height)
{
	struct fh_window *win;
		
	if(name == NULL || width < 0 || height < 0) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Then create a new window */
	if(!(win = win_create(name, width, height)))
		goto err_return;

	/* If a parent is specified */
	if(parent != NULL) {
		win_attach(parent, win);
	}
	/* Otherwise... */
	else {
		/* Mark this window as the main window */
		win->info = win->info | FH_WIN_INFO_MAIN;

		/* Set this window as the main window */
		fh_core_set_main_window(win);
	}

	return win;

err_return:
	ALARM(ALARM_ERR, "Failed to create add new window");
	return NULL;
}


FH_API void fh_CloseWindow(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	/* Recursivly close all windows downwards, starting from win */
	win_hlf_up(win, &win_close_windows, NULL);

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to close window");
}


FH_API struct fh_window *fh_GetWindow(s32 wd)
{
	struct fh_win_selector sel;
	struct fh_window *main;

	sel.state = 0;
	sel.id = wd;
	sel.win = NULL;

	/* Recursifly search for the window... */
	main = fh_core_get_main_window();
	win_hlf_down(main, &win_find_window, &sel);

	/* ...and if the window was found, return it */
	if(sel.state == 1) {
		return sel.win;
	}

	return NULL;
}


FH_API void fh_ResizeWindow(struct fh_window *win, u16 w, u16 h)
{
	if(!win) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/*
	 * Update the shape.
	 */
	fh_rect_set(&win->shape, 0, 0, w, h);

	/*
	 * Update the document.
	 */
	fh_ResizeDocument(win->document);
}


FH_API void fh_ActivateWindow(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	/* TODO */
}


FH_API void fh_RedrawWindow(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	win_redraw(win, NULL);
}


FH_API void fh_RedrawAllWindows(void)
{
	struct fh_window *main;

	/* Call the fh_win_redraw() function on all visible windows */
	main = fh_core_get_main_window();
	win_hlf_down(main, &win_redraw, NULL);
}


FH_API struct fh_context *fh_GetContext(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return NULL;
	}

	return win->context;
}


FH_API struct fh_document *fh_GetDocument(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return NULL;
	}

	return win->document;
}
