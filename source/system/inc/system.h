#ifndef _WT_SYSTEM_H
#define _WT_SYSTEM_H

#include "core/inc/define.h"
#include "core/inc/import.h"

/* Catch-all */
#include "system/inc/file.h"

/*
 * Safer reimplementation of malloc().
 *
 * @len: Number of bytes to allocate in the heap
 *
 * Returns: Pointer to the allocated memory or NULL if an error occurred
 */
WT_API void *wt_malloc(s32 len);


/*
 * Safer reimplementation of calloc().
 *
 * @len: Number of bytes to allocate in the heap and set to zero
 *
 * Returns: Pointer to the allocated memory or NULL if an error occurred
 */
WT_API void *wt_calloc(s32 len);


/*
 * The same as malloc(), but overwrites the memory with zeros.
 *
 * @len: The number of bytes to allocate in the heap and set to zero
 *
 * Returns: Pointer to the allocated memory or NULL if an error occurred
 */
WT_API void *wt_zalloc(s32 len);


/* 
 * Safer reimplementation of realloc().
 *
 * @old: Pointer to the old memory
 * @newlen: The number of bytes to allocate
 *
 * Returns: Either a pointer to the reallocated memory or NULL if an error
 *          occurred
 */
WT_API void *wt_realloc(void *old, s32 newlen);


/*
 * Safer reimplementation of free().
 *
 * @p: Pointer to the memory to free
 */
WT_API void wt_free(void *p);


/*
 * Set len bytes to zero, starting from p.
 *
 * @p: The starting pointer
 * @len: The number of bytes to set to zero starting from p
 */
WT_API void wt_zeros(void *p, s32 len);


/*
 * Convert a nullterminated string to an integer.
 *
 * @out: A pointer to write the integer to
 * @s: The nullterminated string
 * @base: The base to interpret the value with(2 to 36)
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_strtoint(s32 *out, char *s, s32 base);


/*
 * Dump memory into the console both in hexadecimal and ASCII characters.
 *
 * @p: Pointer to the memory to dump
 * @len: Number of bytes to dump starting from p
 */
WT_API void wt_bindump(void *p, s32 len);


/*
 * Generate a random string and write it to the given out buffer.
 *
 * @out: A char buffer to write the random string to
 * @size: The number of chars to write
 */
WT_API void wt_rand_string(char *out, u16 size);


#endif /* _WT_SYSTEM_H */
