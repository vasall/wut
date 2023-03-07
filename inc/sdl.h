#ifndef _XWIN_SDL_H
#define _XWIN_SDL_H

#include "define.h"
#include "imports.h"

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
