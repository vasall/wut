#include "core.h"

#include "system.h"

#include <stdlib.h>


struct fh_core_container g_fh_core;



FH_API void fh_core_reset(void)
{
	/* Reset the quit flag */
	g_fh_core.quit = 0;

	/* Reset pointer to main window */
	fh_core_set_main_window(NULL);

	/* Reset active window */
	fh_core_set_active_window(NULL);
}


FH_API void fh_core_quit(void)
{
	g_fh_core.quit = 1;
}


FH_API s8 fh_core_check_quit(void)
{
	return g_fh_core.quit;
}


FH_API void fh_core_set_main_window(struct fh_window *win)
{
	g_fh_core.main_window = win;
}


FH_API struct fh_window *fh_core_get_main_window(void)
{
	return g_fh_core.main_window;
}


FH_API void fh_core_set_active_window(struct fh_window *win)
{
	if(win)
		printf("%s is not the active window\n", win->name);
	else
		printf("no more active window\n");

	g_fh_core.active_window = win;
}


FH_API struct fh_window *fh_core_get_active_window(void)
{
	return g_fh_core.active_window;
}


FH_API s8 fh_core_is_active_window(struct fh_window *win)
{
	if(g_fh_core.active_window == NULL)
		return 0;

	if(strcmp(win->name, g_fh_core.active_window->name) == 0)
		return 1;

	return 0;
}
