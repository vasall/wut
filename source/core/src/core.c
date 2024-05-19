#include "core/inc/core.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdlib.h>


struct wt_core_container g_wt_core;



WT_API void wt_core_reset(void)
{
	/* Reset the quit flag */
	g_wt_core.quit = 0;

	/* Reset pointer to main window */
	wt_core_set_main_window(NULL);

	/* Reset active window */
	wt_core_set_active_window(NULL);
}


WT_API void wt_core_quit(void)
{
	g_wt_core.quit = 1;
}


WT_API s8 wt_core_check_quit(void)
{
	return g_wt_core.quit;
}


WT_API void wt_core_set_main_window(struct wt_window *win)
{
	g_wt_core.main_window = win;
}


WT_API struct wt_window *wt_core_get_main_window(void)
{
	return g_wt_core.main_window;
}


WT_API void wt_core_set_active_window(struct wt_window *win)
{
	if(win)
		printf("%s is not the active window\n", win->name);
	else
		printf("no more active window\n");

	g_wt_core.active_window = win;
}


WT_API struct wt_window *wt_core_get_active_window(void)
{
	return g_wt_core.active_window;
}


WT_API s8 wt_core_is_active_window(struct wt_window *win)
{
	if(g_wt_core.active_window == NULL)
		return 0;

	if(strcmp(win->name, g_wt_core.active_window->name) == 0)
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

WT_API s8 wt_Init(void)
{
	/* Reset the core */
	wt_core_reset();

	/* Then initialize the SDL-frameworks */
	if(wt_sdl_init() < 0) {
		WT_ALARM(WT_ERROR, "Failed to initialize SDL");
		goto err_return;
	}

	/* Initialize OpenGL */
	if(wt_gl_init() < 0) {
		WT_ALARM(WT_ERROR, "Failed to initialize OpenGL");
		goto err_quit_sdl;
	}

	return 0;

err_quit_sdl:
	wt_sdl_quit();

err_return:
	/* Reset te core */
	wt_core_reset();

	WT_ALARM(WT_ERROR, "Failed to initialize the freihand framework");
	return -1;
}


WT_API void wt_Quit(void)
{
	/* Close all windows */
	wt_CloseWindow(wt_core_get_main_window());
	wt_core_set_main_window(NULL);
	
	/* Shutdown SDL */
	wt_sdl_quit();

	/* Reset the core */
	wt_core_reset();
}


WT_API s8 wt_Update(void)
{
	/* Check if the quit flag has been triggered */
	if(wt_core_check_quit())
		return 0;

	wt_event_update();

	/* Redraw all visible windows */
	wt_window_redraw_all();

	return 1;
}

