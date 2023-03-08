#include "core.h"


#include <stdlib.h>


struct xwin_core_container g_xwin_core;



XWIN_API void xwin_core_reset(void)
{
	/* Reset pointer to main window */
	g_xwin_core.main_window = NULL;

	/* Clear the event pipe */
	xwin_evt_pipe_clear();
}
