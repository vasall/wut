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
        if(strcmp("custom", str) == 0)  return WUT_CUSTOM;

        return WUT_UNDEF;
}
