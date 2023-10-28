#ifndef _FH_EVENT_LISTENER_H
#define _FH_EVENT_LISTENER_H

#include "stdinc.h"

#include "event.h"
#include "event_types.h"

#define FH_EVENT_LISTENER_NAME_LIMIT	128


struct fh_event_listener;
struct fh_event_listener {
	char name[FH_EVENT_LISTENER_NAME_LIMIT];

	enum fh_event_type type;

	struct fh_event_listener *next;
	struct fh_event_listener *below;

	fh_evt_cfnc fnc;

	void *data;
};


struct fh_event_handler {
	s8 number;

	struct fh_event_listener *listener;
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
FH_XMOD struct fh_event_handler *fh_handler_create(void);


/*
 * Destroy an event handler and free the allocated memory.
 *
 * @hdl: Pointer to the event handler
 */
FH_XMOD void fh_handler_destroy(struct fh_event_handler *hdl);


/*
 * Add a new event listener to the handler.
 *
 * @hdl: Pointer to the event handler
 * @lst: The new event listener to add
 */
FH_XMOD void fh_handler_add(struct fh_event_handler *hdl, 
		struct fh_event_listener *lst);


/*
 * Remove an event listener from the event handler.
 *
 * @hdl: Pointer to the event handler
 * @type: The type of the event listener
 * @name: The name of the event listener
 */
FH_XMOD void fh_handler_remove(struct fh_event_handler *hdl,
		enum fh_event_type type, char *name);

/*
 * Throw the event at the event handler and see if it sticks.
 *
 * @hdl: Pointer to the event handler
 * @evt: The event to handle
 *
 * Returns: 1 if the event has been handled, and 0 if not
 */
FH_XMOD s8 fh_handler_rundow(struct fh_event_handler *hdl,
		struct fh_event *evt);


/*
 * Get the handler of a given object(element/window).
 *
 * @ptr: Pointer to the object
 *
 * Returns: Either a pointer to the handler or NULL if an error occurred
 */
FH_XMOD struct fh_event_handler *fh_handler_retrieve(void *ptr);

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
FH_API s8 fh_BindEventListener(struct fh_event_handler *hdl,
		enum fh_event_type type, char *name, fh_evt_cfnc fnc,
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
FH_API void fh_UnbindEventListener(struct fh_event_handler *hdl,
		enum fh_event_type type, char *name);


#endif /* _FH_EVENT_LISTENER_H */
