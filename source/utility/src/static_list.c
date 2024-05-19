#include "utility/inc/static_list.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WT_API struct wt_statlist *wt_statlist_create(s16 size, s16 alloc)
{
	struct wt_statlist *lst;

	if(size < 1 || alloc < 1) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return NULL;
	}

	if(!(lst = malloc(sizeof(struct wt_statlist)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for list");
		return NULL;
	}

	if(!(lst->mask = wt_calloc(alloc))) {
		goto err_free_lst;
	}

	if(!(lst->data = wt_malloc(size * alloc))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for list data");
		goto err_free_mask;
	}

	lst->size = size;
	lst->count = 0;
	lst->alloc = alloc;

	return lst;

err_free_lst:
	wt_free(lst);

err_free_mask:
	wt_free(lst->mask);

	return NULL;
}


WT_API void wt_statlist_destroy(struct wt_statlist *lst)
{
	if(!lst) return;

	wt_free(lst->data);
	wt_free(lst->mask);
	wt_free(lst);
}


WT_API s16 wt_statlist_add(struct wt_statlist *lst, void *inp)
{
	s32 off;
	s16 i;

	if(!lst || !inp) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count + 1 >= lst->alloc)
		return -1;

	for(i = 0; i < lst->alloc; i++) {
		if(!lst->mask[i]) {
			off = i * lst->size;
			memcpy(lst->data + off, inp, lst->size);

			lst->mask[i] = 1;
			lst->count++;
				
			return i;
		}
	}

	return -1;
}


WT_API void wt_statlist_rmv(struct wt_statlist *lst, s16 idx)
{
	if(!lst || idx < 0 || idx >= lst->alloc)
		return;

	lst->mask[idx] = 0;
	lst->count--;
}


WT_API s8 wt_statlist_get(struct wt_statlist *lst, s16 idx, void *out)
{
	s32 off;

	if(!lst || !out) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(lst->count < 1 || idx >= lst->alloc)
		return -1;

	if(!lst->mask[idx])
		return 0;

	off = idx * lst->size;
	memcpy(out, lst->data + off, lst->size);

	return 1;
}


WT_API s8 wt_statlist_apply(struct wt_statlist *lst, wt_statlist_fnc_t fnc, void *p)
{
	u16 i;

	if(!lst || !fnc) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < lst->alloc; i++) {
		if(!lst->mask[i])
			continue;

		if(fnc(lst->data + (i * lst->size), i, p)) {
			return 0;
		}
	}

	return 0;
}
