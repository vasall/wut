#include "core/inc/core.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdlib.h>


struct wut_coreContainer _wut_coreContainer;




/*
 * Reset everything in the core. This will not free any memory, but just
 * overwrite everything with zeros and NULLs, so be careful.
 */
WUT_INTERN void cor_reset(void)
{
	/* Reset the quit flag */
	_wut_coreContainer.quit = 0;

	/* Reset pointer to main window */
	wut_cor_set_main_window(NULL);

	/* Reset active window */
	wut_cor_set_active_window(NULL);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_XMOD void wut_cor_quit(void)
{
	_wut_coreContainer.quit = 1;
}


WUT_XMOD s8 wut_cor_check_quit(void)
{
	return _wut_coreContainer.quit;
}


WUT_XMOD void wut_cor_set_main_window(struct wut_Window *win)
{
	_wut_coreContainer.main_window = win;
}


WUT_XMOD struct wut_Window *wut_cor_get_main_window(void)
{
	return _wut_coreContainer.main_window;
}


WUT_XMOD void wut_cor_set_active_window(struct wut_Window *win)
{
	if(win)
		printf("%s is not the active window\n", win->name);
	else
		printf("no more active window\n");

	_wut_coreContainer.active_window = win;
}


WUT_XMOD struct wut_Window *wut_cor_get_active_window(void)
{
	return _wut_coreContainer.active_window;
}


WUT_XMOD s8 wut_cor_is_active_window(struct wut_Window *win)
{
	if(_wut_coreContainer.active_window == NULL)
		return 0;

	if(strcmp(win->name, _wut_coreContainer.active_window->name) == 0)
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

WUT_API s8 wut_Init(void)
{
	/* Reset the core */
	cor_reset();

	/* Then initialize the SDL-frameworks */
	if(wut_sdl_init() < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to initialize SDL");
		goto err_return;
	}

	/* Initialize OpenGL */
	if(wut_gl_init() < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to initialize OpenGL");
		goto err_quit_sdl;
	}

	return 0;

err_quit_sdl:
	wut_sdl_quit();

err_return:
	/* Reset te core */
	cor_reset();

	WUT_ALARM(WUT_ERROR, "Failed to initialize the freihand framework");
	return -1;
}


WUT_API void wut_Quit(void)
{
	/* Close all windows */
	wut_CloseWindow(wut_cor_get_main_window());
	wut_cor_set_main_window(NULL);
	
	/* Shutdown SDL */
	wut_sdl_quit();

	/* Reset the core */
	cor_reset();
}


WUT_API s8 wut_Update(void)
{
	/* Check if the quit flag has been triggered */
	if(wut_cor_check_quit())
		return 0;

	wut_evt_update();

	/* Redraw all visible windows */
	wut_win_redraw_all();

	return 1;
}

