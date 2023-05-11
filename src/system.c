#include "system.h"

#include "alarm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>


FH_API void *fh_malloc(s32 len)
{
	void *p;

	if(len < 1) {
		ALARM(ALARM_WARN, "len invalid");
		return NULL;
	}

	if(!(p = malloc(len))) {
		ALARM(ALARM_ERR, "failed to allocate memory");
		return NULL;
	}

	return p;
}


FH_API void *fh_calloc(s32 len)
{
	void *p;

	if(len < 1) {
		ALARM(ALARM_WARN, "len invalid");
		return NULL;
	}

	if(!(p = calloc(1, len))) {
		ALARM(ALARM_ERR, "failed to callocate memory");
		return NULL;
	}

	return p;
}


FH_API void *fh_zalloc(s32 len)
{
	void *p;

	if(len < 1) {
		ALARM(ALARM_WARN, "len invalid");
		return NULL;
	}

	if(!(p = malloc(len))) {
		ALARM(ALARM_ERR, "failed to allocate memory");
		return NULL;
	}

	memset(p, 0, len);
	return p;
}


FH_API void *fh_realloc(void *old, s32 newlen)
{
	void *p;

	if(!old || newlen < 1) {
		ALARM(ALARM_WARN, "old undefined or newlen invalid");
		return NULL;
	}

	if(!(p = realloc(old, newlen))) {
		ALARM(ALARM_ERR, "failed to reallocate memory");
		return NULL;
	}

	return p;
}


FH_API void fh_free(void *p)
{
	if(!p) {
		return;
	}

	free(p);
}


FH_API void fh_zeros(void *p, s32 len)
{
	memset(p, 0, len);
}


FH_API s8 fh_strtoint(s32 *out, char *s, s32 base)
{
	char *end;
	s64 l;

	if(s[0] == '\0' || isspace(s[0]))
		goto err_return;

	errno = 0;
	l = strtol(s, &end, base);

	/* Both checks are needed because INT_MAX == LONG_MAX is possible. */
	if(l > INT_MAX || (errno == ERANGE && l == LONG_MAX)) {
		ALARM(ALARM_ERR, "integer overflow");
		goto err_return;
	}
	if(l < INT_MIN || (errno == ERANGE && l == LONG_MIN)) {
		ALARM(ALARM_ERR, "integer undeflow");
		goto err_return;
	}
	if(*end != '\0') {
		ALARM(ALARM_ERR, "inconvertible");
		goto err_return;
	}

	*out = l;
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to convert string to integer");
	return -1;
}


FH_API void fh_bindump(void *p, s32 len)
{
	char buf[17];
	char *pc;
	s32 i;

	if(!p || len < 1) {
		ALARM(ALARM_WARN, "p undefined or len invalid");
		return;
	}

	pc = p;

	for(i = 0; i < len; i++) {
		if ((i % 16) == 0) {
			if (i != 0)
				printf("  %s\n", buf);

			printf("  %04x ", i);
		}

		printf(" %02x", pc[i]);

		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buf[i % 16] = '.';
		else
			buf[i % 16] = pc[i];

		buf[(i % 16) + 1] = '\0';
	}

	while((i % 16) != 0) {
		printf("   ");
		i++;
	}

	printf("  %s\n", buf);
}


FH_API void fh_rand_string(char *out, u16 size)
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
	u16 i;
	u8 key;

	if(size) {
		--size;
		for (i = 0; i < size; i++) {
			key = (u8)(rand() %  (sizeof(charset) - 1));
			out[i] = charset[key];
		}
		out[size] = '\0';
	}
}
