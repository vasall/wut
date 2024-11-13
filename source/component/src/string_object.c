#include "source/component/inc/string_object.h"

#include "source/utility/inc/alarm.h"

#include "source/system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WUT_INTERN s8 str_ensure_fit(struct wut_String *str, s16 add)
{
        char *ptr;
        s16 nalloc;

        if(str->size + add < str->alloc) 
                return 0;

        nalloc = str->alloc * WUT_STRING_GROWTH;
        if(!(ptr = wut_realloc(str->buf, nalloc))) {
                WUT_ALARM(WUT_ERROR, "Failed to scale string buffer");
                return -1;
        }
 
        str->buf = ptr;
        str->alloc = nalloc;

        return 0;
}

WUT_INTERN s8 str_set_nullterm(struct wut_String *str)
{
        *(str->buf + str->size) = 0;
        return 0;
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API struct wut_String *wut_CreateString(s16 alloc)
{
        struct wut_String *str;

        if(!(str = wut_malloc(sizeof(struct wut_String)))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate memory for string");
                return NULL;
        }

        str->length = 0;
        str->size = 0;
        str->alloc = alloc;
        
        if(!(str->buf = wut_zalloc(alloc))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate string buffer");
                goto err_free_str;
        }

        return str;

err_free_str:
        wut_free(str);

        return NULL;
}


WUT_API void wut_DestroyString(struct wut_String *str)
{
        if(!str)
                return;

        wut_free(str->buf);
        wut_free(str);
}


WUT_API void wut_ClearString(struct wut_String *str)
{
        if(!str)
                return;

        wut_zeros(str->buf, str->alloc);

        str->length = 0;
        str->size = 0;
}


WUT_API char *wut_GetString(struct wut_String *str)
{
        if(!str) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return NULL;
        }

        return str->buf;
}


WUT_API struct wut_String *wut_CombineStrings(struct wut_String *str1,
                struct wut_String *str2)
{
        struct wut_String *outstr;
        s16 nalloc;

        if(!str1 || !str2) {
                WUT_ALARM(WUT_ERROR, "Invalid parameters");
                return NULL;
        }

        /* Create the output string object */
        nalloc = (str1->size + str2->size) * WUT_STRING_GROWTH;
        if(!(outstr = wut_CreateString(nalloc))) {
                WUT_ALARM(WUT_ERROR, "Failed to create new string");
                return NULL;
        }

        /* 
         * Copy the character from the first and second string into the output
         * string.
         */
        memcpy(outstr->buf, str1->buf, str1->size);
        memcpy(outstr->buf + str1->size, str2->buf, str2->size);

        /*
         * Set the size and length attributes for the new string.
         *
         */
        outstr->length = str1->length + str2->length;
        outstr->size = str1->size + str2->size;
        outstr->alloc = nalloc;

        return outstr;
}


WUT_API s8 wut_WriteString(struct wut_String *str, s16 off, char *txt, s16 len)
{
        s16 overhang;

        if(!str || !txt || off < 0) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return -1;
        }

        if(len == WUT_STRING_AUTO) {
                len = strlen(txt);
        }
 
        overhang = (off + len) - str->size;

        /* Scale the string buffer accordingly */
        if(str_ensure_fit(str, overhang) < 0) {
                return -1;
        }

        /* Copy over the characters and update the nullterminator */
        memcpy(str->buf + off, txt, len);

        str->length += len;
        str->size += len;

        str_set_nullterm(str);

        return 0;
}


WUT_API s8 wut_ExtendString(struct wut_String *str, s16 off, char *txt, s16 len)
{
        s16 i;

        if(!str || !txt) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return -1;
        }

        if(off == WUT_STRING_END) {
                off = str->size;
        }

        if(len == WUT_STRING_AUTO) {
                len = strlen(txt);
        }

        /* Scale the string buffer accordingly */
        if(str_ensure_fit(str, len) < 0) {
                return -1;
        }

        /* Move the trailing characters back to fit the new ones*/
        for(i = str->size - off; i >= 0; i--) {
               str->buf[off + len + i] = str->buf[off + i]; 
        }

        /* Copy over the characters and update the nullterminator */
        memcpy(str->buf + off, txt, len);

        str->length += len;
        str->size += len;
        
        str_set_nullterm(str);

        return 0;
}


WUT_API s8 wut_CropString(struct wut_String *str, s16 start, s16 end)
{
        s16 i;
        s16 len;

        if(!str) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return -1;
        }

        if(end > str->size) {
                end = str->size - 1;
        }

        len = end - start + 1;

        for(i = 0; i < len; i++) {
                str->buf[i] = str->buf[start + i];
        }

        str->length = len;
        str->size = len;

        str_set_nullterm(str);

        return 0;
}
