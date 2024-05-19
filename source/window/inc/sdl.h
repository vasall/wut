#ifndef _WT_WINDOW_SUBSYSTEMS_SDL_H
#define _WT_WINDOW_SUBSYSTEMS_SDL_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_R_MASK 0xff000000
#define SDL_G_MASK 0x00ff0000
#define SDL_B_MASK 0x0000ff00
#define SDL_A_MASK 0x000000ff
#else
#define SDL_R_MASK 0x000000ff
#define SDL_G_MASK 0x0000ff00
#define SDL_B_MASK 0x00ff0000
#define SDL_A_MASK 0xff000000
#endif


/*
 * Initialize the SDL-API.
 *
 * Returns: 0 on success or -1 if an error occurred
 *
 */
WT_API s8 wt_sdl_init(void); 


/*
 * Shutdown the SDL-API.
 */
WT_API void wt_sdl_quit(void);


/*
 * Print relevant information about SDL and it parts in the console.
 */
WT_API void wt_sdl_print_info(void);


#endif /* _WT_WINDOW_SUBSYSTEMS_SDL_H */
