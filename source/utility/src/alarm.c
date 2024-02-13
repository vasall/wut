#include "utility/inc/alarm.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


FH_API int fh_alarm_log(enum fh_alarm_prio prio, const char *format, ...)
{
	FILE *file;
	va_list args;
	char *prio_str;

	file = fopen(FH_ALARM_FILE, "a");
	if(!file)
		return -1;

	switch (prio) {
		case FH_NOTICE:
			prio_str = "NOTICE";
			break;
		case FH_WARNING:
			prio_str = "WARNING";
			break;
		case FH_ERROR:
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

