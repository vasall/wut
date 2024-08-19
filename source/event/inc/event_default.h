#ifndef _WUT_EVENT_DEFAULT_H
#define _WUT_EVENT_DEFAULT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "event/inc/event.h"

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * A splitter function that takes the given event and directs it into the right
 * path depending on the type.
 *
 * @evt: Pointer to the event
 *
 * Returns: 1 if the event has been handled, 0 if not
 */
WUT_XMOD s8 wut_evd_rundown(struct wut_Event *evt);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


#endif /* _WUT_EVENT_DEFAULT_H */
