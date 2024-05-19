#ifndef _WT_EVENT_H
#define _WT_EVENT_H


#include "core/inc/define.h"
#include "core/inc/import.h"


struct wt_event;

/* The default event callback functions */
typedef s8 (*wt_evt_cfnc)(struct wt_event *, void *);

#include "window/inc/window.h"

/* Catch-all */
#include "event/inc/event_default.h"
#include "event/inc/event_listener.h"
#include "event/inc/event_types.h"
#include "event/inc/trigger.h"


#define WT_EVT_PIPE_LIM	64

#define WT_EVT_DATA_LIM	128

#define WT_EVT_QUIT		1


/*
 * EVENT-FLAGS
 */
#define WT_EVT_F_PDEF		(1<<0)	/* Prevent-Default */




struct wt_event_data {
	u8			buffer[128];
};

struct wt_event_context {
	struct wt_window 	*window;
	struct wt_element 	*element;
};

struct wt_event {
	enum wt_event_type 	type;

	SDL_Event 		raw;

	struct wt_event_context	context;

	struct wt_event_data	data;

	u8 			flags;
};






/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Process all event in the SDL event queue and handle them.
 */
WT_XMOD void wt_event_update(void);


/*
 * Trigger a new event so it can be handled.
 */
WT_XMOD void wt_event_trigger(struct wt_event *evt);


/*
 * Trigger a custom event without the underlying SDL-event.
 *
 * @type: The type of the event
 * @win: Pointer to the window context
 * @ele: Pointer to the element context
 */
WT_XMOD void wt_event_trigger_raw(enum wt_event_type type,
		struct wt_window *win, struct wt_element *ele);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Prevent the default event handling for this event. This function should be
 * called in a handler callback function, if needed.
 *
 * @evt: Pointer to the event
 */
WT_API void wt_PreventDefault(struct wt_event *evt);


#endif /* _WT_EVENT_H */
