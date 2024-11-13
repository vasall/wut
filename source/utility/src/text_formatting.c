#include "source/utility/inc/text_formatting.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WUT_XMOD void wut_tfm_to_lowercase(char *str)
{       
        char *r = str;

        while(*r) {
                if(*r >= 0x41 && *r <= 0x5A)
                        *r += 0x20;

                r++;
        }

        /* Write the null-terminator */
        *r = 0;
}


WUT_XMOD void wut_tfm_to_uppercase(char *str)
{       
        char *r = str;

        while(*r) {
                if(*r >= 0x61 && *r <= 0x7A)
                        *r -= 0x20;

                r++;
        }

        /* Write the null-terminator */
        *r = 0;
}


WUT_XMOD s8 wut_tfm_is_space(char c)
{
        return c == 0x20;
}


WUT_XMOD s8 wut_tfm_is_letter(char c)
{
        /* Uppercase letters A..Z */
        if(c >= 0x41 && c <= 0x5A)
                return 1;

        /* Lowercase letters a..z */
        if(c >= 0x61 && c <= 0x7A)
                return 1;

        return 0;
}


WUT_XMOD s8 wut_tfm_is_number(char c)
{
        if(c >= 0x30 && c <= 0x39)
                return 1;

        return 0;
}


WUT_XMOD s8 wut_tfm_is_hexcode(char c)
{
        return (wut_tfm_is_letter(c) || wut_tfm_is_number(c));
}


WUT_XMOD char *wut_tfm_trim(char *str)
{
        char *end;

        /* Trim leading space */
        while (wut_tfm_is_space(*str)) str++;

        /* All spaces? */
        if (*str == 0)
                return str;

        /* Trim trailing space */
        end = str + strlen(str) - 1;
        while (end > str && wut_tfm_is_space(*end)) end--;

        /* Write new null terminator */
        *(end + 1) = 0;

        return str;
}


WUT_XMOD void wut_tfm_conform(char *str)
{
        while(*str) {
                if(*str < 0x20 || *str > 0x7E)
                        *str = 0x20;

                str++;
        }
}


WUT_XMOD void wut_tfm_reduce(char *str)
{
        char *ptr = str;
        s8 count = 0;

        while(*ptr) {
                /* If a space is detected */
                if(*ptr == 0x20) {
                        if(count < 1) {
                                *str = *ptr;
                                str++;
                                count = 1;
                        }
                }
                else {
                        *str = *ptr;
                        str++;

                        count = 0;
                }

                ptr++;
        }

        *str = 0;
}


WUT_XMOD void wut_tfm_strip(char *str)
{
        char *d = str;

        do {
                while(*d == 0x20)
                        d++;

        } while((*(str++) = *(d++)));
}
