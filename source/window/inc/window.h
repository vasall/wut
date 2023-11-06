#ifndef _FH_WINDOW_H
#define _FH_WINDOW_H

struct fh_window;

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/context.h"
#include "document/inc/identity.h"

#include "event/inc/event.h"
#include "event/inc/event_listener.h"

#include "graphic/inc/shape.h"

#define FH_WIN_NAME_LIM  	126
#define FH_WIN_CHILDREN_LIM     6


/*
 * INFO-FLAGS
 */
#define FH_WIN_INFO_MAIN	(1<<0)
#define FH_WIN_INFO_VISIBLE	(1<<1)


struct fh_window {
	u8 identifier;

	/* The unique identifier for this window */
	u16 id;

	/* The name of the window */
	char name[FH_WIN_NAME_LIM];

	/* The size of the window in pixels */
	struct fh_rect shape;

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
	struct fh_window *parent;
	s32 level;

	u8 children_num;
	struct fh_window *firstborn;

	struct fh_window *older_sibling;
	struct fh_window *younger_sibling;


	/* The document contained in this window */
	struct fh_document *document;

	/* The rendering context */
	struct fh_context *context;


	struct fh_element *hovered;
	struct fh_element *selected;

	struct fh_event_handler *event_handler;
};


/* The default callback function higher-level-functions for windows */
typedef s8 (*fh_win_cfnc)(struct fh_window *, void *);


struct fh_win_selector {
	s8 state;

	s32 id;

	struct fh_window *win;
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
FH_XMOD void fh_window_hlf(struct fh_window *str, fh_win_cfnc pre_fnc,
		fh_win_cfnc post_fnc, void *data);


/*
 * Redraw a window.
 *
 * @win: Pointer to the window to redraw.
 */
FH_XMOD void fh_window_redraw(struct fh_window *win);


/*
 * This function will call the fh_win_redraw() function on all visible windows.
 */
FH_XMOD void fh_window_redraw_all(void);


/*
 * Set a new element in the window as selected.
 *
 * @win: Pointer to the window
 * @ele: Pointer to the element
 *
 * Returns: 1 if the element has been selected, 0 if nothing has been done
 */
FH_XMOD s8 fh_window_hover(struct fh_window *win, struct fh_element *ele);



/*
 * Set an element in the window as selected.
 *
 * @win: Pointer to the window
 * @ele: Pointer to the element
 *
 * Returns: 1 if the element has been selected, 0 if nothing has been done
 */
FH_XMOD s8 fh_window_select(struct fh_window *win, struct fh_element *ele);



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
FH_API struct fh_window *fh_CreateWindow(struct fh_window *parent, char *name,
		s32 width, s32 height);


/*
 * Close a window, by first detaching it if necessary and then destroying it.
 *
 * @win: Pointer to the window to close
 */
FH_API void fh_CloseWindow(struct fh_window *win);


/*
 * Retrieve a pointer to the window by searching for it with the given
 * window-descriptor.
 *
 * @wd: The window descriptor
 *
 * Returns: A pointer to the window or NULL if either an error occurred or the
 * 	    window could not be found
 */
FH_API struct fh_window *fh_GetWindow(s32 wd);


/*
 * Resize the window to the new given size.
 *
 * @win: Pointer to the window
 * @w: The new width
 * @h: The new height
 */
FH_API void fh_ResizeWindow(struct fh_window *win, u16 w, u16 h);


/*
 * Activate a window so it can be rendered on.
 *
 * @win: Pointer to the window
 */
FH_API void fh_ActivateWindow(struct fh_window *win);


/*
 * Get the context of a window.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the context
 */
FH_API struct fh_context *fh_GetContext(struct fh_window *win);


/*
 * Retrieve the document of a window.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the document of a window
 */
FH_API struct fh_document *fh_GetDocument(struct fh_window *win);


/*
 * Show all windows in the console.
 */
FH_API void fh_DumpWindowTree(void);


#endif /* _FH_WINDOW_H */
