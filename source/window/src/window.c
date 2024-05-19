#include "window/inc/window.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include "core/inc/predefined.h"
#include "core/inc/core.h"

#include "document/inc/document.h"

#include <stdlib.h>



WT_INTERN struct wt_window *win_create(char *name, s16 w, s16 h)
{
	struct wt_window *win;
	SDL_Window *hdl;
	s8 name_len;

	name_len = strlen(name);
	if(name_len < 1 || name_len > WT_WIN_NAME_LIM) {
		WT_ALARM(WT_ERROR, "Window name is invalid");
		goto err_return;
	}

	/* Allocate the memory for the window struct */
	if(!(win = wt_malloc(sizeof(struct wt_window)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate the window struct");
		goto err_return;
	}

	/* Create the SDL window */
	if(!(hdl = SDL_CreateWindow(name, 0, 0, w, h, SDL_WINDOW_OPENGL))) {
		WT_ALARM(WT_ERROR, "Failed to create SDL window");
		goto err_free_win;
	}

	/* Set the identifier byte */
	win->identity = WT_IDT_WINDOW;
		
	/* Set the attributes of the window struct */
	win->id = SDL_GetWindowID(hdl);
	strcpy(win->name, name);
	wt_rect_set(&win->shape, 0, 0, w, h);
	win->info = WT_WIN_INFO_VISIBLE;
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
	if(!(win->context = wt_CreateContext(win))) {
		WT_ALARM(WT_ERROR, "Failed to create rendering context");
		goto err_destroy_hdl;
	}

	/* Create the document contained in the window */
	if(!(win->document = wt_CreateDocument(win))) {
		WT_ALARM(WT_ERROR, "Failed to create document for window");
		goto err_destroy_ctx;
	}

	/* Create the event handler */
	if(!(win->event_handler = wt_handler_create()))
		goto err_destroy_document;

	return win;

err_destroy_document:
	wt_DestroyDocument(win->document);

err_destroy_ctx:
	wt_DestroyContext(win->context);

err_destroy_hdl:
	SDL_DestroyWindow(win->handle);

err_free_win:
	wt_free(win);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create window");
	return NULL;
}


WT_INTERN void win_destroy(struct wt_window *win)
{
	struct wt_window *mwin;

	mwin = wt_core_get_main_window();
	if(strcmp(mwin->name, win->name) == 0) {
		wt_core_set_main_window(NULL);
	}

	wt_handler_destroy(win->event_handler);

	wt_DestroyDocument(win->document);

	wt_DestroyContext(win->context);

	SDL_DestroyWindow(win->handle);

	wt_free(win);
}


WT_INTERN s8 win_attach(struct wt_window *par, struct wt_window *win)
{
	struct wt_window *run;

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


WT_INTERN void win_detach(struct wt_window *win)
{
	struct wt_window *par;

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


WT_INTERN s8 win_cfnc_close(struct wt_window *w, void *data)
{
	wt_Ignore(data);

	/* First detach the window... */
	win_detach(w);

	/* Then check if this window is the active one */
	if(wt_core_is_active_window(w))
		wt_core_set_active_window(NULL);

	/* ...finally destroy it */
	win_destroy(w);

	return 0;
}

WT_INTERN s8 win_cfnc_redraw(struct wt_window *win, void *data)
{
	wt_Ignore(data);

	/* Select the current context */
	SDL_GL_MakeCurrent(win->handle, win->context->gl_context);

	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Render the Document-UI and views */
	wt_RenderDocument(win->document);

	/* Flush all batches */
	wt_ContextRenderBatches(win->context);

	/* Swap buffer */
	SDL_GL_SwapWindow(win->handle);

	return 0;
}


WT_INTERN s8 win_cfnc_find(struct wt_window *w, void *data)
{
	struct wt_win_selector *sel = (struct wt_win_selector *)data;

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


WT_INTERN s8 win_cfnc_show(struct wt_window *w, void *data)
{
	s32 i;

	wt_Ignore(data);

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

WT_XMOD void wt_window_hlf(struct wt_window *str, wt_win_cfnc pre_fnc,
		wt_win_cfnc post_fnc, void *data)
{
	struct wt_window *run;
	struct wt_window *next;

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

		wt_window_hlf(run, pre_fnc, post_fnc, data);

		run = next;
	}

	if(post_fnc) {
		post_fnc(str, data);
	}
}


WT_XMOD void wt_window_redraw(struct wt_window *win)
{
	win_cfnc_redraw(win, NULL);
}


WT_XMOD void wt_window_redraw_all(void)
{
	wt_window_hlf(wt_core_get_main_window(), &win_cfnc_redraw, NULL, NULL);
}


WT_XMOD s8 wt_window_hover(struct wt_window *win, struct wt_element *ele)
{
	struct wt_element *old_hovered;

	/*
	 * Check if the hovered element has changed.
	 */
	if(!(wt_element_compare(win->hovered, ele))) {
		old_hovered = win->hovered;

		/* If that is the case, first modify the element flags */
		wt_element_mod_info(win->hovered, WT_ELEMENT_F_HOVERED, 0);
		wt_element_mod_info(ele, WT_ELEMENT_F_HOVERED, 1);

		/* Then link the new element */
		win->hovered = ele;

		/* Lastly trigger the events */
		if(old_hovered) {
			wt_event_trigger_raw(
					WT_EVT_ELEMENTLEAVE,
					win,
					old_hovered
					);
		}

		wt_event_trigger_raw(
				WT_EVT_ELEMENTENTER,
				win,
				ele
				);

	}

	return 1;
}


WT_XMOD s8 wt_window_select(struct wt_window *win, struct wt_element *ele)
{
	struct wt_element *old_selected;

	/*
	 * Check if the selected element has changed.
	 */
	if(!(wt_element_compare(win->selected, ele))) {
		old_selected = win->selected;

		/* If that is the case, first modify the element flags */
		wt_element_mod_info(win->selected, WT_ELEMENT_F_SELECTED, 0);
		wt_element_mod_info(ele, WT_ELEMENT_F_SELECTED, 1);

		/* Then link the new element */
		win->selected = ele;

		/* Lastly trigger the events */
		if(old_selected) {
			wt_event_trigger_raw(
					WT_EVT_ELEMENTUNSELECT,
					win,
					old_selected
					);
		}

		wt_event_trigger_raw(
				WT_EVT_ELEMENTSELECT,
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

WT_API struct wt_window *wt_CreateWindow(struct wt_window *parent, char *name,
		s32 width, s32 height)
{
	struct wt_window *win;

	if(name == NULL || width < 0 || height < 0) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
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
		win->info = win->info | WT_WIN_INFO_MAIN;

		/* Set this window as the main window */
		wt_core_set_main_window(win);
	}

	return win;

err_return:
	WT_ALARM(WT_ERROR, "Failed to create add new window");
	return NULL;
}


WT_API void wt_CloseWindow(struct wt_window *win)
{
	if(!win) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		goto err_return;
	}

	/* Recursivly close all windows downwards, starting from win */
	wt_window_hlf(win, NULL, &win_cfnc_close, NULL);

	return;

err_return:
	WT_ALARM(WT_WARNING, "Failed to close window");
}


WT_API struct wt_window *wt_GetWindow(s32 wd)
{
	struct wt_win_selector sel;
	struct wt_window *mwin;

	sel.state = 0;
	sel.id = wd;
	sel.win = NULL;

	/* Recursifly search for the window... */
	mwin = wt_core_get_main_window();
	wt_window_hlf(mwin, &win_cfnc_find, NULL, &sel);

	/* ...and if the window was found, return it */
	if(sel.state == 1) {
		return sel.win;
	}

	return NULL;
}


WT_API void wt_ResizeWindow(struct wt_window *win, u16 w, u16 h)
{
	if(!win) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	/*
	 * Update the shape.
	 */
	wt_rect_set(&win->shape, 0, 0, w, h);

	/*
	 * Update the document.
	 */
	wt_ResizeDocument(win->document);
}


WT_API void wt_ActivateWindow(struct wt_window *win)
{
	if(!win) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return;
	}

	/* TODO */
}


WT_API struct wt_context *wt_GetContext(struct wt_window *win)
{
	if(!win) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return win->context;
}


WT_API struct wt_document *wt_GetDocument(struct wt_window *win)
{
	if(!win) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return win->document;
}



WT_API void wt_DumpWindowTree(void)
{
	struct wt_window *mwin = wt_core_get_main_window();

	wt_window_hlf(mwin, &win_cfnc_show, NULL, NULL);
}
