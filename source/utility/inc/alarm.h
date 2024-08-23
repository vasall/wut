#ifndef _WUT_ALARM_H
#define _WUT_ALARM_H

#include "core/inc/define.h"


#define WUT_ALARM_FILE "log.txt"


enum wut_eAlarmPriority {
	WUT_NOTICE,
	WUT_WARNING,
	WUT_ERROR
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
WUT_API int wut_alarm_log(enum wut_eAlarmPriority prio, const char *format, ...);


/*
 * Same as pnc_log, but instead of having the option to use a message format,
 * this macro additionally outputs the file and line number, where WUT_ALARM_LOG was
 * called from.
 */
#define WUT_ALARM(prio, msg) wut_alarm_log(prio, "%s:%d %s", __FILE__, __LINE__, msg)



#endif /* _WUT_ALARM_H */
