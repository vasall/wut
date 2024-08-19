#ifndef _WUT_WINDOW_H
#define _WUT_WINDOW_H

struct wut_Window;

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

#define WUT_WIN_NAME_LIM  	126
#define WUT_WIN_CHILDREN_LIM     6


/*
 * INFO-FLAGS
 */
#define WUT_WIN_INFO_MAIN	(1<<0)
#define WUT_WIN_INFO_VISIBLE	(1<<1)


struct wut_Window {
	enum wut_eIdentity      identity;

	/* The unique identifier for this window */
	u16                     id;

	/* The name of the window */
	char                    name[WUT_WIN_NAME_LIM];

	/* The size of the window in pixels */
	struct wut_Rect         shape;

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


	struct wut_Element      *hovered;
	struct wut_Element      *selected;

	struct wut_EventHandler *event_handler;
};


/* The default callback function higher-level-functions for windows */
typedef s8 (*wut_WindowFunc)(struct wut_Window *, void *);


struct wut_win_selector {
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
 * Redraw a window.
 *
 * @win: Pointer to the window to redraw.
 */
WUT_XMOD void wut_win_redraw(struct wut_Window *win);


/*
 * This function will call the wut_win_redraw() function on all visible windows.
 */
WUT_XMOD void wut_win_redraw_all(void);


/*
 * Set a new element in the window as selected.
 *
 * @win: Pointer to the window
 * @ele: Pointer to the element
 *
 * Returns: 1 if the element has been selected, 0 if nothing has been done
 */
WUT_XMOD s8 wut_win_hover(struct wut_Window *win, struct wut_Element *ele);



/*
 * Set an element in the window as selected.
 *
 * @win: Pointer to the window
 * @ele: Pointer to the element
 *
 * Returns: 1 if the element has been selected, 0 if nothing has been done
 */
WUT_XMOD s8 wut_win_select(struct wut_Window *win, struct wut_Element *ele);



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
 * @w: The new width
 * @h: The new height
 */
WUT_API void wut_ResizeWindow(struct wut_Window *win, u16 w, u16 h);


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
