#ifndef _DOCUMENT_TAG_H
#define _DOCUMENT_TAG_H

#include "source/core/inc/define.h"

enum wut_eTag {
        WUT_UNDEF               = -1,
        WUT_BODY		=  0,
        WUT_BLOCK	        =  1,
        WUT_TEXT		=  2,
        WUT_BUTTON	        =  3,
        WUT_INPUT	        =  4,
        WUT_IMAGE	        =  5,
        WUT_VIEW		=  6,
        WUT_CUSTOM	        =  7
};


/*
 * Get the tag from the name.
 *
 * @str: The string containing the tag name
 *
 * Returns: The tag, including -1 or WUT_UNDEF which means an error occurred
 */
WUT_API enum wut_eTag wut_tag_get(char *str);



#endif /* _DOCUMENT_TAG_H */
