#ifndef _WUT_EVENT_LISTENER_H
#define _WUT_EVENT_LISTENER_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/event/inc/event.h"
#include "source/event/inc/event_type.h"

#define WUT_LIS_NAME_LIMIT	128


struct wut_EventListener;
struct wut_EventListener {
	char                            name[WUT_LIS_NAME_LIMIT];

	enum wut_eEventType              type;

	struct wut_EventListener        *next;
	struct wut_EventListener        *below;

	wut_EventFunc                   fnc;

	void                            *data;
};


struct wut_EventHandler {
	s8                              number;

	struct wut_EventListener        *listener;
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
WUT_XMOD struct wut_EventHandler *wut_hdl_create(void);


/*
 * Destroy an event handler and free the allocated memory.
 *
 * @hdl: Pointer to the event handler
 */
WUT_XMOD void wut_hdl_destroy(struct wut_EventHandler *hdl);


/*
 * Add a new event listener to the handler.
 *
 * @hdl: Pointer to the event handler
 * @lst: The new event listener to add
 */
WUT_XMOD void wut_hdl_add(struct wut_EventHandler *hdl, 
		struct wut_EventListener *lst);


/*
 * Remove an event listener from the event handler.
 *
 * @hdl: Pointer to the event handler
 * @type: The type of the event listener
 * @name: The name of the event listener
 */
WUT_XMOD void wut_hdl_remove(struct wut_EventHandler *hdl,
		enum wut_eEventType type, char *name);

/*
 * Throw the event at the event handler and see if it sticks.
 *
 * @hdl: Pointer to the event handler
 * @evt: The event to handle
 *
 * Returns: 1 if the event has been handled, and 0 if not
 */
WUT_XMOD s8 wut_hdl_rundow(struct wut_EventHandler *hdl,
                struct wut_Event *evt);


/*
 * Get the handler of a given object(element/window).
 *
 * @ptr: Pointer to the object
 *
 * Returns: Either a pointer to the handler or NULL if an error occurred
 */
WUT_XMOD struct wut_EventHandler *wut_hdl_retrieve(void *ptr);

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
WUT_API s8 wut_BindEventListener(struct wut_EventHandler *hdl,
		enum wut_eEventType type, char *name, wut_EventFunc fnc,
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
WUT_API void wut_UnbindEventListener(struct wut_EventHandler *hdl,
		enum wut_eEventType type, char *name);


#endif /* _WUT_EVENT_LISTENER_H */
