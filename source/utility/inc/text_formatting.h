#ifndef _WUT_UTILITY_TEXT_FORMATTING_H
#define _WUT_UTILITY_TEXT_FORMATTING_H

#include "source/core/inc/define.h"


/*
 * Convert all letters in a string to lowercase.
 *
 * @str: The input string
 */
WUT_XMOD void wut_tfm_to_lowercase(char *str);


/*
 * Convert all letters in a string to uppercase.
 *
 * @str: The input string
 */
WUT_XMOD void wut_tfm_to_uppercase(char *str);


/*
 * Check if the given character is a space-character(ASCII: 0x20).
 *
 * @c: The character to check
 *
 * Returns: 1 if the character is a space or 0 if not
 */
WUT_XMOD s8 wut_tfm_is_space(char c);


/*
 * Check if the given character is a letter(ASCII: 0x41..0x5A and 0x61..0x7A).
 *
 * @c: The character to check
 *
 * Returns: 1 if the character is a letter or 0 if not
 */
WUT_XMOD s8 wut_tfm_is_letter(char c);


/*
 * Check if the given character is a number(ASCII: 0x30..0x39).
 *
 * @c: The character to check
 *
 * Returns: 1 if the character is a number or 0 if not
 */
WUT_XMOD s8 wut_tfm_is_number(char c);


/*
 * Check if the given character is valid hexcode, so both uppercase and
 * lowercase letter or a number. (ASCII: 0x41..0x5A and 0x61..0x7A and
 * 0x30..0x39).
 *
 * @c: The character to check
 *
 * Returns: 1 if the character is valid hexcode or 0 if not
 */
WUT_XMOD s8 wut_tfm_is_hexcode(char c);


/*
 * Remove all leading and trailing spaces from a string.
 *
 * @str: The input string
 *
 * Returns: The trimmed string
 */
WUT_XMOD char *wut_tfm_trim(char *str);


/*
 * Confirm that the given string only consists of the following types of
 * characters:
 *
 *      - Letters (a..zA..Z)
 *      - Numbers (0..1)
 *      - Allowed special characters
 *
 * Everything else will be turned into a space.
 *
 * @str: Pointer to the string which will be updated to conform to standards
 */
WUT_XMOD void wut_tfm_conform(char *str);


/*
 * Reduce the seperating number of spaces between tokens to 1.
 * This function will work on the given string and modify it.
 *
 * @str: The input string to reduce
 */
WUT_XMOD void wut_tfm_reduce(char *str);


/*
 * Remove all spaces in a string.
 *
 * @str: The input string
 */
WUT_XMOD void wut_tfm_strip(char *str);


#endif /* _WUT_UTILITY_TEXT_FORMATTING_H */
