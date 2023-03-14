#include "core.h"


#include <stdlib.h>


struct fh_core_container g_fh_core;



FH_API void fh_core_reset(void)
{
	/* Reset the quit flag */
	g_fh_core.quit = 0;

	/* Reset pointer to main window */
	fh_core_set_main_window(NULL);

	/* Clear the event pipe */
	fh_core_clear_event_pipe();

	/* Reset active window */
	fh_core_set_active_window(NULL);

	/* Close the shader list */
	fh_shader_close();

	/* Reset the UI shader */
	fh_core_set_ui_shader(NULL);
}


FH_API void fh_core_quit(void)
{
	g_fh_core.quit = 1;
}


FH_API s8 fh_core_check_quit(void)
{
	return g_fh_core.quit;
}


FH_API void fh_core_clear_event_pipe(void)
{
	g_fh_core.event_pipe.off = 0;
	g_fh_core.event_pipe.num = 0;
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
	g_fh_core.active_window = win;
}


FH_API struct fh_window *fh_core_get_active_window(void)
{
	return g_fh_core.active_window;
}


FH_API void fh_core_set_ui_shader(struct fh_shader *shader)
{
	g_fh_core.ui_shader = shader;
}


FH_API struct fh_shader *fh_core_get_ui_shader(void)
{
	return g_fh_core.ui_shader;
}
