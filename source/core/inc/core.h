#ifndef _FH_CORE_H
#define _FH_CORE_H


#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/document.h"
#include "event/inc/event.h"
#include "graphic/inc/graphic.h"
#include "parser/inc/parser.h"
#include "style/inc/style.h"
#include "system/inc/system.h"
#include "utils/inc/utils.h"
#include "widget/inc/widget.h"

#include "window/inc/window.h"
#include "window/inc/sdl.h"
#include "window/inc/opengl.h"

struct fh_core_container {
	/* The main window and by that all windows attached to it */
	struct fh_window *main_window;

	/* 
	 * If this var is set to 1, fh_update() will return 0, promting the
	 * programm to exit.
	 */
	s8 quit;

	/* Pointer to the active window, or NULL if none is active */
	struct fh_window *active_window;
};



FH_API struct fh_core_container g_fh_core;



/*
 * Reset everything in the core. This will not free any memory, but just
 * overwrite everything with zeros.
 */
FH_API void fh_core_reset(void);


/*
 * This will set the quit flag to 1, which will prompt the fh_update() function
 * to return 0 and by doing so exit the program.
 */
FH_API void fh_core_quit(void);


/*
 * Check if the quit flag has been set.
 *
 * Returns: The value of the quit flag
 */
FH_API s8 fh_core_check_quit(void);


/*
 * Set the main window.
 *
 * @win: Pointer to the main window
 */
FH_API void fh_core_set_main_window(struct fh_window *win);


/*
 * Get the main window.
 *
 * Returns: Pointer to the main window
 */
FH_API struct fh_window *fh_core_get_main_window(void);


/*
 * Set the active window.
 *
 * @win: Pointer to the active window
 */
FH_API void fh_core_set_active_window(struct fh_window *win);


/*
 * Get the current active window.
 *
 * Returns: Pointer to the active window
 */
FH_API struct fh_window *fh_core_get_active_window(void);


/*
 * Check if the given window is the active window.
 *
 * @win: Pointer to the window
 *
 * Returns: 1 if the given window is the active one and 0 if not
 */
FH_API s8 fh_core_is_active_window(struct fh_window *win);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

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

#endif /* _FH_CORE_H */
