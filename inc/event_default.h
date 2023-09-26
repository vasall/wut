#ifndef _FH_EVENT_DEFAULT_H
#define _FH_EVENT_DEFAULT_H

#include "define.h"
#include "datatype.h"
#include "import.h"

#include "event.h"

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
FH_XMOD s8 fh_evtdef_rundown(struct fh_event *evt);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


#endif /* _FH_EVENT_DEFAULT_H */
