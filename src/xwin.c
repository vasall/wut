#include "xwin.h"

#include <stdlib.h>



XWIN_API s8 xwin_init(void)
{
	/* Reset the core */
	xwin_core_reset();

	/* Then initialize the SDL-frameworks */
	if(xwin_sdl_init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize the XWIN-SDL module");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize XWIN framework");
	return -1;
}


XWIN_API void xwin_quit(void)
{
	/* Close all windows */
	xwin_win_close(g_xwin_core.main_window);
	g_xwin_core.main_window = NULL;
	
	/* Shutdown SDL */
	xwin_sdl_quit();

	/* Reset the core */
	xwin_core_reset();
}


XWIN_API s32 xwin_add_window(s32 parent, char *name, s32 width, s32 height)
{
	struct xwin_window *win;
	struct xwin_window *par;
		
	if(parent < 0 || name == NULL || width < 0 || height < 0) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* First find the parent to see if it even exists */
	if(parent != 0 && !(par = xwin_win_get(parent))) {
		ALARM(ALARM_ERR, "Parent could not be found");
		goto err_return;
	}

	/* Then create a new window */
	if(!(win = xwin_win_create(name, width, height)))  {
		ALARM(ALARM_ERR, "Failed to create new window");
		goto err_return;
	}

	/* Lastly attach it */
	if(parent == 0) {
		g_xwin_core.main_window = win;
	}
	else {
		xwin_win_attach(par, win);
	}

	return win->id;
		

err_return:
	ALARM(ALARM_ERR, "Failed to create add new window");
	return -1;
}



XWIN_API s8 xwin_update(void)
{
	SDL_Event evt;

	/* Go through all outstanding events */
	while(SDL_PollEvent(&evt)) {

	}

	return 0;
}


XWIN_API void xwin_render(void)
{

}
