#ifndef _XWIN_WINDOW_H
#define _XWIN_WINDOW_H

#include "define.h"
#include "imports.h"



#define XWIN_WIN_NAME_LIM  	128
#define XWIN_WIN_CHILDREN_LIM     6


struct xwin_window {
	/* The unique identifier for this window */
	u16 id;

	/* The name of the window */
	char name[XWIN_WIN_NAME_LIM];

	/* The size of the window in pixels */
	s16 width;
	s16 height;

	/* The SDL_Window-handle */
	SDL_Window *handle;

	/* The SDL-GL-context */
	SDL_GLContext context;

	/* References to both the parent and children windows */
	struct xwin_window *parent;

	u8 children_num;
	struct xwin_window *children[XWIN_WIN_CHILDREN_LIM];
};




/*
 * Allocate the memory and create a new window.
 *
 * @name: The name of the window
 * @w: The initial width of the window
 * @h: The initial height of the window
 *
 * Returns: Either a pointer to the created window struct or NULL if an error
 *          occurred
 */
XWIN_API struct xwin_window *xwin_win_create(char *name, s16 w, s16 h);


/*
 * Destroy a window and free the allocated memory.
 *
 * @win: A pointer to the window struct
 */
XWIN_API void xwin_win_destroy(struct xwin_window *win);


/*
 * Attach a window to a parent window.
 *
 * @parent: Pointer to the parent window struct
 * @window: Pointer to the window struct to attach
 *
 * Returns: 0 on success or -1 if an error occurred
 */
XWIN_API s8 xwin_win_attach(struct xwin_window *parent,
		struct xwin_window *window);


/*
 * Remove the link to the parent window struct for this window.
 *
 * @window: Pointer to the window struct to detach from the parent
 */
XWIN_API void xwin_win_detach(struct xwin_window *window);


/*
 * This function is a higher-level-function which will apply the cfnc-function to
 * all window structs from the lowest child window struct below start, up to the
 * the str window struct itself.
 *
 * @str: The active window struct, also the one at the top
 * @cfnc: The callback-function to execute to all window structs
 * @data: A data pointer which will be passed to every function call
 */
XWIN_API void xwin_win_hlfup(struct xwin_window *str,
		void (*cfnc)(struct xwin_window *w, void *data), void *data);









#endif /* _XWIN_WINDOW_H */
