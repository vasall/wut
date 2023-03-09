#ifndef _FH_OPENGL_H
#define _FH_OPENGL_H

#include "define.h"
#include "imports.h"

#include "window.h"



/*
 * Initialize and configure the SDL GL module.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_gl_init(void);


/*
 * Create and initialize the GL context for a window.
 *
 * @win: Pointer to the window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_gl_create(struct fh_window *win);


/*
 * Destroy a GL context and remove it from a window.
 *
 * @win: Pointer to the window
 */
FH_API void fh_gl_destroy(struct fh_window *win);


#endif /* _FH_OPENGL_H */
