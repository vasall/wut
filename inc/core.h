#ifndef _FH_CORE_H
#define _FH_CORE_H


#include "define.h"
#include "imports.h"
#include "window.h"
#include "event.h"


struct fh_core_container {
	/* The main window */
	struct fh_window *main_window;

	/* The event pipe containing all unhandled events */
	struct fh_event_pipe event_pipe;
};



FH_API struct fh_core_container g_fh_core;



/*
 * Reset everything in the core. This will not free any memory, but just
 * overwrite everything with zeros.
 */
FH_API void fh_core_reset(void);





#endif /* _FH_CORE_H */
