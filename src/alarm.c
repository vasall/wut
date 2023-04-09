#include "alarm.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

FH_API int alarm_log(enum alarm_prio prio, const char *format, ...)
{
	FILE *file;
	va_list args;
	char *prio_str;

	file = fopen(ALARM_FILE, "a");
	if(!file)
		return -1;

	switch (prio) {
		case ALARM_NOTICE:
			prio_str = "NOTICE";
			break;
		case ALARM_WARN:
			prio_str = "WARNING";
			break;
		case ALARM_ERR:
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
