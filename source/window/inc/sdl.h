#ifndef _FH_WINDOW_SDL_H
#define _FH_WINDOW_SDL_H

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
FH_API s8 fh_sdl_init(void); 


/*
 * Shutdown the SDL-API.
 */
FH_API void fh_sdl_quit(void);


/*
 * Print relevant information about SDL and it parts in the console.
 */
FH_API void fh_sdl_print_info(void);


#endif /* _FH_WINDOW_SDL_H */
