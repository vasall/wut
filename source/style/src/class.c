#include "style/inc/class.h"

#include "style/inc/stylesheet_attribute.h"

#include "utility/inc/alarm.h"
#include "utility/inc/utility.h"
#include "utility/inc/text_formatting.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WUT_INTERN s8 cls_cfnc_print(void *ptr, s16 idx, void *data)
{
        WUT_IGNORE(idx);
        WUT_IGNORE(data);

        wut_sat_print((struct wut_SheetEntry *)ptr);

        return 0;
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_XMOD struct wut_Class *wut_cls_create(char *name)
{
        struct wut_Class *cls;
        s16 size;

        if(!(cls = wut_malloc(sizeof(struct wut_Class)))) {
                WUT_ALARM(WUT_ERROR, "Failed to alloc memory for class");
                return NULL;
        }

        strcpy(cls->name, name);

        size = sizeof(struct wut_SheetEntry);
        if(!(cls->attributes = wut_CreateList(size, 16))) {
                WUT_ALARM(WUT_ERROR, "Failed to create attribute list");
                goto err_free_cls;
        }

        return cls;

err_free_cls:
        wut_free(cls);
        return NULL;
}


WUT_XMOD void wut_cls_destroy(struct wut_Class *cls)
{
        wut_DestroyList(cls->attributes);
        wut_free(cls);
}


WUT_XMOD void wut_cls_push_attr(struct wut_Class *cls,
                struct wut_SheetEntry *ent)
{
        wut_PushList(cls->attributes, ent); 
}


WUT_XMOD void wut_cls_push_table(struct wut_ClassTable *tbl,
                struct wut_Class *cls)
{
        if(tbl->number + 1 >= WUT_CLASS_LIMIT) {
                return;
        }

        tbl->list[tbl->number] = cls;
        tbl->number++;
}


WUT_XMOD struct wut_ClassTable *wut_cls_create_table(void)
{
        struct wut_ClassTable *tbl;
        s32 i;

        if(!(tbl = wut_malloc(sizeof(struct wut_ClassTable)))) {
                WUT_ALARM(WUT_ERROR, "Failed to alloc memory for class table");
                return NULL;
        }

        /* Reset everything! */
        tbl->number = 0;
        for(i = 0; i < WUT_CLASS_LIMIT; i++)
                tbl->list[i] = NULL;

        return tbl;
}


WUT_XMOD void wut_cls_destroy_table(struct wut_ClassTable *tbl)
{
        s32 i;

        if(!tbl)
                return;

        for(i = 0; i < WUT_CLASS_LIMIT; i++) {
                if(tbl->list[i]) {
                        wut_cls_destroy(tbl->list[i]);
                }
        }

        wut_free(tbl);
}


WUT_XMOD struct wut_Class *wut_cls_get(struct wut_ClassTable *tbl, char *name)
{
        s16 i;

        if(!tbl || !name) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return NULL;
        }

        for(i = 0; i < WUT_CLASS_LIMIT; i++) {
                if(!tbl->list[i])
                        continue;

                if(strcmp(tbl->list[i]->name, name) == 0) {
                        return tbl->list[i];
                }
        }

        return NULL;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */ 

WUT_API void wut_PrintClass(struct wut_Class *cls)
{
        if(!cls)
                return;

        printf("%s:\n", cls->name);

        wut_ApplyList(cls->attributes, &cls_cfnc_print, NULL);
}


WUT_API void wut_PrintClasses(struct wut_ClassTable *tbl)
{
        s32 i;

        printf("Class Table (%d):\n", tbl->number);

        for(i = 0; i < tbl->number; i++) {
                if(tbl->list[i]) {
                        wut_PrintClass(tbl->list[i]);
                }
        }
}
