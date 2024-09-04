#include "utility/inc/list.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WUT_API struct wut_List *wut_CreateList(s16 size, s16 alloc)
{
        struct wut_List *lst;

        if(size < 1 || alloc < 1) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return NULL;
        }

        if(!(lst = malloc(sizeof(struct wut_List)))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate memory for list");
                return NULL;
        }

        if(!(lst->data = malloc(size * alloc))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate memory for list data");
                free(lst);
                return NULL;
        }

        lst->size = size;
        lst->count = 0;
        lst->alloc = alloc;

        return lst;
}


WUT_API void wut_DestroyList(struct wut_List *lst)
{
        if(!lst) return;

        free(lst->data);
        free(lst);
}


WUT_API s16 wut_PushList(struct wut_List *lst, void *inp)
{
        void *ptr;
        s32 nalloc;
        s32 off;

        if(!lst || !inp) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return -1;
        }

        /*
         * Resize data buffer if necessary.
         */
        if(lst->count + 1 >= lst->alloc) {
                nalloc = lst->alloc * 1.5;
                if(!(ptr = realloc(lst->data, nalloc * lst->size))) {
                        WUT_ALARM(WUT_ERROR, "Failed to resize list data buffer");
                        return -1;
                }
                lst->data = ptr;
                lst->alloc = nalloc;
        }

        off = lst->count * lst->size;
        memcpy(lst->data + off, inp, lst->size);	
        return ++lst->count;
}


WUT_API s16 wut_PopList(struct wut_List *lst, void *out)
{
        s32 off;

        if(!lst) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
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


WUT_API s16 wut_ShiftList(struct wut_List *lst, void *out)
{
        if(!lst) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
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


WUT_API s16 wut_TestListHead(struct wut_List *lst, void *out)
{
        if(!lst || !out) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return -1;
        }

        if(lst->count < 1)
                return 0;

        memcpy(out, lst->data, lst->size);
        return 1;
}


WUT_API s16 wut_TestListTail(struct wut_List *lst, void *out)
{
        s32 off;

        if(!lst || !out) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return -1;
        }

        if(lst->count < 1)
                return 0;

        off = (lst->count - 1) * lst->size;
        memcpy(out, lst->data + off, lst->size);

        return 1;
}


WUT_API s8 wut_GetList(struct wut_List *lst, u16 idx, void **out)
{
        s32 off;

        if(!lst || !out) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return -1;
        }

        if(lst->count < 1 || idx >= lst->count)
                return 0;

        off = idx * lst->size;
        *out = lst->data + off;

        return 1;
}


WUT_API struct wut_List *wut_DuplicateList(struct wut_List *src)
{
        struct wut_List *lst;
        s32 size;

        if(!(lst = wut_malloc(sizeof(struct wut_List)))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate memory for list");
                return NULL;
        }

        size = src->size * src->alloc;
        if(!(lst->data = wut_malloc(size))) {
                WUT_ALARM(WUT_ERROR, "Failed to preallocate memory for list");
                goto err_free_lst;
        }

        /* Copy over everything */
        lst->size = src->size;
        lst->count = src->count;
        lst->alloc = src->alloc;
        memcpy(lst->data, src->data, size);

        return lst;

err_free_lst:
        wut_free(lst);
        return NULL;
}


WUT_API s8 wut_ApplyList(struct wut_List *lst, wut_ListFunc fnc, void *data)
{
        s16 i;

        if(!lst || !fnc) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return -1;
        }

        for(i = 0; i < lst->count; i++) {
                if(fnc(lst->data + (i * lst->size), i, data)) {
                        return 0;
                }
        }

        return 0;
}
