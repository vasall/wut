#ifndef _WUT_COMPONENT_DICTIONARY_H
#define _WUT_COMPONENT_DICTIONARY_H

#include "source/core/inc/define.h"

#define WUT_DIC_MAX_NUMBER      128
#define WUT_DIC_MAX_LENGTH      128

struct wut_Dictionary {
        s16 number;
        char entries[2][WUT_DIC_MAX_NUMBER][WUT_DIC_MAX_LENGTH]; 
};


/*
 * Reset and clear all entries in the dictionary. This function should always be
 * called before using the dictionary for the first time.
 *
 * @dic: Pointer to the dictionary
 */
WUT_API void wut_ClearDictionary(struct wut_Dictionary *dic);


/*
 * Set a new keyword-value-pair in the dictionary. And if the keyword is already
 * registered, overwrite the value.
 *
 * @dic: Pointer to the dictionary
 * @key: The string containing the keyword
 * @value: The string containing the value
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_SetDictionary(struct wut_Dictionary *dic,
                char *key, char *value);


/*
 * Remove a keyword-value-pair from the dictionary.
 *
 * @dic: Pointer to the dictionary
 * @key: The  string containing the keyword
 */
WUT_API void wut_ResetDictionary(struct wut_Dictionary *dic,
                char *key);


/*
 * Retrieve a value from the dictionary through the keyword.
 *
 * @dic: Pointer to the dictionary
 * @key: The string containing the keyword
 *
 * Returns: A poointer to the interal value-string, or NULL if either the
 *          keyword doesn't  exist or an error occurred
 */
WUT_API char *wut_GetDictionary(struct wut_Dictionary *dic,
                char *key);


/*
 * Print the contents of a dictionary in the console.
 *
 * @dic: Pointer to the dictionary
 */
WUT_API void wut_PrintDictionary(struct wut_Dictionary *dic);


#endif /* _WUT_COMPONENT_DICTIONARY_H */
