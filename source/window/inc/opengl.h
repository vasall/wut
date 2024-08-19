#ifndef _WUT_WINDOW_OPENGL_H
#define _WUT_WINDOW_OPENGL_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"


/*
 * Initialize and configure the SDL GL module.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_gl_init(void);


/*
 * Print info about OpenGL in the console.
 */
WUT_API void wut_gl_print_info(void);

#endif /* _WUT_WINDOW_OPENGL_H */
