#include "window/inc/sdl.h"

#include "window/inc/opengl.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include "core/inc/core.h"

#include <stdlib.h>


WT_API s8 wt_sdl_init(void)
{
	s32 imgFlags = IMG_INIT_PNG;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		WT_ALARM(WT_ERROR, "Failed to initialize SDL");
		WT_ALARM(WT_ERROR, SDL_GetError());
		goto err_return;
	}

	if(TTF_Init() < 0) {
		WT_ALARM(WT_ERROR, "Failed to initialize SDL_TTF");
		goto err_quit_sdl;
	}

	if(!(IMG_Init(imgFlags) & imgFlags)) {
		WT_ALARM(WT_ERROR, "Failed to initialize SDL_IMG");
		goto err_quit_sdl;
	}
	
	return 0;

err_quit_sdl:
	SDL_Quit();

err_return:
	WT_ALARM(WT_ERROR, "Failed to initialize SDL module");
	return -1;
}


WT_API void wt_sdl_quit(void)
{
	if(TTF_WasInit())
		TTF_Quit();

	SDL_Quit();
}


WT_API void wt_sdl_print_info(void)
{
	SDL_version sdl_comp;
	SDL_version sdl_link;
	SDL_version img_comp;
	const SDL_version *img_link;
	SDL_version ttf_comp;

	/* Get the TTF version */
	const SDL_version *ttf_link = TTF_Linked_Version();
	SDL_TTF_VERSION(&ttf_comp);

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

	printf("TTF comp. Version: %d.%d.%d\n", 
			ttf_comp.major, ttf_comp.minor, ttf_comp.patch);
	printf("TTF link. Version: %d.%d.%d\n", 
			ttf_link->major, ttf_link->minor, ttf_link->patch);
	printf("--------------------------------------------------\n");
}

