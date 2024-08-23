#ifndef _WUT_CORE_H
#define _WUT_CORE_H

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


struct wut_coreContainer {
	/* The main window and by that all windows attached to it */
	struct wut_Window *main_window;

	/* 
	 * If this var is set to 1, wut_update() will return 0, promting the
	 * programm to exit.
	 */
	s8 quit;

	/* Pointer to the active window, or NULL if none is active */
	struct wut_Window *active_window;
};


WUT_API struct wut_coreContainer _wut_coreContainer;





/*
 * This will set the quit flag to 1, which will prompt the wut_update() function
 * to return 0 and by doing so exit the program.
 */
WUT_XMOD void wut_cor_quit(void);


/*
 * Check if the quit flag has been set.
 *
 * Returns: The value of the quit flag
 */
WUT_XMOD s8 wut_cor_check_quit(void);


/*
 * Set the main window.
 *
 * @win: Pointer to the main window
 */
WUT_XMOD void wut_cor_set_main_window(struct wut_Window *win);


/*
 * Get the main window.
 *
 * Returns: Pointer to the main window
 */
WUT_XMOD struct wut_Window *wut_cor_get_main_window(void);


/*
 * Set the active window.
 *
 * @win: Pointer to the active window
 */
WUT_XMOD void wut_cor_set_active_window(struct wut_Window *win);


/*
 * Get the current active window.
 *
 * Returns: Pointer to the active window
 */
WUT_XMOD struct wut_Window *wut_cor_get_active_window(void);


/*
 * Check if the given window is the active window.
 *
 * @win: Pointer to the window
 *
 * Returns: 1 if the given window is the active one and 0 if not
 */
WUT_XMOD s8 wut_cor_is_active_window(struct wut_Window *win);

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
WUT_API s8 wut_Init(void);


/*
 * Shutdown the framework and clear the allocated memory.
 */
WUT_API void wut_Quit(void);


/*
 * Handle inputs, update and redraw the window.
 *
 * Returns: 1 if everything is normal, and 0 if either a fatal error occurred or
 * 	    the user requested to close the program
 */
WUT_API s8 wut_Update(void);

#endif /* _WUT_CORE_H */
