#include "window.h"

#include "core.h"
#include "opengl.h"
#include "document.h"

#include <stdlib.h>




FH_API struct fh_window *fh_win_create(char *name, s16 w, s16 h)
{
	struct fh_window *win;
	SDL_Window *hdl;
	struct fh_document *doc;
	struct fh_context *ctx;
	s8 i;
	s8 name_len;

	if(!name || w <= 0 || h <= 0) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	name_len = strlen(name);
	if(name_len < 1 || name_len > FH_WIN_NAME_LIM) {
		ALARM(ALARM_ERR, "Window name is invalid");
		goto err_return;
	}

	/* Allocate the memory for the window struct */
	if(!(win = smalloc(sizeof(struct fh_window)))) {
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
	win->info = FH_WIN_INFO_VISIBLE;
	win->handle = hdl; 

	win->parent = NULL;
	win->children_num = 0;
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++)
		win->children[i] = NULL;

	/* Create new OpenGL context */
	if(!(ctx = fh_gl_create(win))) {
		ALARM(ALARM_ERR, "Failed to add GL context to window");
		goto err_destroy_hdl;
	}
	win->context = ctx;

	/* Create the document contained in the window */
	if(!(doc = fh_doc_create(win))) {
		ALARM(ALARM_ERR, "Failed to create document for window");
		goto err_destroy_ctx;
	}
	win->document = doc;


	return win;

err_destroy_ctx:
	fh_gl_destroy(win->context);

err_destroy_hdl:
	SDL_DestroyWindow(hdl);

err_free_win:
	sfree(win);

err_return:
	ALARM(ALARM_ERR, "Failed to create window");
	return NULL;
}


FH_API void fh_win_destroy(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	/* Close the document */
	fh_doc_destroy(win->document);

	/* Delete the OpenGL context */
	fh_gl_destroy(win->context);

	/* Destroy the SDL window */
	SDL_DestroyWindow(win->handle);

	/* Free the memory used by the window struct */
	sfree(win);

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to destroy window");
}


FH_API s8 fh_win_attach(struct fh_window *parent,
		struct fh_window *window)
{
	if(!parent || !window) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(parent->children_num + 1 > FH_WIN_CHILDREN_LIM) {
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



FH_API void fh_win_detach(struct fh_window *window)
{
	s8 i;
	struct fh_window *parent;

	if(!window) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!window->parent) {
		return;
	}

	parent = window->parent;

	/* Go through all children windows to find the one */
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++) {
		/* ...and remove if found */
		if(parent->children[i]->id == window->id) {
			parent->children[i] = NULL;
			parent->children_num--;

			window->parent = NULL;
			return;
		}
	}
}


FH_INTERN s8 fh_cfnc_close_windows(struct fh_window *w, void *data)
{
	/* SILENCE COMPILER */
	if(data) {}

	/* First detach the window... */
	fh_win_detach(w);

	/* ...then destroy it */
	fh_win_destroy(w);

	return 0;
}

FH_API void fh_win_close(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	/* Recursivly close all windows downwards, starting from win */
	fh_win_hlf_up(win, &fh_cfnc_close_windows, NULL);

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to close window");
}

struct fh_win_selector {
	s8 state;

	s32 id;

	struct fh_window *win;
};


FH_INTERN s8 fh_cfnc_find_window(struct fh_window *w, void *data)
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

FH_API struct fh_window *fh_win_get(s32 wd)
{
	struct fh_win_selector sel;
	struct fh_window *main;

	sel.state = 0;
	sel.id = wd;
	sel.win = NULL;

	/* Recursifly search for the window... */
	main = fh_core_get_main_window();
	fh_win_hlf_down(main, &fh_cfnc_find_window, &sel);

	/* ...and if the window was found, return it */
	if(sel.state == 1) {
		return sel.win;
	}

	return NULL;
}



FH_API void fh_win_hlf_down(struct fh_window *str,
		s8 (*cfnc)(struct fh_window *w, void *data), void *data)
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
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		fh_win_hlf_down(str->children[i], cfnc, data);
	}

	return;
}

FH_API void fh_win_hlf_up(struct fh_window *str,
		s8 (*cfnc)(struct fh_window *w, void *data), void *data)
{
	s8 i;

	if(!str) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* Call this function on all children */
	for(i = 0; i < FH_WIN_CHILDREN_LIM; i++) {
		if(!str->children[i])
			continue;

		fh_win_hlf_up(str->children[i], cfnc, data);
	}

	/* Then apply the callback function to this window struct */
	if(cfnc(str, data) == 1)
		return;

	return;
}


FH_API void fh_win_redraw(struct fh_window *win)
{
	/* SILENCIO! */
	if(win) {}

	/* Select context for this window */
	SDL_GL_MakeCurrent(win->handle, win->context);

	/* Clear the window screen */
        glClear(GL_COLOR_BUFFER_BIT);

	/* Render the document onto the window */
	fh_doc_render(win->document);

	/* Swap buffer */
        SDL_GL_SwapWindow(win->handle);
}



FH_INTERN s8 fh_win_cfnc_redraw(struct fh_window *win, void *data)
{
	/* SILENCIO! */
	if(data) {}

	if(win->info & FH_WIN_INFO_VISIBLE) {
		fh_win_redraw(win);
	}

	return 0;
}

FH_API void fh_win_redraw_all(void)
{
	struct fh_window *main;

	/* Call the fh_win_redraw() function on all visible windows */
	main = fh_core_get_main_window();
	fh_win_hlf_down(main, &fh_win_cfnc_redraw, NULL);	
}
