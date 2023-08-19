#ifndef _FH_EVENT_H
#define _FH_EVENT_H

#include "define.h"
#include "datatype.h"
#include "import.h"
#include "window.h"


#define FH_EVT_PIPE_LIM	64

#define FH_EVT_DATA_LIM	128

#define FH_EVT_QUIT		1


struct fh_event {
	u8 type;

	SDL_Event event;

	struct fh_window *window;
	struct fh_element *element;

	u8 data[128];

	u8 flags;
};


/* Typedefine the standard callback function for events */
typedef void (*fh_event_fnc_t)(struct fh_event evt, void *data);



/*
 * Bind a callback function to a window event.
 *
 * @win: Pointer to the window
 * @type: The type of event to bind the callback function to
 * @fnc: The callback function to call
 *
 * Returns: Either an ID to identify the callback function in the list or -1 if
 *          an error occurred
 */
FH_API s16 fh_evt_win_bind(struct fh_window *win, u32 type,
		fh_event_fnc_t fnc);


/*
 * Unbind a specific window event callback.
 *
 * @win: Pointer to the window
 * @eid: The id of the event callback to remove
 */
FH_API void fh_evt_win_unbind(struct fh_window *win, s16 eid);


/*
 * Unbind all window event callbacks from the given type.
 *
 * @win: Pointer to the window
 * @type: The type of the event
 */
FH_API void fh_evt_win_unbind_type(struct fh_window *win, u32 type);


/*
 * Unbind all window event callbacks.
 *
 * @win: Pointer to the window
 */
FH_API void fh_evt_win_unbind_all(struct fh_window *win);



/*
 * Process all incoming SDL events and distribute them to the according places.
 * If an event can't be handeld internally, it will be appended to the event
 * pipe, so it can be handeld manually.
 */
FH_API void fh_evt_process(void);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */



#endif /* _FH_EVENT_H */
