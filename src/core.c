#include "core.h"


#include <stdlib.h>


struct fh_core_container g_fh_core;



FH_API void fh_core_reset(void)
{
	/* Reset pointer to main window */
	g_fh_core.main_window = NULL;

	/* Clear the event pipe */
	fh_evt_pipe_clear();
}
