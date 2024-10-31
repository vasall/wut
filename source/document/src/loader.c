#include "document/inc/loader.h"

#include "utility/inc/alarm.h"
#include "utility/inc/text_formatting.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_LENGTH 1024
#define MAX_CLASS_NAME_LENGTH 100
#define MAX_ATTRIBUTE_LENGTH 100

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				INTERNAL-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_INTERN char *ldr_opening_tag(char *ptr, char *tag, char *name,
                char *classes)
{
        char *tag_start = ptr;
        char *attr_start;
        char *name_start;
        char *class_start;

        while (*ptr && *ptr != ' ' && *ptr != '>' && *ptr != '/') {
                ptr++;
        }

        if(tag) {
                strncpy(tag, tag_start, ptr - tag_start);
        }

        if (*ptr == ' ') {
                attr_start = ++ptr;
                while (*ptr && *ptr != '>') {
                        if(strncmp(ptr, "name=\"", 6) == 0) {
                                ptr += 6;
                                name_start = ptr;
                                while (*ptr && *ptr != '\"') {
                                        ptr++;
                                }
                                if(name) {
                                        strncpy(name, name_start, ptr - name_start);
                                        name[ptr - name_start] = '\0';
                                }
                        }
                        else if (strncmp(ptr, "class=\"", 7) == 0) {
                                ptr += 7;
                                class_start = ptr;
                                while (*ptr && *ptr != '\"') {
                                        ptr++;
                                }
                                if(classes) {
                                        strncpy(classes, class_start, ptr - class_start);
                                        classes[ptr - class_start] = '\0';
                                }
                        } 
                        ptr++;
                }
        }

        return ptr;
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API s8 wut_LoadElements(struct wut_Document *doc, char *pth,
                struct wut_Element *start)
{
        FILE *file;

        char line[MAX_LINE_LENGTH];
        char *ptr;

        char tag[MAX_LINE_LENGTH];
        char name[MAX_LINE_LENGTH];
        char classes[MAX_LINE_LENGTH];

        char *tag_start;

        enum wut_eTag tagt;

        struct wut_Element *run = start;
        struct wut_Element *ele;


        if(!run) {
                run = doc->body;
        }

        if(!(file = fopen(pth, "r"))) {
                char swp[512];
                sprintf(swp, "Failed to open %s", pth);
                WUT_ALARM(WUT_ERROR, swp);
                return -1;
        }

        memset(tag, 0, sizeof(tag));
        memset(name, 0, sizeof(name));
        memset(classes, 0, sizeof(classes));

        while (fgets(line, sizeof(line), file)) {
                ptr = line;

                while (*ptr) {
                        if(strncmp(ptr, "</", 2) == 0) {
                                ptr += 2;
                                tag_start = ptr;

                                while (*ptr && *ptr != ' ' && *ptr != '>' && *ptr != '/') {
                                        ptr++;
                                }

                                if(strncmp(tag, tag_start, ptr - tag_start) == 0) {
                                        if(run->parent) {
                                                run = run->parent;
                                        }
                                }
                        }
                        else if (*ptr == '<') {
                                ptr++;

                                *name = 0;
                                *classes = 0;

                                ptr = ldr_opening_tag(ptr, tag, name, classes);

                                tagt = wut_tag_get(tag); 
                                if(!(ele = wut_CreateElement(doc, name, tagt, NULL))) {
                                        printf("Failed to create %s\n", name);
                                        goto err_close_file;
                                }

                                /* Attach the element to the parent */
                                wut_AttachElement(run, ele); 
                                run = ele;

                                /* Attach classes to the element */
                                wut_AddClasses(ele, classes);


                        }
                        ptr++;
                }
        }

        wut_doc_has_changed(doc, start, 0, 0);

        fclose(file);
        return 0;

err_close_file:
        fclose(file);
        return -1;
}


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

        s8 isnew = 1;

        if(!doc || !pth) {
                WUT_ALARM(WUT_WARNING, "Invalid parameters");
                return -1;
        }

        tbl = doc->class_table;

        if(!(file = fopen(pth, "r"))) {
                char tmp[512];
                sprintf(tmp, "Failed to open file \"%s\"", pth);
                WUT_ALARM(WUT_ERROR, tmp);
                return -1;
        }


        while(fgets(line, sizeof(line), file)) {
                char *trimmed_line = wut_tfm_trim(line);

                /* Check if the line starts with a class selector */
                if(trimmed_line[0] == '.' && !cls) {
                        /* Extract the class name */
                        sscanf(trimmed_line, ".%[^ {]", class_name);

                        /* Check if the class already exists */
                        if((cls = wut_cls_get(doc->class_table, class_name))) {
                                isnew = 0;
                        }
                        /* Otherwise, create a new class */
                        else {
                                cls = wut_cls_create(class_name);
                        }
                }

                /* Check if the line contains attributes (inside a block) */
                if(cls) {
                        if(strstr(trimmed_line, "{") != NULL) {
                                /* Skip the opening brace line */
                                continue;
                        }
                        else if(strstr(trimmed_line, "}") != NULL) {
                                /* Push class if it is new */
                                if(isnew) {
                                        wut_cls_push_table(tbl, cls);
                                }

                                isnew = 1;
                                cls = NULL;
                        }
                        else {
                                wut_sat_reset(&ent);

                                /* TODO: Newlines are read as attribute name */
                                sscanf(trimmed_line, "%[^:]:%[^;];", attribute, value);

                                /* Parse attribute and push to class */
                                wut_sat_parse(
                                                wut_tfm_trim(attribute),
                                                wut_tfm_trim(value),
                                                &ent
                                             );

                                wut_cls_set_attr(cls, &ent);
                        }
                }
        }


        wut_doc_has_changed(doc, NULL, 0, 0);

        fclose(file);
        return 0;
}
