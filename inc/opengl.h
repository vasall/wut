#ifndef _FH_OPENGL_H
#define _FH_OPENGL_H

#include "define.h"
#include "datatype.h"
#include "import.h"

#include "window.h"


/*
 * Initialize and configure the SDL GL module.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_gl_init(void);


/*
 * Print info about OpenGL in the console.
 */
FH_API void fh_gl_print_info(void);

#endif /* _FH_OPENGL_H */
