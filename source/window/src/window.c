#include "window/inc/window.h"

#include "window/inc/opengl.h"

#include "system/inc/system.h"

#include "core/inc/predef_resources.h"
#include "core/inc/core.h"

#include "document/inc/document.h"

#include <stdlib.h>



FH_INTERN struct fh_window *win_create(char *name, s16 w, s16 h)
{
	struct fh_window *win;
	SDL_Window *hdl;
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

	/* Set the identifier byte */
	win->identifier = FH_IDT_WINDOW;
		
	/* Set the attributes of the window struct */
	win->id = SDL_GetWindowID(hdl);
	strcpy(win->name, name);
	fh_rect_set(&win->shape, 0, 0, w, h);
	win->info = FH_WIN_INFO_VISIBLE;
	win->handle = hdl; 

	win->parent = NULL;
	win->level = 0;
	win->children_num = 0;
	win->firstborn = NULL;

	win->older_sibling = NULL;
	win->younger_sibling = NULL;

	win->hovered = NULL;
	win->selected = NULL;

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

	/* Create the event handler */
	if(!(win->event_handler = fh_handler_create()))
		goto err_destroy_document;

	return win;

err_destroy_document:
	fh_DestroyDocument(win->document);

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
	struct fh_window *mwin;

	mwin = fh_core_get_main_window();
	if(strcmp(mwin->name, win->name) == 0) {
		fh_core_set_main_window(NULL);
	}

	fh_handler_destroy(win->event_handler);

	fh_DestroyDocument(win->document);

	fh_DestroyContext(win->context);

	SDL_DestroyWindow(win->handle);

	fh_free(win);
}


FH_INTERN s8 win_attach(struct fh_window *par, struct fh_window *win)
{
	struct fh_window *run;

	if(!par->firstborn) {
		par->firstborn = win;
	}
	else {
		run = par->firstborn;
		while(run->younger_sibling) {
			run = run->younger_sibling;
		}
		
		run->younger_sibling = win;
		win->older_sibling = run;

	}

	par->children_num++;

	/* Set parent attribute of window */
	win->parent = par;
	win->level = par->level + 1;
		
	return 0;
}


FH_INTERN void win_detach(struct fh_window *win)
{
	struct fh_window *par;

	if(!win->parent) {
		return;
	}

	par = win->parent;

	if(!win->older_sibling) {
		par->firstborn = win->younger_sibling;
	}
	else
		(win->older_sibling)->younger_sibling = win->younger_sibling;

	if(win->younger_sibling)
		(win->younger_sibling)->older_sibling = win->older_sibling;


	win->parent = NULL;


	par->children_num--;
}


FH_INTERN s8 win_cfnc_close(struct fh_window *w, void *data)
{
	fh_Ignore(data);

	/* First detach the window... */
	win_detach(w);

	/* Then check if this window is the active one */
	if(fh_core_is_active_window(w))
		fh_core_set_active_window(NULL);

	/* ...finally destroy it */
	win_destroy(w);

	return 0;
}

FH_INTERN s8 win_cfnc_redraw(struct fh_window *win, void *data)
{
	fh_Ignore(data);

	/* Select the current context */
	SDL_GL_MakeCurrent(win->handle, win->context->gl_context);	

	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Render the Document-UI and views */
	fh_RenderDocument(win->document);

	/* Swap buffer */
        SDL_GL_SwapWindow(win->handle);

	return 0;
}


FH_INTERN s8 win_cfnc_find(struct fh_window *w, void *data)
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


FH_INTERN s8 win_cfnc_show(struct fh_window *w, void *data)
{
	s32 i;

	fh_Ignore(data);

	for(i = 0; i < w->level; i++)
		printf("  ");

	printf("%s", w->name);

	printf("\n");

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_XMOD void fh_window_hlf(struct fh_window *str, fh_win_cfnc pre_fnc,
		fh_win_cfnc post_fnc, void *data)
{
	struct fh_window *run;
	struct fh_window *next;

	if(!str)
		return;

	if(pre_fnc) {
		if(pre_fnc(str, data)) {
			return;
		}
	}

	/* Call this function on all children */
	run = str->firstborn;
	while(run) {
		next = run->younger_sibling;

		fh_window_hlf(run, pre_fnc, post_fnc, data);

		run = next;
	}

	if(post_fnc) {
		post_fnc(str, data);
	}
}


FH_XMOD void fh_window_redraw(struct fh_window *win)
{
	win_cfnc_redraw(win, NULL);
}


FH_XMOD void fh_window_redraw_all(void)
{
	fh_window_hlf(fh_core_get_main_window(), &win_cfnc_redraw, NULL, NULL);
}


FH_XMOD s8 fh_window_hover(struct fh_window *win, struct fh_element *ele)
{
	struct fh_element *old_hovered;

	/*
	 * Check if the hovered element has changed.
	 */
	if(!(fh_element_compare(win->hovered, ele))) {
		old_hovered = win->hovered;

		/* If that is the case, first modify the element flags */
		fh_element_mod_info(win->hovered, FH_ELEMENT_F_HOVERED, 0);
		fh_element_mod_info(ele, FH_ELEMENT_F_HOVERED, 1);

		/* Then link the new element */
		win->hovered = ele;

		/* Lastly trigger the events */
		if(old_hovered) {
			fh_event_trigger_raw(
					FH_EVT_ELEMENTLEAVE,
					win,
					old_hovered
					);
		}

		fh_event_trigger_raw(
				FH_EVT_ELEMENTENTER,
				win,
				ele
				);

		return 1;
	}

	return 0;
}


FH_XMOD s8 fh_window_select(struct fh_window *win, struct fh_element *ele)
{
	struct fh_element *old_selected;

	/*
	 * Check if the selected element has changed.
	 */
	if(!(fh_element_compare(win->selected, ele))) {
		old_selected = win->selected;

		/* If that is the case, first modify the element flags */
		fh_element_mod_info(win->selected, FH_ELEMENT_F_SELECTED, 0);
		fh_element_mod_info(ele, FH_ELEMENT_F_SELECTED, 1);

		/* Then link the new element */
		win->selected = ele;

		/* Lastly trigger the events */
		if(old_selected) {
			fh_event_trigger_raw(
					FH_EVT_ELEMENTUNSELECT,
					win,
					old_selected
					);
		}

		fh_event_trigger_raw(
				FH_EVT_ELEMENTSELECT,
				win,
				ele
				);

		return 1;
	}

	return 0;
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
	fh_window_hlf(win, NULL, &win_cfnc_close, NULL);

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to close window");
}


FH_API struct fh_window *fh_GetWindow(s32 wd)
{
	struct fh_win_selector sel;
	struct fh_window *mwin;

	sel.state = 0;
	sel.id = wd;
	sel.win = NULL;

	/* Recursifly search for the window... */
	mwin = fh_core_get_main_window();
	fh_window_hlf(mwin, &win_cfnc_find, NULL, &sel);

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



FH_API void fh_DumpWindowTree(void)
{
	struct fh_window *mwin = fh_core_get_main_window();

	fh_window_hlf(mwin, &win_cfnc_show, NULL, NULL);
}
