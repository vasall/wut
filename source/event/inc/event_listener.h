#ifndef _WT_EVENT_LISTENER_H
#define _WT_EVENT_LISTENER_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "event/inc/event.h"
#include "event/inc/event_types.h"

#define WT_EVENT_LISTENER_NAME_LIMIT	128


struct wt_event_listener;
struct wt_event_listener {
	char name[WT_EVENT_LISTENER_NAME_LIMIT];

	enum wt_event_type type;

	struct wt_event_listener *next;
	struct wt_event_listener *below;

	wt_evt_cfnc fnc;

	void *data;
};


struct wt_event_handler {
	s8 number;

	struct wt_event_listener *listener;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Create a new event handler so event listeners can be added.
 *
 * Returns: Either a pointer to the newly created event handler or NULL if an
 *          error occurred
 */
WT_XMOD struct wt_event_handler *wt_handler_create(void);


/*
 * Destroy an event handler and free the allocated memory.
 *
 * @hdl: Pointer to the event handler
 */
WT_XMOD void wt_handler_destroy(struct wt_event_handler *hdl);


/*
 * Add a new event listener to the handler.
 *
 * @hdl: Pointer to the event handler
 * @lst: The new event listener to add
 */
WT_XMOD void wt_handler_add(struct wt_event_handler *hdl, 
		struct wt_event_listener *lst);


/*
 * Remove an event listener from the event handler.
 *
 * @hdl: Pointer to the event handler
 * @type: The type of the event listener
 * @name: The name of the event listener
 */
WT_XMOD void wt_handler_remove(struct wt_event_handler *hdl,
		enum wt_event_type type, char *name);

/*
 * Throw the event at the event handler and see if it sticks.
 *
 * @hdl: Pointer to the event handler
 * @evt: The event to handle
 *
 * Returns: 1 if the event has been handled, and 0 if not
 */
WT_XMOD s8 wt_handler_rundow(struct wt_event_handler *hdl,
		struct wt_event *evt);


/*
 * Get the handler of a given object(element/window).
 *
 * @ptr: Pointer to the object
 *
 * Returns: Either a pointer to the handler or NULL if an error occurred
 */
WT_XMOD struct wt_event_handler *wt_handler_retrieve(void *ptr);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Bind a custom event listener. If an event of the given type occurrs, the
 * listener will pick it up and call the attached function.
 *
 * @hdl: Pointer to the event handler
 * @type: The type of event to handle
 * @name: The name of the event listener
 * @fnc: Pointer to the handler function
 * @data: An optional pointer that will be passed to the callback function
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_BindEventListener(struct wt_event_handler *hdl,
		enum wt_event_type type, char *name, wt_evt_cfnc fnc,
		void *data);


/*
 * This function will remove a specifiy event listener from either an element or
 * window. The listener will be selected by type and name. If not event listener
 * could be found, nothing is done.
 *
 * @hdl: Pointer to the event handler
 * @type: The type of the event listener
 * @name: The name of the event listener
 */
WT_API void wt_UnbindEventListener(struct wt_event_handler *hdl,
		enum wt_event_type type, char *name);


#endif /* _WT_EVENT_LISTENER_H */
