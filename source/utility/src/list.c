#include "utility/inc/list.h"

#include "utility/inc/alarm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WT_API struct wt_list *wt_list_create(s16 size, s16 alloc)
{
	struct wt_list *lst;

	if(size < 1 || alloc < 1) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return NULL;
	}

	if(!(lst = malloc(sizeof(struct wt_list)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for list");
		return NULL;
	}

	if(!(lst->data = malloc(size * alloc))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for list data");
		free(lst);
		return NULL;
	}

	lst->size = size;
	lst->count = 0;
	lst->alloc = alloc;

	return lst;
}


WT_API void wt_list_destroy(struct wt_list *lst)
{
	if(!lst) return;

	free(lst->data);
	free(lst);
}


WT_API s16 wt_list_push(struct wt_list *lst, void *inp)
{
	void *ptr;
	s32 nalloc;
	s32 off;

	if(!lst || !inp) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	/*
	 * Resize data buffer if necessary.
	 */
	if(lst->count + 1 >= lst->alloc) {
		nalloc = lst->alloc * 1.5;
		if(!(ptr = realloc(lst->data, nalloc * lst->size))) {
			WT_ALARM(WT_ERROR, "Failed to resize list data buffer");
			return -1;
		}
		lst->data = ptr;
		lst->alloc = nalloc;
	}

	off = lst->count * lst->size;
	memcpy(lst->data + off, inp, lst->size);	
	return ++lst->count;
}


WT_API s16 wt_list_pop(struct wt_list *lst, void *out)
{
	s32 off;

	if(!lst) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
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


WT_API s16 wt_list_shift(struct wt_list *lst, void *out)
{
	if(!lst) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
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


WT_API s16 wt_list_test_head(struct wt_list *lst, void *out)
{
	if(!lst || !out) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1)
		return 0;

	memcpy(out, lst->data, lst->size);
	return 1;
}


WT_API s16 wt_list_test_tail(struct wt_list *lst, void *out)
{
	s32 off;

	if(!lst || !out) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1)
		return 0;

	off = (lst->count - 1) * lst->size;
	memcpy(out, lst->data + off, lst->size);

	return 1;
}


WT_API s8 wt_list_apply(struct wt_list *lst, wt_list_fnc_t fnc, void *data)
{
	s16 i;

	if(!lst || !fnc) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < lst->count; i++) {
		if(fnc(lst->data + (i * lst->size), i, data)) {
			return 0;
		}
	}

	return 0;
}


WT_API s8 wt_list_get(struct wt_list *lst, u16 idx, void **out)
{
	s32 off;

	if(!lst || !out) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1 || idx >= lst->count)
		return 0;

	off = idx * lst->size;
	*out = lst->data + off;

	return 1;
}
