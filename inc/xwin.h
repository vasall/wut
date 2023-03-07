#ifndef _XWIN_XWIN_H
#define _XWIN_XWIN_H

#include "define.h"
#include "imports.h"
#include "global.h"
#include "sdl.h"
#include "window.h"


/*
 * Initialize the XWIN-framework.
 *
 * @name: The name of the main window
 * @width: The initial width of the main window
 * @height: The initial height of the main window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
XWIN_API s8 xwin_init(char *name, s32 width, s32 height);


/*
 * Shutdown the XWIN-framework and clear the allocated memory.
 */
XWIN_API void xwin_quit(void);


/*
 * Handle inputs, update and then redraw the window.
 * If this function returns an error, the programm should be closed.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
XWIN_API s8 xwin_update(void);




#endif /* _XWIN_XWIN_H */
