#include "utility/inc/alarm.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


WT_API int wt_alarm_log(enum wt_alarm_prio prio, const char *format, ...)
{
	FILE *file;
	va_list args;
	char *prio_str;

	file = fopen(WT_ALARM_FILE, "a");
	if(!file)
		return -1;

	switch (prio) {
		case WT_NOTICE:
			prio_str = "NOTICE";
			break;
		case WT_WARNING:
			prio_str = "WARNING";
			break;
		case WT_ERROR:
			prio_str = "ERROR";
			break;
		default:
			fclose(file);
			return -1;
			break;
	}

	fprintf(file, "%s | ", prio_str);

	va_start(args, format);
	vfprintf(file, format, args);
	va_end(args);

	fputc('\n', file);

	fclose(file);

	return 0;
}

