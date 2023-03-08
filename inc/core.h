#ifndef _XWIN_CORE_H
#define _XWIN_CORE_H


#include "define.h"
#include "imports.h"
#include "window.h"
#include "event.h"


struct xwin_core_container {
	/* The main window */
	struct xwin_window *main_window;

	/* The event pipe containing all unhandled events */
	struct xwin_event_pipe event_pipe;
};



XWIN_API struct xwin_core_container g_xwin_core;



/*
 * Reset everything in the core. This will not free any memory, but just
 * overwrite everything with zeros.
 */
XWIN_API void xwin_core_reset(void);





#endif /* _XWIN_CORE_H */
