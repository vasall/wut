#ifndef _WT_WINDOW_H
#define _WT_WINDOW_H

struct wt_window;

#include "core/inc/define.h"
#include "core/inc/import.h"

/* Catch-all */
#include "window/inc/sdl.h"
#include "window/inc/opengl.h"

#include "graphic/inc/context.h"

#include "document/inc/identity.h"

#include "event/inc/event.h"
#include "event/inc/event_listener.h"

#include "utility/inc/shape.h"

#define WT_WIN_NAME_LIM  	126
#define WT_WIN_CHILDREN_LIM     6


/*
 * INFO-FLAGS
 */
#define WT_WIN_INFO_MAIN	(1<<0)
#define WT_WIN_INFO_VISIBLE	(1<<1)


struct wt_window {
	wt_identity_t identity;

	/* The unique identifier for this window */
	u16 id;

	/* The name of the window */
	char name[WT_WIN_NAME_LIM];

	/* The size of the window in pixels */
	struct wt_rect shape;

	/* 
	 * The state flags of this window (from lowest to highest):
	 *
	 * 0: Main window
	 * 1: Visibility
	 */
	u8 info;

	/* The SDL_Window-handle */
	SDL_Window *handle;

	/* References to both the parent and children windows */
	struct wt_window *parent;
	s32 level;

	u8 children_num;
	struct wt_window *firstborn;

	struct wt_window *older_sibling;
	struct wt_window *younger_sibling;


	/* The document contained in this window */
	struct wt_document *document;

	/* The rendering context */
	struct wt_context *context;


	struct wt_element *hovered;
	struct wt_element *selected;

	struct wt_event_handler *event_handler;
};


/* The default callback function higher-level-functions for windows */
typedef s8 (*wt_win_cfnc)(struct wt_window *, void *);


struct wt_win_selector {
	s8 state;

	s32 id;

	struct wt_window *win;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * This function is a higher-level-function which will apply the cfnc-function to
 * all window structs starting from the str window downwards.
 *
 * Note that if the pre-function returns 1, the recursion will stop.
 *
 * @str: The active window struct, also the one at the top
 * @pre_fnc: The pre-function
 * @post_fnc: The post-function
 * @data: A data pointer which will be passed to every function call
 */
WT_XMOD void wt_window_hlf(struct wt_window *str, wt_win_cfnc pre_fnc,
		wt_win_cfnc post_fnc, void *data);


/*
 * Redraw a window.
 *
 * @win: Pointer to the window to redraw.
 */
WT_XMOD void wt_window_redraw(struct wt_window *win);


/*
 * This function will call the wt_win_redraw() function on all visible windows.
 */
WT_XMOD void wt_window_redraw_all(void);


/*
 * Set a new element in the window as selected.
 *
 * @win: Pointer to the window
 * @ele: Pointer to the element
 *
 * Returns: 1 if the element has been selected, 0 if nothing has been done
 */
WT_XMOD s8 wt_window_hover(struct wt_window *win, struct wt_element *ele);



/*
 * Set an element in the window as selected.
 *
 * @win: Pointer to the window
 * @ele: Pointer to the element
 *
 * Returns: 1 if the element has been selected, 0 if nothing has been done
 */
WT_XMOD s8 wt_window_select(struct wt_window *win, struct wt_element *ele);



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Create a new window and attach it to the parent. If 0 is given, it will be
 * considered the main window.
 *
 * @parent: Pointer to the parent window or NULL
 * @name: The name of the window
 * @width: The initial width of the window
 * @height: The intial height of the window
 *
 * Returns: Either a pointer to the created window or NULL if an error occurred
 */
WT_API struct wt_window *wt_CreateWindow(struct wt_window *parent, char *name,
		s32 width, s32 height);


/*
 * Close a window, by first detaching it if necessary and then destroying it.
 *
 * @win: Pointer to the window to close
 */
WT_API void wt_CloseWindow(struct wt_window *win);


/*
 * Retrieve a pointer to the window by searching for it with the given
 * window-descriptor.
 *
 * @wd: The window descriptor
 *
 * Returns: A pointer to the window or NULL if either an error occurred or the
 * 	    window could not be found
 */
WT_API struct wt_window *wt_GetWindow(s32 wd);


/*
 * Resize the window to the new given size.
 *
 * @win: Pointer to the window
 * @w: The new width
 * @h: The new height
 */
WT_API void wt_ResizeWindow(struct wt_window *win, u16 w, u16 h);


/*
 * Activate a window so it can be rendered on.
 *
 * @win: Pointer to the window
 */
WT_API void wt_ActivateWindow(struct wt_window *win);


/*
 * Get the context of a window.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the context
 */
WT_API struct wt_context *wt_GetContext(struct wt_window *win);


/*
 * Retrieve the document of a window.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the document of a window
 */
WT_API struct wt_document *wt_GetDocument(struct wt_window *win);


/*
 * Show all windows in the console.
 */
WT_API void wt_DumpWindowTree(void);


#endif /* _WT_WINDOW_H */
