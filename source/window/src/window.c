#include "source/window/inc/window.h"

#include "source/utility/inc/alarm.h"

#include "source/system/inc/system.h"

#include "source/core/inc/predefined.h"
#include "source/core/inc/core.h"

#include "source/document/inc/document.h"

#include <stdlib.h>



WUT_INTERN struct wut_Window *win_create(char *name, s16 w, s16 h)
{
	struct wut_Window *win;
	SDL_Window *hdl;
	s8 name_len;

	name_len = strlen(name);
	if(name_len < 1 || name_len > WUT_WIN_NAME_LIM) {
		WUT_ALARM(WUT_ERROR, "Window name is invalid");
		goto err_return;
	}

	/* Allocate the memory for the window struct */
	if(!(win = wut_malloc(sizeof(struct wut_Window)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate the window struct");
		goto err_return;
	}

	printf("Create window!\n");

	/* Create the SDL window */
	if(!(hdl = SDL_CreateWindow(name, 0, 0, w, h, SDL_WINDOW_OPENGL))) {
		WUT_ALARM(WUT_ERROR, "Failed to create SDL window");
		goto err_free_win;
	}

	/* Set the identifier byte */
	win->identity = WUT_IDT_WINDOW;
		
	/* Set the attributes of the window struct */
	win->id = SDL_GetWindowID(hdl);
	strcpy(win->name, name);
        
	wut_irect_clr(win->shape);
	win->shape[2] = w;
	win->shape[3] = h;

	win->info = WUT_WIN_INFO_VISIBLE;
	win->handle = hdl; 

	win->parent = NULL;
	win->level = 0;
	win->children_num = 0;
	win->firstborn = NULL;

	win->left_window = NULL;
	win->right_window = NULL;

	/* Create the rendering context used by the window */
	if(!(win->context = wut_CreateContext(win))) {
		WUT_ALARM(WUT_ERROR, "Failed to create rendering context");
		goto err_destroy_hdl;
	}

	/* Create the document contained in the window */
	if(!(win->document = wut_doc_create(win))) {
		WUT_ALARM(WUT_ERROR, "Failed to create document for window");
		goto err_destroy_ctx;
	}

	/* Create the event handler */
	if(!(win->event_handler = wut_hdl_create()))
		goto err_destroy_document;

	return win;

err_destroy_document:
	wut_doc_destroy(win->document);

err_destroy_ctx:
	wut_DestroyContext(win->context);

err_destroy_hdl:
	SDL_DestroyWindow(win->handle);

err_free_win:
	wut_free(win);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create window");
	return NULL;
}


WUT_INTERN void win_destroy(struct wut_Window *win)
{
	struct wut_Window *mwin;

	mwin = wut_cor_get_main_window();
	if(strcmp(mwin->name, win->name) == 0) {
		wut_cor_set_main_window(NULL);
	}

	wut_hdl_destroy(win->event_handler);

	wut_doc_destroy(win->document);

	wut_DestroyContext(win->context);

	SDL_DestroyWindow(win->handle);

	wut_free(win);
}


WUT_INTERN s8 win_attach(struct wut_Window *par, struct wut_Window *win)
{
	struct wut_Window *run;

	if(!par->firstborn) {
		par->firstborn = win;
	}
	else {
		run = par->firstborn;
		while(run->right_window) {
			run = run->right_window;
		}
		
		run->right_window = win;
		win->left_window = run;

	}

	par->children_num++;

	/* Set parent attribute of window */
	win->parent = par;
	win->level = par->level + 1;
		
	return 0;
}


WUT_INTERN void win_detach(struct wut_Window *win)
{
	struct wut_Window *par;

	if(!win->parent) {
		return;
	}

	par = win->parent;

	if(!win->left_window) {
		par->firstborn = win->right_window;
	}
	else
		(win->left_window)->right_window = win->right_window;

	if(win->right_window)
		(win->right_window)->left_window = win->left_window;


	win->parent = NULL;


	par->children_num--;
}


WUT_INTERN s8 win_cfnc_close(struct wut_Window *w, void *data)
{
	WUT_IGNORE(data);

	/* First detach the window... */
	win_detach(w);

	/* Then check if this window is the active one */
	if(wut_cor_is_active_window(w))
		wut_cor_set_active_window(NULL);

	/* ...finally destroy it */
	win_destroy(w);

	return 0;
}

WUT_INTERN s8 win_cfnc_update(struct wut_Window *win, void *data)
{
	WUT_IGNORE(data);

        wut_doc_update(win->document);

	return 0;
}


WUT_INTERN s8 win_cfnc_render(struct wut_Window *win, void *data)
{
        WUT_IGNORE(data);

        /* Select the current context */
	SDL_GL_MakeCurrent(win->handle, win->context->gl_context);

	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Render the document and views */
	wut_doc_render(win->document);

	/* Flush all batches */
	wut_ContextRenderBatches(win->context);

	/* Swap buffer */
	SDL_GL_SwapWindow(win->handle);

	return 0;
}


WUT_INTERN s8 win_cfnc_find(struct wut_Window *w, void *data)
{
	struct wut_WindowSelector *sel = (struct wut_WindowSelector *)data;

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


WUT_INTERN s8 win_cfnc_show(struct wut_Window *w, void *data)
{
	s32 i;

	WUT_IGNORE(data);

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

WUT_XMOD void wut_win_hlf(struct wut_Window *str, wut_WindowFunc pre_fnc,
		wut_WindowFunc post_fnc, void *data)
{
	struct wut_Window *run;
	struct wut_Window *next;

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
		next = run->right_window;

		wut_win_hlf(run, pre_fnc, post_fnc, data);

		run = next;
	}

	if(post_fnc) {
		post_fnc(str, data);
	}
}


WUT_XMOD void wut_win_update_all(void)
{
	wut_win_hlf(wut_cor_get_main_window(), &win_cfnc_update, NULL, NULL);
}


WUT_XMOD void wut_win_render_all(void)
{
        wut_win_hlf(wut_cor_get_main_window(), &win_cfnc_render, NULL, NULL);
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API struct wut_Window *wut_CreateWindow(struct wut_Window *parent, char *name,
		s32 width, s32 height)
{
	struct wut_Window *win;

	if(name == NULL || width < 0 || height < 0) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
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
		win->info = win->info | WUT_WIN_INFO_MAIN;

		/* Set this window as the main window */
		wut_cor_set_main_window(win);
	}

	return win;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create add new window");
	return NULL;
}


WUT_API void wut_CloseWindow(struct wut_Window *win)
{
	if(!win) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		goto err_return;
	}

	/* Recursivly close all windows downwards, starting from win */
	wut_win_hlf(win, NULL, &win_cfnc_close, NULL);

	return;

err_return:
	WUT_ALARM(WUT_WARNING, "Failed to close window");
}


WUT_API struct wut_Window *wut_GetWindow(s32 wd)
{
	struct wut_WindowSelector sel;
	struct wut_Window *mwin;

	sel.state = 0;
	sel.id = wd;
	sel.win = NULL;

	/* Recursifly search for the window... */
	mwin = wut_cor_get_main_window();
	wut_win_hlf(mwin, &win_cfnc_find, NULL, &sel);

	/* ...and if the window was found, return it */
	if(sel.state == 1) {
		return sel.win;
	}

	return NULL;
}


WUT_API void wut_ResizeWindow(struct wut_Window *win, u16 width, u16 height)
{
	if(!win) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	/*
	 * Update the size.
	 */
        wut_irect_clr(win->shape);
        win->shape[2] = width;
        win->shape[3] = height;

	/*
	 * Update the document.
	 */
	wut_doc_resize(win->document);
}


WUT_API void wut_ActivateWindow(struct wut_Window *win)
{
	if(!win) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return;
	}

	/* TODO */
}


WUT_API struct wut_Context *wut_GetContext(struct wut_Window *win)
{
	if(!win) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return win->context;
}


WUT_API struct wut_Document *wut_GetDocument(struct wut_Window *win)
{
	if(!win) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return win->document;
}


WUT_API void wut_DumpWindowTree(void)
{
	struct wut_Window *mwin = wut_cor_get_main_window();

	wut_win_hlf(mwin, &win_cfnc_show, NULL, NULL);
}
