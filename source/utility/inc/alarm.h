#ifndef _WT_ALARM_H
#define _WT_ALARM_H

#include "core/inc/define.h"


#define WT_ALARM_FILE "log.txt"


enum wt_alarm_prio {
	WT_NOTICE,
	WT_WARNING,
	WT_ERROR
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
WT_API int wt_alarm_log(enum wt_alarm_prio prio, const char *format, ...);


/*
 * Same as pnc_log, but instead of having the option to use a message format,
 * this macro additionally outputs the file and line number, where WT_ALARM_LOG was
 * called from.
 */
#define WT_ALARM(prio, msg) wt_alarm_log(prio, "%s:%d %s", __FILE__, __LINE__, msg)



#endif /* _WT_ALARM_H */
