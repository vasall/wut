#ifndef _FH_EVENT_H
#define _FH_EVENT_H


#include "define.h"
#include "datatype.h"
#include "import.h"


struct fh_event;

/* The default event callback functions */
typedef s8 (*fh_evt_cfnc)(struct fh_event *, void *);

#include "window.h"
#include "event_types.h"
#include "event_listener.h"

#define FH_EVT_PIPE_LIM	64

#define FH_EVT_DATA_LIM	128

#define FH_EVT_QUIT		1


/*
 * EVENT-FLAGS
 */
#define FH_EVT_F_PDEF		(1<<0)	/* Prevent-Default */




struct fh_event_data {
	u8			buffer[128];
};

struct fh_event_context {
	struct fh_window 	*window;
	struct fh_element 	*element;
};

struct fh_event {
	enum fh_event_type 	type;

	SDL_Event 		raw;

	struct fh_event_context	context;

	struct fh_event_data	data;

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
FH_XMOD void fh_event_update(void);


/*
 * Trigger a new event so it can be handled.
 */
FH_XMOD void fh_event_trigger(struct fh_event *evt);


/*
 * Trigger a custom event without the underlying SDL-event.
 *
 * @type: The type of the event
 * @win: Pointer to the window context
 * @ele: Pointer to the element context
 */
FH_XMOD void fh_event_trigger_raw(enum fh_event_type type,
		struct fh_window *win, struct fh_element *ele);


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
FH_API void fh_PreventDefault(struct fh_event *evt);


#endif /* _FH_EVENT_H */
