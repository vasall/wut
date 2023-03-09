#ifndef _XWIN_OPENGL_H
#define _XWIN_OPENGL_H

#include "define.h"
#include "imports.h"

#include "window.h"



/*
 * Initialize and configure the SDL GL module.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
XWIN_API s8 xwin_gl_init(void);


/*
 * Create and initialize the GL context for a window.
 *
 * @win: Pointer to the window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
XWIN_API s8 xwin_gl_create(struct xwin_window *win);


/*
 * Destroy a GL context and remove it from a window.
 *
 * @win: Pointer to the window
 */
XWIN_API void xwin_gl_destroy(struct xwin_window *win);


#endif /* _XWIN_OPENGL_H */
