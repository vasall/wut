#ifndef _WT_EVENT_DEFAULT_H
#define _WT_EVENT_DEFAULT_H

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
WT_XMOD s8 wt_evtdef_rundown(struct wt_event *evt);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


#endif /* _WT_EVENT_DEFAULT_H */
