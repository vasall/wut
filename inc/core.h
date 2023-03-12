#ifndef _FH_CORE_H
#define _FH_CORE_H


#include "define.h"
#include "import.h"
#include "window.h"
#include "event.h"


struct fh_core_container {
	/* 
	 * If this var is set to 1, fh_update() will return 0, promting the
	 * programm to exit.
	 */
	s8 quit;

	/* The main window */
	struct fh_window *main_window;

	/* The event pipe containing all unhandled events */
	struct fh_event_pipe event_pipe;

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
 * As the name suggests, this function will clear the event pipe.
 */
FH_API void fh_core_clear_event_pipe(void);


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


#endif /* _FH_CORE_H */
