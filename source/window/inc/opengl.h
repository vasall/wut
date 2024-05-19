#ifndef _WT_WINDOW_SUBSYSTEMS_OPENGL_H
#define _WT_WINDOW_SUBSYSTEMS_OPENGL_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"


/*
 * Initialize and configure the SDL GL module.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_gl_init(void);


/*
 * Print info about OpenGL in the console.
 */
WT_API void wt_gl_print_info(void);

#endif /* _WT_WINDOW_SUBSYSTEMS_OPENGL_H */
