#ifndef _WUT_WINDOW_H
#define _WUT_WINDOW_H

struct wut_Window;

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

/* Catch-all */
#include "source/window/inc/sdl.h"
#include "source/window/inc/opengl.h"

#include "source/graphic/inc/context.h"

#include "source/document/inc/identity.h"

#include "source/event/inc/event.h"
#include "source/event/inc/event_listener.h"

#include "source/math/inc/polygon.h"

#define WUT_WIN_NAME_LIM  	126
#define WUT_WIN_CHILDREN_LIM     6


/*
 * INFO-FLAGS
 */
#define WUT_WIN_INFO_MAIN	(1<<0)
#define WUT_WIN_INFO_VISIBLE	(1<<1)


struct wut_Window {
	u8                      identity;

	/* The unique identifier for this window */
	u16                     id;

	/* The name of the window */
	char                    name[WUT_WIN_NAME_LIM];

	/* The size of the window in pixels */
	wut_iRect               shape;

	/* 
	 * The state flags of this window (from lowest to highest):
	 *
	 * 0: Main window
	 * 1: Visibility
	 */
	u8                      info;

	/* The SDL_Window-handle */
	SDL_Window              *handle;

	/* References to both the parent and children windows */
	struct wut_Window       *parent;
	s32                     level;

	u8                      children_num;
	struct wut_Window       *firstborn;

	struct wut_Window       *left_window;
	struct wut_Window       *right_window;


	/* The document contained in this window */
	struct wut_Document     *document;

	/* The rendering context */
	struct wut_Context      *context;

	struct wut_EventHandler *event_handler;
};


/* The default callback function higher-level-functions for windows */
typedef s8 (*wut_WindowFunc)(struct wut_Window *, void *);


struct wut_WindowSelector {
	s8 state;

	s32 id;

	struct wut_Window *win;
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
WUT_XMOD void wut_win_hlf(struct wut_Window *str, wut_WindowFunc pre_fnc,
		wut_WindowFunc post_fnc, void *data);


/*
 * This function will call the wut_win_redraw() function on all visible windows.
 */
WUT_XMOD void wut_win_update_all(void);


/*
 * Go through all windows from top to bottom and render the document in it's
 * current state.
 */
WUT_XMOD void wut_win_render_all(void);

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
WUT_API struct wut_Window *wut_CreateWindow(struct wut_Window *parent, char *name,
		s32 width, s32 height);


/*
 * Close a window, by first detaching it if necessary and then destroying it.
 *
 * @win: Pointer to the window to close
 */
WUT_API void wut_CloseWindow(struct wut_Window *win);


/*
 * Retrieve a pointer to the window by searching for it with the given
 * window-descriptor.
 *
 * @wd: The window descriptor
 *
 * Returns: A pointer to the window or NULL if either an error occurred or the
 * 	    window could not be found
 */
WUT_API struct wut_Window *wut_GetWindow(s32 wd);


/*
 * Resize the window to the new given size.
 *
 * @win: Pointer to the window
 * @width: The new width
 * @height: The new height
 */
WUT_API void wut_ResizeWindow(struct wut_Window *win, u16 width, u16 height);


/*
 * Activate a window so it can be rendered on.
 *
 * @win: Pointer to the window
 */
WUT_API void wut_ActivateWindow(struct wut_Window *win);


/*
 * Get the context of a window.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the context
 */
WUT_API struct wut_Context *wut_GetContext(struct wut_Window *win);


/*
 * Retrieve the document of a window.
 *
 * @win: Pointer to the window
 *
 * Returns: A pointer to the document of a window
 */
WUT_API struct wut_Document *wut_GetDocument(struct wut_Window *win);


/*
 * Show all windows in the console.
 */
WUT_API void wut_DumpWindowTree(void);


#endif /* _WUT_WINDOW_H */
