#include "freihand.h"

#include "alarm.h"
#include "system.h"
#include "document.h"
#include "shader.h"

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

	/* Initialize OpenGL */
	if(fh_gl_init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize the SDL GL module");
		goto err_quit_sdl;
	}

	return 0;

err_quit_sdl:
	fh_sdl_quit();

err_return:
	/* Reset te core */
	fh_core_reset();

	ALARM(ALARM_ERR, "Failed to initialize FH framework");
	return -1;
}


FH_API void fh_quit(void)
{
	/* Close all windows */
	fh_win_close(fh_core_get_main_window());
	fh_core_set_main_window(NULL);
	
	/* Shutdown SDL */
	fh_sdl_quit();

	/* Reset the core */
	fh_core_reset();
}


FH_API s8 fh_update(void)
{
	/* Check if the quit flag has been triggered */
	if(fh_core_check_quit())
		return 0;

	fh_evt_process();



	/* Redraw all visible windows */
	fh_win_redraw_all();

	return 1;
}

