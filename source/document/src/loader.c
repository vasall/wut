#include "source/document/inc/loader.h"

#include "source/document/inc/tag.h"

#include "source/utility/inc/alarm.h"
#include "source/utility/inc/text_formatting.h"

#include "source/component/inc/dictionary.h"
#include "source/component/inc/string_object.h"

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

struct ldr_ele_info {
        /* 0: undefined, 1: opening, 2: closing */
        s8                      var;   

        /*
         * The element info.
         */
        struct wut_ElementInfo  einfo;
};


struct wut_ldr_reader {
        FILE *file;
        char line[MAX_LINE_LENGTH];
        char *ptr;
};


WUT_INTERN void ldr_reset_info(struct ldr_ele_info *info)
{
        info->var = 0;

        info->einfo.tag = WUT_UNDEF;
        wut_ClearDictionary(&info->einfo.attrib);
}


WUT_INTERN void ldr_print(struct ldr_ele_info *info)
{
        char swp[128];
        wut_tag_name(info->einfo.tag, swp);

        printf("Variant: %d, Tag: %s\n", info->var, swp);
        printf("Attributes:\n");
        wut_PrintDictionary(&info->einfo.attrib);
        printf("\n");
}


WUT_INTERN struct wut_ldr_reader ldr_new_reader(FILE *file)
{
        struct wut_ldr_reader rdr;

        rdr.file = file;
        *rdr.line = 0;
        rdr.ptr = rdr.line;

        return rdr;
}


WUT_INTERN s8 ldr_read_next(struct wut_ldr_reader *rdr, char *c)
{
        if(*(rdr->ptr) == 0) {
                if(!fgets(rdr->line, MAX_LINE_LENGTH, rdr->file)) {
                        return 0;
                }

                rdr->ptr = rdr->line;
        }

        *c = *(rdr->ptr);
        rdr->ptr++;
        return 1;
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


WUT_INTERN void ldr_parse_element(char *str, struct ldr_ele_info *info)
{
        char *ptr;

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
                info->var = 2;
        }

        /* Opening tag */
        else if(strncmp(str, "<", 1) == 0) {
                str += 1;
                info->var = 1;                
        }

        /*
         * Then read the tag.
         */ 
        ptr = tag;
        while(*str && (*str != 0x20) && (*str != 0x3E)) {
                *ptr = *str;
                ptr++;
                str++;
        }
        *ptr = 0;

        /* Retrieve the corresponding enum from the string */
        info->einfo.tag = wut_tag_get(tag);

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
                wut_SetDictionary(&info->einfo.attrib, key, value);

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

WUT_API s8 wut_LoadElements(struct wut_Document *doc, char *pth,
                struct wut_Element *atmpnt)
{
        FILE *file;
        struct wut_ldr_reader reader;
        char c;

        char read_buf[MAX_LINE_LENGTH];
        s16 read;
        s8 mode;

        struct wut_String *text;

        struct ldr_ele_info info;

        struct wut_Element *run = NULL;
        struct wut_Element *ele;

        if(atmpnt == NULL) {
                run = doc->body;
        }
        else {
                run = atmpnt;
        }

        /* Open the file */
        if(!(file = fopen(pth, "r"))) {
                char swp[512];
                sprintf(swp, "Failed to open %s", pth);
                WUT_ALARM(WUT_ERROR, swp);
                return -1;
        }

        /* Create a new reader */
        reader = ldr_new_reader(file);

        /* Reset the reading buffers */
        read = -1;
        text = wut_CreateString(512);
        mode = 0;

        /* Read the file character by character */
        while(ldr_read_next(&reader, &c)) {
                /* 
                 * If the opening character for a tag is detected, we
                 * start reading.
                 */
                if(c == '<') {
                        /* Enable reading the element-tag */
                        read = 0;
                        mode = 0;
                }

                /* 
                 * Write the current character to the read-buffer.
                 * Seperating the read-buffer from the line-buffer
                 * allows reading across multiple lines.
                 */
                if(read >= 0 && mode == 0) {
                        read_buf[read] = c;
                        read++;
                }
                /*
                 * Write the current character to the text-string.
                 * This will be used when creating a text-element.
                 */
                else if(read >= 0 && mode == 1) {
                        wut_ExtendString(text, WUT_STRING_END, &c, 1);
                }


                /*
                 * If the closing character for an element-tag is detected, we
                 * stop reading.
                 */
                if(c == '>') {
                        /*
                         * Before we can parse the given string to
                         * tag-info, we have to standardize it a bit.
                         */
                        read_buf[read] = 0;
                        wut_tfm_conform(read_buf);
                        wut_tfm_reduce(read_buf);

                        /*
                         * Now we parse the string into the info-struct.
                         */
                        ldr_reset_info(&info);
                        ldr_parse_element(read_buf, &info); 

                        ldr_print(&info);

                        /* 
                         * Generally disable reading. This can be overwritten
                         * later when reading the text content for example.
                         */
                        read = -1;

                        /* <OpeningTag> */
                        if(info.var == 1) {
                                /*
                                 * Create a new element.
                                 */
                                if(!(ele = wut_CreateElement(doc, &info.einfo))) {
                                        printf("Failed to read!!\n");
                                        fclose(file);
                                        break;
                                }

                                wut_AttachElement(run, ele);
                                run = ele;

                                /* 
                                 * Start reading the content for the
                                 * text-element. This will even read
                                 * over other element defines, until the
                                 * text-ending is found.
                                 */
                                if(info.einfo.tag == WUT_TEXT) {
                                        wut_ClearString(text);
                                        mode = 1;
                                        read = 0;
                                }
                        }
                        /* </ClosingTag> */
                        else if(info.var == 2) {
                                /*
                                 * If we're currently reading the content for a
                                 * text-element, we have to keep going until we
                                 * reach the end of the text-element. Even if
                                 * there are other elements in the way. They
                                 * will be just be read as text.
                                 */
                                if(mode == 1) {
                                        /* Stop reading the text */
                                        if(info.einfo.tag == WUT_TEXT) {
                                                mode = 0;

                                                /*
                                                 * Here we have to add the read
                                                 * text to the text-element.
                                                 */
                                                

                                                /* Go back to the parent */
                                                run = run->parent;
                                        }
                                }
                                else { 
                                        /* Go back to the parent */
                                        run = run->parent;                       
                                }
                        }
                }
        }

        /* ..and update the entire document */
        wut_doc_has_changed(doc, NULL, 0, 0);

        fclose(file);
        return 0;
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
