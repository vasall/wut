#include "source/document/inc/loader.h"

#include "source/utility/inc/alarm.h"
#include "source/utility/inc/text_formatting.h"

#include "source/component/inc/dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_LENGTH         1024
#define MAX_CLASS_NAME_LENGTH   128

#define MAX_ATTRIB_NUMBER       8
#define MAX_ATTRIB_LENGTH       128

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				INTERNAL-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wut_ele_info {
        /* 0: undefined, 1: opening, 2: closing */
        s8                      variant;   

        /* The tag (ie. button, input, text, etc.) */
        enum wut_eTag           tag;

        /* The attributes */
        struct wut_Dictionary   attributes;
};


WUT_INTERN void ldr_reset_info(struct wut_ele_info *info)
{
        info->variant = 0;
        info->tag = WUT_UNDEF;

        wut_ClearDictionary(&info->attributes);
}

WUT_INTERN s16 ldr_find_attr(char *str, char **key, s16 *key_len,
                char **value, s16 *value_len)
{
        char *start = str;

        if(strlen(str) < 1)
                return 0;

        /* Remove leading spaces */
        while(*str && *str == 0x20) str++;
        *key = str;

        /* Find the equal-sign */
        while(*str && *str != 0x3D) str++;
        if(*str == 0) goto expression_invalid;

        *key_len = (s16)(str - *key);
        str++;

        /* Find the opening "-sign */
        while(*str && *str != 0x22) str++;
        if(*str == 0) goto expression_invalid;
        str++;
        *value = str;

        /* Find the closing "-sign */
        while(*str && *str != 0x22) str++;
        if(*str == 0) goto expression_invalid;
        *value_len = (s16)(str - *value);

        str++;
        return (s16)(str - start);

expression_invalid:
        return -1;
}


