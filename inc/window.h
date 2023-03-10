#ifndef _FH_WINDOW_H
#define _FH_WINDOW_H

#include "define.h"
#include "imports.h"


#define FH_WIN_NAME_LIM  	128
#define FH_WIN_CHILDREN_LIM     6


#define FH_WIN_INFO_MAIN	(1<<0)
#define FH_WIN_INFO_VISIBLE	(1<<1)


struct fh_window {
	/* The unique identifier for this window */
	u16 id;

	/* The name of the window */
	char name[FH_WIN_NAME_LIM];

	/* The size of the window in pixels */
	s16 width;
	s16 height;

	/* 
	 * The state flags of this window (from lowest to highest):
	 *
	 * 0: Main window
	 * 1: Visibility
	 */
	u8 info;

	/* The SDL_Window-handle */
	SDL_Window *handle;

	/* The SDL-GL-context */
	SDL_GLContext context;

	/* References to both the parent and children windows */
	struct fh_window *parent;

	u8 children_num;
	struct fh_window *children[FH_WIN_CHILDREN_LIM];

	/* The body of the  */
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
FH_API struct fh_window *fh_win_create(char *name, s16 w, s16 h);


/*
 * Destroy a window and free the allocated memory.
 *
 * @win: A pointer to the window struct
 */
FH_API void fh_win_destroy(struct fh_window *win);


/*
 * Attach a window to a parent window.
 *
 * @parent: Pointer to the parent window struct
 * @window: Pointer to the window struct to attach
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_win_attach(struct fh_window *parent,
		struct fh_window *window);


/*
 * Remove the link to the parent window struct for this window.
 *
 * @window: Pointer to the window struct to detach from the parent
 */
FH_API void fh_win_detach(struct fh_window *window);


/*
 * Close a window, by first detaching it if necessary and then destroying it.
 *
 * @win: Pointer to the window to close
 */
FH_API void fh_win_close(struct fh_window *win);


/*
 * Get a pointer to the window struct using the windowId.
 *
 * @id: The id of the window to find
 *
 * Returns: Either a pointer to the window struct or NULL if it does not exist
 */
FH_API struct fh_window *fh_win_get(s32 id);


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




#endif /* _FH_WINDOW_H */
