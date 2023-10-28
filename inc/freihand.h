#ifndef _FH_FREIHAND_H
#define _FH_FREIHAND_H

#include "stdinc.h"

#include "table.h"
#include "system.h"
#include "core.h"
#include "sdl.h"
#include "opengl.h"
#include "camera.h"
#include "window.h"
#include "document.h"
#include "element.h"
#include "model_constructor.h"
#include "flat.h"


/*
 * Initialize the FH-framework.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_Init(void);


/*
 * Shutdown the FH-framework and clear the allocated memory.
 */
FH_API void fh_Quit(void);


/*
 * Handle inputs, update and redraw the window.
 *
 * Returns: 1 if everything is normal, and 0 if either a fatal error occurred or
 * 	    the user requested to close the program
 */
FH_API s8 fh_Update(void);


#endif /* _FH_FREIHAND_H */
