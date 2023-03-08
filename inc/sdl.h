#ifndef _XWIN_SDL_H
#define _XWIN_SDL_H

#include "define.h"
#include "imports.h"

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
XWIN_API s8 xwin_sdl_init(void); 


/*
 * Shutdown the SDL-API.
 */
XWIN_API void xwin_sdl_quit(void);


#endif /* _XWIN_SDL_H */
