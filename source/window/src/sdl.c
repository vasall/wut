#include "source/window/inc/sdl.h"

#include "source/window/inc/opengl.h"

#include "source/utility/inc/alarm.h"

#include "source/system/inc/system.h"

#include "source/core/inc/core.h"

#include <stdlib.h>


WUT_API s8 wut_sdl_init(void)
{
	s32 imgFlags = IMG_INIT_PNG;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to initialize SDL");
		WUT_ALARM(WUT_ERROR, SDL_GetError());
		goto err_return;
	}

	if(!(IMG_Init(imgFlags) & imgFlags)) {
		WUT_ALARM(WUT_ERROR, "Failed to initialize SDL_IMG");
		goto err_quit_sdl;
	}
	
	return 0;

err_quit_sdl:
	SDL_Quit();

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to initialize SDL module");
	return -1;
}


WUT_API void wut_sdl_quit(void)
{
	IMG_Quit();

	SDL_Quit();
}


WUT_API void wut_sdl_print_info(void)
{
	SDL_version sdl_comp;
	SDL_version sdl_link;
	SDL_version img_comp;
	const SDL_version *img_link;

	/* Get the Image version */
	img_link = IMG_Linked_Version();
	SDL_IMAGE_VERSION(&img_comp);

	/* Get the SDL version */
	SDL_VERSION(&sdl_comp);
	SDL_GetVersion(&sdl_link);

	printf("-------------------- SDL Info --------------------\n");
	printf("SDL comp. Version: %d.%d.%d\n", 
			sdl_comp.major, sdl_comp.minor, sdl_comp.patch);
	printf("SDL link. Version: %d.%d.%d\n", 
			sdl_link.major, sdl_link.minor, sdl_link.patch);

	printf("IMG comp. Version: %d.%d.%d\n",
			img_comp.major, img_comp.minor, img_comp.patch);
	printf("IMG link. Version: %d.%d.%d\n",
			img_link->major, img_link->minor, img_link->patch);

	printf("--------------------------------------------------\n");
}

