#include "sdl.h"

#include "core.h"


#include <stdlib.h>


XWIN_API s8 xwin_sdl_init(void)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		ALARM(ALARM_ERR, "Failed to initialize SDL");
		ALARM(ALARM_ERR, SDL_GetError());
		return -1;
	}

	return 0;
}


XWIN_API void xwin_sdl_quit(void)
{
	SDL_Quit();
}
