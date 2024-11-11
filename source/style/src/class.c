#include "source/style/inc/class.h"

#include "source/style/inc/stylesheet_attribute.h"

#include "source/utility/inc/alarm.h"
#include "source/utility/inc/utility.h"
#include "source/utility/inc/text_formatting.h"

#include "source/system/inc/system.h"

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


WUT_INTERN s8 cls_cfnc_apply(void *ptr, s16 idx, void *data)
{
        struct wut_SheetEntry *ent = (struct wut_SheetEntry *)ptr;
        struct wut_Stylesheet *sht = (struct wut_Stylesheet *)data;

        WUT_IGNORE(idx);

        wut_sht_set(sht, ent);

        return 0;
}

struct cls_search_pass {
        s8 found;
        enum wut_eSheetAttribId id;
        struct wut_SheetEntry *entry;
};

WUT_INTERN s8 cls_cfnc_search(void *ptr, s16 idx, void *data)
{
        struct wut_SheetEntry *ent = (struct wut_SheetEntry *)ptr;
        struct cls_search_pass *pass = (struct cls_search_pass *)data;

        WUT_IGNORE(idx);

        if(ent->id == pass->id) {
                pass->found = 1;
                memcpy(pass->entry, ent, sizeof(struct wut_SheetEntry));
                return 1;
        }

        return 0;

}


WUT_INTERN s8 cls_cfnc_write(void *ptr, s16 idx, void *data)
{
        struct wut_SheetEntry *ent = (struct wut_SheetEntry *)ptr;
        struct cls_search_pass *pass = (struct cls_search_pass *)data;

        WUT_IGNORE(idx);

        if(ent->id == pass->id) {
                pass->found = 1;
                memcpy(ent, pass->entry, sizeof(struct wut_SheetEntry));
                return 1;
        }

        return 0;
}


WUT_INTERN s8 cls_search(struct wut_Class *cls, enum wut_eSheetAttribId id,
                struct wut_SheetEntry *ret)
{
        struct cls_search_pass pass;

        if(!(cls->mask & (1<<id)))
                return 0;

        pass.found = 0;
        pass.id = id;
        pass.entry = ret;

        wut_ApplyList(cls->attributes, &cls_cfnc_search, &pass);

        return pass.found;
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
        cls->mask = 0;

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


WUT_XMOD void wut_cls_set_attr(struct wut_Class *cls,
                struct wut_SheetEntry *ent)
{
        struct cls_search_pass pass;

        pass.found = 0;
        pass.id = ent->id;
        pass.entry = ent;

        wut_ApplyList(cls->attributes, &cls_cfnc_write, &pass);

        if(pass.found == 0) {
                /* Push attribute into list */
                wut_PushList(cls->attributes, ent); 

                /* Add attribute flag to mask */
                cls->mask |= (1<<ent->id);
        }
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


WUT_XMOD void wut_cls_apply(struct wut_Class *cls, struct wut_Stylesheet *sht)
{
        wut_ApplyList(cls->attributes, &cls_cfnc_apply, sht);        
}


WUT_XMOD void wut_cls_reset_references(struct wut_ClassSheet *ref)
{
        s8 i;

        if(!ref) {
                WUT_ALARM(WUT_WARNING, "Input parameter invalid");
                return;
        }

        ref->mask = 0;
        ref->number = 0;

        for(i = 0; i < 8; i++) {
                *ref->names[i] = 0;
                ref->links[i] = NULL;
        }
}


WUT_XMOD void wut_cls_add_names(struct wut_ClassSheet *ref, char *names)
{
        char *class;

        class = strtok(names, " ");
        while(class && ref->number < 8) {
                strcpy(ref->names[ref->number], class);
                ref->number++;

                class = strtok(NULL, " ");
        }
}


WUT_XMOD void wut_cls_link(struct wut_ClassTable *tbl,
                struct wut_ClassSheet *ref)
{
        s8 i;
        struct wut_Class *cls;

        ref->mask = 0;

        for(i = 0; i < 8; i++) {
                if(strlen(ref->names[i]) > 0 && ref->links[i] == NULL) {
                        if((cls = wut_cls_get(tbl, ref->names[i]))) {
                                ref->links[i] = cls; 
                                ref->mask |= cls->mask;
                        }
                }
        }
}


WUT_XMOD void wut_cls_apply_references(struct wut_ClassSheet *ref,
                struct wut_Stylesheet *sht)
{
        s8 i;

        for(i = 0; i < 8; i++) {
                if(ref->links[i]) {
                        wut_cls_apply(ref->links[i], sht);
                }
        }
}


WUT_XMOD s8 wut_cls_find(struct wut_ClassSheet *ref,
                enum wut_eSheetAttribId id, struct wut_SheetEntry *ret)
{
        s8 i;

        for(i = 0; i < 8; i++) {
                if(!ref->links[i])
                        continue;

                if(cls_search(ref->links[i], id, ret)) {
                        return 1;
                }
        }

        return 0;
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
