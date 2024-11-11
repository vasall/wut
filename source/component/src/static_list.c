#include "source/component/inc/static_list.h"

#include "source/utility/inc/alarm.h"

#include "source/system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WUT_API struct wut_StatList *wut_CreateStatList(s16 size, s16 alloc)
{
        struct wut_StatList *lst;

        if(size < 1 || alloc < 1) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return NULL;
        }

        if(!(lst = malloc(sizeof(struct wut_StatList)))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate memory for list");
                return NULL;
        }

        if(!(lst->mask = wut_calloc(alloc))) {
                goto err_free_lst;
        }

        if(!(lst->data = wut_malloc(size * alloc))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate memory for list data");
                goto err_free_mask;
        }

        lst->size = size;
        lst->count = 0;
        lst->alloc = alloc;

        return lst;

err_free_lst:
        wut_free(lst);

err_free_mask:
        wut_free(lst->mask);

        return NULL;
}


WUT_API void wut_DestroyStatList(struct wut_StatList *lst)
{
        if(!lst) return;

        wut_free(lst->data);
        wut_free(lst->mask);
        wut_free(lst);
}


WUT_API s16 wut_AddStatList(struct wut_StatList *lst, void *inp)
{
        s32 off;
        s16 i;

        if(!lst || !inp) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
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


WUT_API void wut_RemoveStatList(struct wut_StatList *lst, s16 idx)
{
        if(!lst || idx < 0 || idx >= lst->alloc)
                return;

        lst->mask[idx] = 0;
        lst->count--;
}


WUT_API s8 wut_GetStatList(struct wut_StatList *lst, s16 idx, void *out)
{
        s32 off;

        if(!lst || !out) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
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


WUT_API s8 wut_ApplyStatList(struct wut_StatList *lst, 
                wut_StatListFunc fnc, void *p)
{
        u16 i;

        if(!lst || !fnc) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
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
