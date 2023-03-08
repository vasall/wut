#ifndef _XWIN_XWIN_H
#define _XWIN_XWIN_H

#include "define.h"
#include "imports.h"
#include "core.h"
#include "sdl.h"
#include "window.h"


/*
 * Initialize the XWIN-framework.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
XWIN_API s8 xwin_init(void);


/*
 * Shutdown the XWIN-framework and clear the allocated memory.
 */
XWIN_API void xwin_quit(void);


/*
 * Create a new window and attach it to the parent. If 0 is given, it will be
 * considered the main window.
 *
 * @parent: The windowId of the parent window
 * @name: The name of the window
 * @width: The initial width of the window
 * @height: The intial height of the window
 *
 * Returns: Either the windowId or -1 if an error occurred
 */
XWIN_API s32 xwin_add_window(s32 parent, char *name, s32 width, s32 height);


/*
 * Handle inputs and update the window.
 * If this function returns an error or the return code 1, the window should be
 * closed.
 *
 * Returns: 1 if everything is normal, -1 if an error occurred and 1 if the user
 *          requested to close the program
 */
XWIN_API s8 xwin_update(void);


/*
 * Redraw the window.
 */
XWIN_API void xwin_render(void);



#endif /* _XWIN_XWIN_H */
