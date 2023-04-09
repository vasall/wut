#ifndef _FH_ALARM_H
#define _FH_ALARM_H

#include "define.h"


#define ALARM_FILE "log.txt"


enum alarm_prio {
	ALARM_NOTICE,
	ALARM_WARN,
	ALARM_ERR
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
FH_API int alarm_log(enum alarm_prio prio, const char *format, ...);


/*
 * Same as pnc_log, but instead of having the option to use a message format,
 * this macro additionally outputs the file and line number, where ALARM_LOG was
 * called from.
 */
#define ALARM(prio, msg) alarm_log(prio, "%s:%d %s", __FILE__, __LINE__, msg)



#endif /* FH_ALARM_H */
