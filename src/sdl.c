#include "sdl.h"

#include "core.h"
#include "opengl.h"

#include <stdlib.h>


FH_API s8 fh_sdl_init(void)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		ALARM(ALARM_ERR, "Failed to initialize SDL");
		ALARM(ALARM_ERR, SDL_GetError());
		goto err_return;
	}


	if(fh_gl_init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize the SDL GL module");
		goto err_return;
	}
	
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize SDL module");
	return -1;
}


FH_API void fh_sdl_quit(void)
{
	SDL_Quit();
}
