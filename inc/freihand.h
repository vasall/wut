#ifndef _FH_FREIHAND_H
#define _FH_FREIHAND_H

#include "define.h"
#include "imports.h"
#include "core.h"
#include "sdl.h"
#include "window.h"


/*
 * Initialize the FH-framework.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_init(void);


/*
 * Shutdown the FH-framework and clear the allocated memory.
 */
FH_API void fh_quit(void);


/*
 * Create a new window and attach it to the parent. If 0 is given, it will be
 * considered the main window.
 *
 * @parent: The windowId of the parent window
 * @name: The name of the window
 * @width: The initial width of the window
 * @height: The intial height of the window
 *
 * Returns: Either the windowId or -1 if an error occurred
 */
FH_API s32 fh_add_window(s32 parent, char *name, s32 width, s32 height);


/*
 * Handle inputs and update the window.
 * If this function returns an error or the return code 1, the window should be
 * closed.
 *
 * Returns: 1 if everything is normal, -1 if an error occurred and 1 if the user
 *          requested to close the program
 */
FH_API s8 fh_update(void);


/*
 * Redraw the window.
 */
FH_API void fh_render(void);


/*
 * Pull the oldest event from the event pipe.
 *
 * @event: A pointer to write the pulled event to, if there is any
 *
 * Returns: 1 if an event has been returned, 0 if there are no more events and
 * 	    -1 if an error occurred
 */
FH_API s8 fh_pull_event(struct fh_event *event);


#endif /* _FH_FREIHAND_H */
