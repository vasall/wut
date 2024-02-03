#include "utility/inc/list.h"

#include "utility/alarm/inc/alarm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FH_API struct fh_list *fh_list_create(s16 size, s16 alloc)
{
	struct fh_list *lst;

	if(size < 1 || alloc < 1) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return NULL;
	}

	if(!(lst = malloc(sizeof(struct fh_list)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for list");
		return NULL;
	}

	if(!(lst->data = malloc(size * alloc))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for list data");
		free(lst);
		return NULL;
	}

	lst->size = size;
	lst->count = 0;
	lst->alloc = alloc;

	return lst;
}


FH_API void fh_list_destroy(struct fh_list *lst)
{
	if(!lst) return;

	free(lst->data);
	free(lst);
}


FH_API s16 fh_list_push(struct fh_list *lst, void *inp)
{
	void *ptr;
	s32 nalloc;
	s32 off;

	if(!lst || !inp) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	/*
	 * Resize data buffer if necessary.
	 */
	if(lst->count + 1 >= lst->alloc) {
		nalloc = lst->alloc * 1.5;
		if(!(ptr = realloc(lst->data, nalloc * lst->size))) {
			FH_ALARM(FH_ERROR, "Failed to resize list data buffer");
			return -1;
		}
		lst->data = ptr;
		lst->alloc = nalloc;
	}

	off = lst->count * lst->size;
	memcpy(lst->data + off, inp, lst->size);	
	return ++lst->count;
}


FH_API s16 fh_list_pop(struct fh_list *lst, void *out)
{
	s32 off;

	if(!lst) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1)
		return 0;

	lst->count--;

	if(out) {
		off = lst->count * lst->size;
		memcpy(out, lst->data + off, lst->size);
	}

	return 1;
}


FH_API s16 fh_list_shift(struct fh_list *lst, void *out)
{
	if(!lst) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1)
		return 0;

	if(out) {
		memcpy(out, lst->data, lst->size);
	}

	lst->count--;
	memmove(lst->data, lst->data + lst->size, lst->size * lst->count);

	return 1;
}


FH_API s16 fh_list_test_head(struct fh_list *lst, void *out)
{
	if(!lst || !out) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1)
		return 0;

	memcpy(out, lst->data, lst->size);
	return 1;
}


FH_API s16 fh_list_test_tail(struct fh_list *lst, void *out)
{
	s32 off;

	if(!lst || !out) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1)
		return 0;

	off = (lst->count - 1) * lst->size;
	memcpy(out, lst->data + off, lst->size);

	return 1;
}


FH_API s8 fh_list_apply(struct fh_list *lst, fh_list_fnc_t fnc, void *data)
{
	u16 i;

	if(!lst || !fnc) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < lst->count; i++) {
		if(fnc(lst->data + (i * lst->size), i, data)) {
			return 0;
		}
	}

	return 0;
}


FH_API s8 fh_list_get(struct fh_list *lst, u16 idx, void *out)
{
	s32 off;

	if(!lst || !out) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1 || idx >= lst->count)
		return 0;

	off = idx * lst->size;
	memcpy(out, lst->data + off, lst->size);

	return 1;
}
