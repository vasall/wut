#include "xwin.h"

#include <stdlib.h>



XWIN_API s8 xwin_init(char *name, s32 width, s32 height)
{
	struct xwin_window *main;

	if(xwin_sdl_init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize the XWIN-SDL module");
		goto err_return;
	}

	/* Create the main window */
	if(!(main = xwin_win_create(name, width, height))) {
		ALARM(ALARM_ERR, "Failed to create main window");
		goto err_shutdown_sdl;
	}

	/* Set the main window */	
	g_cnt.main_window = main;


	return 0;

err_shutdown_sdl:
	xwin_sdl_quit();

err_return:
	ALARM(ALARM_ERR, "Failed to initialize XWIN framework");
	return -1;
}


XWIN_INTERN void xwin_cfnc_destroy_all(struct xwin_window *w, void *data)
{
	/* SILENCE COMPILER */
	if(data) {}

	/* First detach window from parent */
	xwin_win_detach(w);

	/* Then destroy it */
	xwin_win_destroy(w);
}



XWIN_API void xwin_quit(void)
{
	/* Destroy all windows */
	xwin_win_hlfup(g_cnt.main_window, &xwin_cfnc_destroy_all, NULL);	
	g_cnt.main_window = NULL;
	

	/* Shutdown SDL */
	xwin_sdl_quit();
}


XWIN_API s8 xwin_update(void)
{
	
}
