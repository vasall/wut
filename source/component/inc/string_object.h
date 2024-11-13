#ifndef _WUT_COMPONENT_STRING_OBJECT_H
#define _WUT_COMPONENT_STRING_OBJECT_H

#include "source/core/inc/define.h"

#define WUT_STRING_GROWTH               1.5

#define WUT_STRING_END                  -1
#define WUT_STRING_START                0


/*
 * NOTE:
 * The string component is only necessary for two usecases:
 *
 *  - If the string has to scale dynamically
 *  [ - The string needs to support UTF-8 ]
 *
 *  For all other reasons, fall back to regular C-strings.
 *
 *  I also have to name this file string_object because naming it string.h
 *  causes conflicts with the default string.h files.
 */

/*
 * TODO:
 * In the future this component has to be expanded to support UTF-8
 * functionality. But for now we just work with ASCII.
 */

struct wut_String {
        s16     length; /* The number of characters in the string */

        s16     size;   /* The size of the string in bytes */
        s16     alloc;  /* The number of allocated bytes in the buffer */

        char    *buf;   /* The buffer containing the string */
};


/*
 * Create a new string object and preallocate the given number of bytes.
 *
 * @alloc: The number of bytes to preallocate
 *
 * Returns: Either a pointer to the newly created string object or NULL if an
 *          error occurred
 */
WUT_API struct wut_String *wut_CreateString(s16 alloc);


/*
 * Destroy a string and free up the allocated memory.
 *
 * @str: Pointer to the string object
 */
WUT_API void wut_DestroyString(struct wut_String *str);


/*
 * Clear the string and reset it's attributes to zero.
 *
 * @str: Pointer to the string object
 */
WUT_API void wut_ClearString(struct wut_String *str);


/*
 * Get the characters from the string object as a regular C-string.
 *
 * @str: Pointer to the string object
 *
 * Returns: The C-string
 */
WUT_API char *wut_GetString(struct wut_String *str);

/*
 * Combine two strings to one new one, by attaching strings 2 to string 1.
 * This will not destroy the two input strings, so if you don't need them
 * anymore, remember to clear both of them.
 *
 * @str1: The first string
 * @str2: The second string appended to the first one
 *
 * Returns: Either a new string or NULL if an error occurred
 */
WUT_API struct wut_String *wut_CombineStrings(struct wut_String *str1,
                struct wut_String *str2);


/*
 * This function will write the given characters to the string. If you specify
 * an offset in the middle of the string, the trailing characters will be
 * overwritten. If you want to just add the new characters, use the extend
 * function instead.
 *
 * @str: Pointer to the string object
 * @off: The offset to write the new characters to
 * @txt: The new characters to write to the string
 *
 * Returns: 0 if successfull or -1 if an error occurred
 */
WUT_API s8 wut_WriteString(struct wut_String *str, s16 off, char *txt);


/*
 * Append characters to the string. If you specify an offset, the trailing
 * characters will be moved back to fit the incoming ones, so nothing will be
 * overwritten.
 *
 * @str: Pointer to the string object
 * @off: The offset to place the new characters into in characters
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_ExtendString(struct wut_String *str, s16 off, char *txt);


/*
 * Crop a string to the given limits.
 *
 * @str: Pointer to the string object
 * @start: The starting character to start cropping from
 * @end: The end character to crop to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_CropString(struct wut_String *str, s16 start, s16 end);

#endif /* _WUT_COMPONENT_STRING_OBJECT_H */
