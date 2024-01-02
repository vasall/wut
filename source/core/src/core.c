#include "core/inc/core.h"

#include "utility/alarm/inc/alarm.h"

#include "system/inc/system.h"

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

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_Init(void)
{
	/* Reset the core */
	fh_core_reset();

	/* Then initialize the SDL-frameworks */
	if(fh_sdl_init() < 0) {
		FH_ALARM(FH_ERROR, "Failed to initialize SDL");
		goto err_return;
	}

	/* Initialize OpenGL */
	if(fh_gl_init() < 0) {
		FH_ALARM(FH_ERROR, "Failed to initialize OpenGL");
		goto err_quit_sdl;
	}

	return 0;

err_quit_sdl:
	fh_sdl_quit();

err_return:
	/* Reset te core */
	fh_core_reset();

	FH_ALARM(FH_ERROR, "Failed to initialize the freihand framework");
	return -1;
}


FH_API void fh_Quit(void)
{
	/* Close all windows */
	fh_CloseWindow(fh_core_get_main_window());
	fh_core_set_main_window(NULL);
	
	/* Shutdown SDL */
	fh_sdl_quit();

	/* Reset the core */
	fh_core_reset();
}


FH_API s8 fh_Update(void)
{
	/* Check if the quit flag has been triggered */
	if(fh_core_check_quit())
		return 0;

	fh_event_update();

	/* Redraw all visible windows */
	fh_window_redraw_all();

	return 1;
}

