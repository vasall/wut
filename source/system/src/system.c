#include "system/inc/system.h"

#include "utility/inc/alarm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>


WT_API void *wt_malloc(s32 len)
{
	void *p;

	if(len < 1) {
		WT_ALARM(WT_WARNING, "len invalid");
		return NULL;
	}

	if(!(p = malloc(len))) {
		WT_ALARM(WT_ERROR, "failed to allocate memory");
		return NULL;
	}

	return p;
}


WT_API void *wt_calloc(s32 len)
{
	void *p;

	if(len < 1) {
		WT_ALARM(WT_WARNING, "len invalid");
		return NULL;
	}

	if(!(p = calloc(1, len))) {
		WT_ALARM(WT_ERROR, "failed to callocate memory");
		return NULL;
	}

	return p;
}


WT_API void *wt_zalloc(s32 len)
{
	void *p;

	if(len < 1) {
		WT_ALARM(WT_WARNING, "len invalid");
		return NULL;
	}

	if(!(p = malloc(len))) {
		WT_ALARM(WT_ERROR, "failed to allocate memory");
		return NULL;
	}

	memset(p, 0, len);
	return p;
}


WT_API void *wt_realloc(void *old, s32 newlen)
{
	void *p;

	if(newlen < 1) {
		WT_ALARM(WT_WARNING, "old undefined or newlen invalid");
		return NULL;
	}

	if(!old) {
		if(!(p = malloc(newlen))) {
			printf("failed to allocate memory");
			return NULL;
		}
	}
	else if(!(p = realloc(old, newlen))) {
		WT_ALARM(WT_ERROR, "failed to reallocate memory");
		return NULL;
	}

	return p;
}


WT_API void wt_free(void *p)
{
	if(!p) {
		return;
	}

	free(p);
}


WT_API void wt_zeros(void *p, s32 len)
{
	memset(p, 0, len);
}


WT_API s8 wt_strtoint(s32 *out, char *s, s32 base)
{
	char *end;
	s64 l;

	if(s[0] == '\0' || isspace(s[0]))
		goto err_return;

	errno = 0;
	l = strtol(s, &end, base);

	/* Both checks are needed because INT_MAX == LONG_MAX is possible. */
	if(l > INT_MAX || (errno == ERANGE && l == LONG_MAX)) {
		WT_ALARM(WT_ERROR, "integer overflow");
		goto err_return;
	}
	if(l < INT_MIN || (errno == ERANGE && l == LONG_MIN)) {
		WT_ALARM(WT_ERROR, "integer undeflow");
		goto err_return;
	}
	if(*end != '\0') {
		WT_ALARM(WT_ERROR, "inconvertible");
		goto err_return;
	}

	*out = l;
	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to convert string to integer");
	return -1;
}


WT_API void wt_bindump(void *p, s32 len)
{
	char buf[17];
	char *pc;
	s32 i;

	if(!p || len < 1) {
		WT_ALARM(WT_WARNING, "p undefined or len invalid");
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


WT_API void wt_rand_string(char *out, u16 size)
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
