#ifndef _WT_CORE_H
#define _WT_CORE_H

/* ---- Catch-all ---- */
#include "core/inc/define.h"
#include "core/inc/import.h"
#include "core/inc/predefined.h"

#include "document/inc/document.h"
#include "event/inc/event.h"
#include "graphic/inc/graphic.h"
#include "style/inc/style.h"
#include "system/inc/system.h"
#include "utility/inc/utility.h"
#include "widget/inc/widget.h"

#include "window/inc/window.h"


struct wt_core_container {
	/* The main window and by that all windows attached to it */
	struct wt_window *main_window;

	/* 
	 * If this var is set to 1, wt_update() will return 0, promting the
	 * programm to exit.
	 */
	s8 quit;

	/* Pointer to the active window, or NULL if none is active */
	struct wt_window *active_window;
};



WT_API struct wt_core_container g_wt_core;



/*
 * Reset everything in the core. This will not free any memory, but just
 * overwrite everything with zeros.
 */
WT_API void wt_core_reset(void);


/*
 * This will set the quit flag to 1, which will prompt the wt_update() function
 * to return 0 and by doing so exit the program.
 */
WT_API void wt_core_quit(void);


/*
 * Check if the quit flag has been set.
 *
 * Returns: The value of the quit flag
 */
WT_API s8 wt_core_check_quit(void);


/*
 * Set the main window.
 *
 * @win: Pointer to the main window
 */
WT_API void wt_core_set_main_window(struct wt_window *win);


/*
 * Get the main window.
 *
 * Returns: Pointer to the main window
 */
WT_API struct wt_window *wt_core_get_main_window(void);


/*
 * Set the active window.
 *
 * @win: Pointer to the active window
 */
WT_API void wt_core_set_active_window(struct wt_window *win);


/*
 * Get the current active window.
 *
 * Returns: Pointer to the active window
 */
WT_API struct wt_window *wt_core_get_active_window(void);


/*
 * Check if the given window is the active window.
 *
 * @win: Pointer to the window
 *
 * Returns: 1 if the given window is the active one and 0 if not
 */
WT_API s8 wt_core_is_active_window(struct wt_window *win);

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
WT_API s8 wt_Init(void);


/*
 * Shutdown the FH-framework and clear the allocated memory.
 */
WT_API void wt_Quit(void);


/*
 * Handle inputs, update and redraw the window.
 *
 * Returns: 1 if everything is normal, and 0 if either a fatal error occurred or
 * 	    the user requested to close the program
 */
WT_API s8 wt_Update(void);

#endif /* _WT_CORE_H */
