#ifndef _XWIN_EVENT_H
#define _XWIN_EVENT_H

#include "define.h"
#include "imports.h"
#include "window.h"


#define XWIN_EVT_PIPE_LIM	64


struct xwin_event {
	u8 type;
};


/* Typedefine the standard callback function for events */
typedef void (*xwin_event_fnc_t)(struct xwin_event evt, void *data);



struct xwin_event_pipe {
	s16 num;
	s16 off;
	struct xwin_event evts[XWIN_EVT_PIPE_LIM];
};



/*
 * Reset the event pipe.
 */
XWIN_API void xwin_evt_pipe_clear(void);


/*
 * Append a new event to the end of the event pipe.
 *
 * @evt: The event to append
 *
 * Returns: Either the new length of the pipe or -1 if an error occurred
 */
XWIN_API s8 xwin_evt_pipe_append(struct xwin_event evt);


/*
 * Pull the oldest entry from the pipe and write it to the given pointer.
 *
 * @evt: A pointer to write the pulled event to
 *
 * Returns: Either 1 if an event has been pulled, 0 if the pipe is empty and -1
 *          if an error occurred
 */
XWIN_API s8 xwin_evt_pipe_pull(struct xwin_event *evt);




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
XWIN_API s16 xwin_evt_win_bind(struct xwin_window *win, u32 type,
		xwin_event_fnc_t fnc);


/*
 * Unbind a specific window event callback.
 *
 * @win: Pointer to the window
 * @eid: The id of the event callback to remove
 */
XWIN_API void xwin_evt_win_unbind(struct xwin_window *win, s16 eid);


/*
 * Unbind all window event callbacks from the given type.
 *
 * @win: Pointer to the window
 * @type: The type of the event
 */
XWIN_API void xwin_evt_win_unbind_type(struct xwin_window *win, u32 type);


/*
 * Unbind all window event callbacks.
 *
 * @win: Pointer to the window
 */
XWIN_API void xwin_evt_win_unbind_all(struct xwin_window *win);



/*
 * Process all incoming SDL events and distribute them to the according places.
 * If an event can't be handeld internally, it will be appended to the event
 * pipe, so it can be handeld manually.
 */
XWIN_API void xwin_evt_process(void);


#endif /* _XWIN_EVENT_H */
