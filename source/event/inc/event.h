#ifndef _WUT_EVENT_H
#define _WUT_EVENT_H


#include "source/core/inc/define.h"
#include "source/core/inc/import.h"


struct wut_Event;

/* The default event callback functions */
typedef s8 (*wut_EventFunc)(struct wut_Event *, void *);

#include "source/window/inc/window.h"

#include "source/math/inc/vector.h"

/* Catch-all */
#include "source/event/inc/event_default.h"
#include "source/event/inc/event_listener.h"
#include "source/event/inc/event_type.h"


#define WUT_EVT_PIPE_LIM	64

#define WUT_EVT_DATA_LIM	128

#define WUT_EVT_QUIT		1


/*
 * EVENT-FLAGS
 */
#define WUT_EVT_F_PDEF		(1<<0)	/* Prevent-Default */




struct wut_EventData {
	u8			buffer[128];
};

struct wut_EventContext {
	wut_iVec2               position;
        struct wut_Window 	*window;
	struct wut_Element 	*element;
};

struct wut_Event {
	enum wut_eEventType 	type;

	SDL_Event 		raw;

	struct wut_EventContext	context;

	struct wut_EventData	data;

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
WUT_XMOD void wut_evt_update(void);


/*
 * Trigger a new event so it can be handled.
 */
WUT_XMOD void wut_evt_trigger(struct wut_Event *evt);


/*
 * Trigger a custom event without the underlying SDL-event.
 *
 * @type: The type of the event
 * @win: Pointer to the window context
 * @ele: Pointer to the element context
 */
WUT_XMOD void wut_evt_trigger_raw(enum wut_eEventType type,
		struct wut_Window *win, struct wut_Element *ele);


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
WUT_API void wut_PreventDefault(struct wut_Event *evt);


#endif /* _WUT_EVENT_H */
