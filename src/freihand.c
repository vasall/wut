#include "freihand.h"

#include <stdlib.h>



FH_API s8 fh_init(void)
{
	/* Reset the core */
	fh_core_reset();

	/* Then initialize the SDL-frameworks */
	if(fh_sdl_init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize the XWIN-SDL module");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize XWIN framework");
	return -1;
}


FH_API void fh_quit(void)
{
	/* Close all windows */
	fh_win_close(g_fh_core.main_window);
	g_fh_core.main_window = NULL;
	
	/* Shutdown SDL */
	fh_sdl_quit();

	/* Reset the core */
	fh_core_reset();
}


FH_API s32 fh_add_window(s32 parent, char *name, s32 width, s32 height)
{
	struct fh_window *win;
	struct fh_window *par;
		
	if(parent < 0 || name == NULL || width < 0 || height < 0) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* First find the parent to see if it even exists */
	if(parent != 0 && !(par = fh_win_get(parent))) {
		ALARM(ALARM_ERR, "Parent could not be found");
		goto err_return;
	}

	/* Then create a new window */
	if(!(win = fh_win_create(name, width, height)))  {
		ALARM(ALARM_ERR, "Failed to create new window");
		goto err_return;
	}

	/* Lastly attach it */
	if(parent == 0) {
		/* Mark this window as the main window */
		win->info = win->info | FH_WIN_INFO_MAIN;

		g_fh_core.main_window = win;
	}
	else {
		fh_win_attach(par, win);
	}

	return win->id;
		

err_return:
	ALARM(ALARM_ERR, "Failed to create add new window");
	return -1;
}



FH_API s8 fh_update(void)
{
	fh_evt_process();

	return 0;
}


FH_API void fh_render(void)
{

}


FH_API s8 fh_pull_event(struct fh_event *event)
{
	return fh_evt_pipe_pull(event);
}
