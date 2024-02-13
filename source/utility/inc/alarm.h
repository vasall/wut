#ifndef _FH_ALARM_H
#define _FH_ALARM_H

#include "core/inc/define.h"


#define FH_ALARM_FILE "log.txt"


enum fh_alarm_prio {
	FH_NOTICE,
	FH_WARNING,
	FH_ERROR
};

/*
 * Write to the log.
 *
 * @prio: The priority of the log message
 * @format: The log message format
 * @...: Like printf; the variables that should be inserted into the message
 *
 * Returns: 0 on success or -1 if an error occurrs
 */
FH_API int fh_alarm_log(enum fh_alarm_prio prio, const char *format, ...);


/*
 * Same as pnc_log, but instead of having the option to use a message format,
 * this macro additionally outputs the file and line number, where FH_ALARM_LOG was
 * called from.
 */
#define FH_ALARM(prio, msg) fh_alarm_log(prio, "%s:%d %s", __FILE__, __LINE__, msg)



#endif /* _FH_ALARM_H */
