#ifndef _FH_WINDOW_H
#define _FH_WINDOW_H

#include "stdinc.h"
#include "context.h"


#define FH_WIN_NAME_LIM  	126
#define FH_WIN_CHILDREN_LIM     6


#define FH_WIN_INFO_MAIN	(1<<0)
#define FH_WIN_INFO_VISIBLE	(1<<1)


struct fh_window {
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

	u8 children_num;
	struct fh_window *firstborn;

	struct fh_window *older_sibling;
	struct fh_window *younger_sibling;


	/* The document contained in this window */
	struct fh_document *document;

	/* The rendering context */
	struct fh_context *context;
};


struct fh_win_selector {
	s8 state;

	s32 id;

	struct fh_window *win;
};


/*
 * This function is a higher-level-function which will apply the cfnc-function to
 * all window structs starting from the str window downwards.
 *
 * Note that if the callback function returns 1, the recursion will stop.
 *
 * @str: The active window struct, also the one at the top
 * @cfnc: The callback-function to execute to all window structs
 * @data: A data pointer which will be passed to every function call
 */
FH_API void fh_win_hlf_down(struct fh_window *str,
		s8 (*cfnc)(struct fh_window *w, void *data), void *data);


/*
 * This function is a higher-level-function which will apply the cfnc-function to
 * all window structs from the lowest child window struct below start, up to the
 * the str window struct itself.
 *
 * Note that if the callback function returns 1, the recursion will stop.
 *
 * @str: The active window struct, also the one at the top
 * @cfnc: The callback-function to execute to all window structs
 * @data: A data pointer which will be passed to every function call
 */
FH_API void fh_win_hlf_up(struct fh_window *str,
		s8 (*cfnc)(struct fh_window *w, void *data), void *data);


/*
 * Redraw a window.
 *
 * @win: Pointer to the window to redraw.
 */
FH_API void fh_win_redraw(struct fh_window *win);


/*
 * This function will call the fh_win_redraw() function on all visible windows.
 */
FH_API void fh_win_redraw_all(void);


/*
 * Activate a window to enable rendering.
 *
 * @w: A pointer to the window
 */
FH_API void fh_win_activate(struct fh_window *w);


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
 * Update the pixels depending on the given inputs and changes.
 *
 * @win: Pointer to the window
 */
FH_API void fh_RedrawWindow(struct fh_window *win);


/*
 * Redraw all windows.
 */
FH_API void fh_RedrawAllWindows(void);


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
 * Apply a function downwards the window tree, starting from <str>.
 * If the callback function returns 1, the recursion will end.
 *
 * @str: Pointer to the starting window
 * @fnc: The function to apply
 * @[data]: Data to be passed to every function call
 */
FH_API void fh_ApplyWindowsDown(struct fh_window *str,
		s8 (*fnc)(struct fh_window *w, void *data), void *data);


/*
 * Apply a function upwards the window tree, up to <str>.
 * If the callback function returns 1, the recursion will end.
 *
 * @str: Pointer to the window branch
 * @fnc: The function to apply
 * @[data]: Data to be passed to every function call
 */
FH_API void fh_ApplyWindowsUp(struct fh_window *str,
		s8 (*fnc)(struct fh_window *w, void *data), void *data);

#endif /* _FH_WINDOW_H */
