#ifndef _FH_OPENGL_H
#define _FH_OPENGL_H

#include "define.h"
#include "import.h"

#include "window.h"


struct fh_context {
	void *context;
};


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
 * Returns: Either a pointer to the newly created context or NULL if an error
 * 	    occurred
 */
FH_API struct fh_context *fh_gl_create(struct fh_window *win);


/*
 * Destroy a GL context and remove it from a window.
 *
 * @win: Pointer to the context
 */
FH_API void fh_gl_destroy(struct fh_context *ctx);


#endif /* _FH_OPENGL_H */
