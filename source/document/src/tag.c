#include "source/document/inc/tag.h"

#include "source/utility/inc/text_formatting.h"

#include <stdlib.h>
#include <string.h>


WUT_API enum wut_eTag wut_tag_get(char *str)
{
        wut_tfm_to_lowercase(str); 

        if(strcmp("body", str) == 0)    return WUT_BODY;
        if(strcmp("block", str) == 0)   return WUT_BLOCK;
        if(strcmp("text", str) == 0)    return WUT_TEXT;
        if(strcmp("button", str) == 0)  return WUT_BUTTON;
        if(strcmp("input", str) == 0)   return WUT_INPUT;
        if(strcmp("image", str) == 0)   return WUT_IMAGE;
        if(strcmp("view", str) == 0)    return WUT_VIEW;

        return WUT_UNDEF;
}

WUT_API void wut_tag_name(enum wut_eTag tag, char *out)
{
        if(tag == WUT_BODY)             strcpy(out, "body");
        else if(tag == WUT_BLOCK)       strcpy(out, "block");
        else if(tag == WUT_TEXT)        strcpy(out, "text");
        else if(tag == WUT_BUTTON)      strcpy(out, "button");
        else if(tag == WUT_INPUT)       strcpy(out, "input");
        else if(tag == WUT_IMAGE)       strcpy(out, "image");
        else if(tag == WUT_VIEW)        strcpy(out, "view");
        else                            strcpy(out, "undef");
}
