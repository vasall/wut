#include "utility/inc/static_list.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FH_API struct fh_statlist *fh_statlist_create(s16 size, s16 alloc)
{
	struct fh_statlist *lst;

	if(size < 1 || alloc < 1) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return NULL;
	}

	if(!(lst = malloc(sizeof(struct fh_statlist)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for list");
		return NULL;
	}

	if(!(lst->mask = fh_calloc(alloc))) {
		goto err_free_lst;
	}

	if(!(lst->data = fh_malloc(size * alloc))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for list data");
		goto err_free_mask;
	}

	lst->size = size;
	lst->count = 0;
	lst->alloc = alloc;

	return lst;

err_free_lst:
	fh_free(lst);

err_free_mask:
	fh_free(lst->mask);

	return NULL;
}


FH_API void fh_statlist_destroy(struct fh_statlist *lst)
{
	if(!lst) return;

	fh_free(lst->data);
	fh_free(lst->mask);
	fh_free(lst);
}


FH_API s16 fh_statlist_add(struct fh_statlist *lst, void *inp)
{
	s32 off;
	s16 i;

	if(!lst || !inp) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
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


FH_API void fh_statlist_rmv(struct fh_statlist *lst, s16 idx)
{
	if(!lst || idx < 0 || idx >= lst->alloc)
		return;

	lst->mask[idx] = 0;
	lst->count--;
}


FH_API s8 fh_statlist_get(struct fh_statlist *lst, s16 idx, void *out)
{
	s32 off;

	if(!lst || !out) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
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


FH_API s8 fh_statlist_apply(struct fh_statlist *lst, fh_statlist_fnc_t fnc, void *p)
{
	u16 i;

	if(!lst || !fnc) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
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