WUT_INTERN void ldr_parse_element(char *str, struct wut_ele_info *info)
{
        char *ptr;
        char *attr_start;

        s16 len;
        char *kptr;
        s16 klen;
        char *vptr;
        s16 vlen;

        char tag[512];
        char key[512];
        char value[512];

        /*
         * First determine the variant of the tag (opening/closing)
         */

        /* Closing tag */
        if(strncmp(str, "</", 2) == 0) {
                str += 2;
                info->variant = 2;
        }

        /* Opening tag */
        else if(strncmp(str, "<", 1) == 0) {
                str += 1;
                info->variant = 1;                
        }

        /*
         * Then read the tag.
         */
        
        ptr = tag;
        while(*str && (*str != 0x20)) {
                *ptr = *str;
                ptr++;
                str++;
        }
        *ptr = 0;
        
        /* Retrieve the corresponding enum from the string */
        info->tag = wut_tag_get(tag);

        /*
         * Finally extract the attributes.
         */

        /* Set the null-terminator to exclude the closing bracket */
        ptr = str;
        while(*str && (*str != 0x3E)) {
                str++;
        }
        *str = 0;

        /*
         * Finally extract the seperate attributes.
         */
        while((len = ldr_find_attr(ptr, &kptr, &klen, &vptr, &vlen)) > 0) {
                strncpy(key, kptr, klen);
                key[klen] = 0;
                wut_tfm_trim(key);

                strncpy(value, vptr, vlen);
                value[vlen] = 0;
                wut_tfm_trim(value);

                /* Write keyword-value-pair to dictionary */
                wut_SetDictionary(&info->attributes, key, value);
                        
                ptr += len;
        }
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

#if 0
WUT_API s8 wut_LoadElements(struct wut_Document *doc, char *pth,
                struct wut_Element *start)
{
        FILE *file;

        char line[MAX_LINE_LENGTH];
        char *ptr;

        char tag[MAX_LINE_LENGTH];
        char name[MAX_LINE_LENGTH];
        char classes[MAX_LINE_LENGTH];

        char swp[MAX_LINE_LENGTH];

        char *tag_start;

        enum wut_eTag tagt;

        struct wut_Element *run = start;
        struct wut_Element *ele;

        /* If not starting element is given, use the body */
        if(!run) {
                run = doc->body;
        }

        /* Open the file */
        if(!(file = fopen(pth, "r"))) {
                char swp[512];
                sprintf(swp, "Failed to open %s", pth);
                WUT_ALARM(WUT_ERROR, swp);
                return -1;
        }

        /* Reset the string buffers */
        memset(tag, 0, sizeof(tag));
        memset(name, 0, sizeof(name));
        memset(classes, 0, sizeof(classes));

        /* Reead the file line by line */
        while (fgets(line, sizeof(line), file)) {
                ptr = line;

                /* Go through the line */
                while(*ptr) {
                        /* Check if the current position indicates a closing tag */
                        if(strncmp(ptr, "</", 2) == 0) {
                                ptr += 2;
                                tag_start = ptr;

                                while (*ptr && *ptr != ' ' && *ptr != '>' && *ptr != '/') {
                                        ptr++;
                                }

                                strncpy(swp, tag_start, ptr - tag_start);

                                printf("Closing tag \"%s\"\n", swp);

                                if(strcmp(swp, tag) == 0) {
                                        if(run->parent) {
                                                run = run->parent;
                                        }
                                }
                        }
                        else if (*ptr == '<') {
                                ptr++;

                                *name = 0;
                                *classes = 0;

                                printf("Opening tag \"%s\"\n", tag);

                                ptr = ldr_opening_ele(ptr, tag, name, classes);

                                tagt = wut_ele_get(tag); 
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
#endif

WUT_API s8 wut_LoadElements(struct wut_Document *doc, char *pth,
                struct wut_Element *start)
{
        FILE *file;


        char *ptr;
        char line[MAX_LINE_LENGTH];

        char read_buf[MAX_LINE_LENGTH];
        s16 read;

        struct wut_ele_info ele_info;

        char swp[MAX_LINE_LENGTH];

        char *tag_start;

        enum wut_eTag tagt;

        struct wut_Element *run = start;
        struct wut_Element *ele;

        /* If not starting element is given, use the body */
        if(!run) {
                run = doc->body;
        }

        /* Open the file */
        if(!(file = fopen(pth, "r"))) {
                char swp[512];
                sprintf(swp, "Failed to open %s", pth);
                WUT_ALARM(WUT_ERROR, swp);
                return -1;
        }

        /* Reset the reading buffers */
        read = -1;

        printf("Start reading:\n\n");

        /* Reead the file line by line */
        while (fgets(line, sizeof(line), file)) {
                ptr = line;

                /* Go through the line */
                while(*ptr) {
                        /* 
                         * If the opening character for a tag is detected, we
                         * start reading.
                         */
                        if(strncmp(ptr, "<", 1) == 0) {
                                /* Enable reading to the read-buffer */
                                read = 0;
                        }

                        /* 
                         * Write the current character to the read-buffer.
                         * Seperating the read-buffer from the line-buffer
                         * allows reading across multiple lines.
                         */
                        if(read >= 0) {
                                read_buf[read] = *ptr;
                                read++;
                        }

                        /*
                         * If the closing character for a tag is detected, we
                         * stop reading.
                         */
                        if(strncmp(ptr, ">", 1) == 0) {
                                /*
                                 * Before we can parse the given string to
                                 * tag-info, we have to standardize it a bit.
                                 */
                                read_buf[read] = 0;
                                wut_tfm_conform(read_buf);
                                wut_tfm_reduce(read_buf);

                                /*
                                 * Now we parse the string.
                                 */
                                ldr_reset_info(&ele_info);
                                ldr_parse_element(read_buf, &ele_info); 

                                /* Disabled reading */
                                read = -1;
                        }
 
                        ptr++;
                }
        }

        printf("Finished reading\n");

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

        char attribute[MAX_ATTRIB_LENGTH] = {0};
        char value[MAX_ATTRIB_LENGTH] = {0};

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
