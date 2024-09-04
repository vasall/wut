#include "style/inc/class.h"

#include "style/inc/stylesheet_attribute.h"

#include "utility/inc/alarm.h"
#include "utility/inc/text_formatting.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_CLASS_NAME_LENGTH 100
#define MAX_ATTRIBUTE_LENGTH 100


WUT_INTERN struct wut_Class *cls_create(char *name)
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


WUT_INTERN void cls_destroy(struct wut_Class *cls)
{
        wut_DestroyList(cls->attributes);
        wut_free(cls);
}


WUT_INTERN void cls_push_attr(struct wut_Class *cls, struct wut_SheetEntry *ent)
{
       wut_PushList(cls->attributes, ent); 
}


WUT_INTERN void cls_push_table(struct wut_ClassTable *tbl,
                struct wut_Class *cls)
{
        if(tbl->number + 1 >= WUT_CLASS_LIMIT) {
                return;
        }

        tbl->list[tbl->number] = cls;
        tbl->number++;
}


WUT_INTERN s8 cls_cfnc_print(void *ptr, s16 idx, void *data)
{
        WUT_IGNORE(idx);
        WUT_IGNORE(data);

        wut_sat_print((struct wut_SheetEntry *)ptr);
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

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
                /* TODO: Remove classes */
        }

        wut_free(tbl);
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API s8 wut_LoadClasses(struct wut_Document *doc, char *pth)
{
        FILE *file;
        
        char line[MAX_LINE_LENGTH];
        char class_name[MAX_CLASS_NAME_LENGTH] = {0};

        char attribute[MAX_ATTRIBUTE_LENGTH] = {0};
        char value[MAX_ATTRIBUTE_LENGTH] = {0};

        struct wut_ClassTable *tbl;
        struct wut_Class *cls = NULL;
        struct wut_SheetEntry ent;

        if(!doc || !pth) {
                WUT_ALARM(WUT_WARNING, "Invalid parameters");
                return -1;
        }

        tbl = doc->classes;

        if(!(file = fopen(pth, "r"))) {
                char tmp[512];
                sprintf(tmp, "Failed to open file \"%s\"", pth);
                WUT_ALARM(WUT_ERROR, tmp);
                return -1;
        }

        printf("\n\n");

        
        while(fgets(line, sizeof(line), file)) {
                char *trimmed_line = wut_tfm_trim(line);

                printf("> %s\n", trimmed_line);

                /* Check if the line starts with a class selector */
                if(trimmed_line[0] == '.' && !cls) {
                        /* Extract the class name */
                        sscanf(trimmed_line, ".%[^ {]", class_name);
                        printf("Class: %s\n", class_name);

                        /* Create a new class */
                        cls = cls_create(class_name);
                }

                /* Check if the line contains attributes (inside a block) */
                if(cls) {
                        if(strstr(trimmed_line, "{") != NULL) {
                                /* Skip the opening brace line */
                                continue;
                        }
                        else if(strstr(trimmed_line, "}") != NULL) {
                                /* Push class to table and reset */
                                cls_push_table(tbl, cls);
                                cls = NULL;

                                printf("Finished class\n");
                        }
                        else {
                                wut_sat_reset(&ent);

                                sscanf(trimmed_line, "%[^:]:%[^;];", attribute, value);

                                printf("  %s: %s\n", wut_tfm_trim(attribute),
                                                wut_tfm_trim(value));

                                /* Parse attribute and push to class */
                                wut_sat_parse(
                                                wut_tfm_trim(attribute),
                                                wut_tfm_trim(value),
                                                &ent
                                                );

                                cls_push_attr(cls, &ent);
                        }
                }
        }

        printf("\n\n");

        fclose(file);

        return 0;
} 


WUT_API void wut_PrintClass(struct wut_Class *cls)
{
        if(!cls)
                return;

        printf("Class <%s>:\n", cls->name);

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
