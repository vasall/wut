#ifndef _WUT_UTILITY_STATIC_LIST_H
#define _WUT_UTILITY_STATIC_LIST_H

#include "core/inc/define.h"
#include "core/inc/import.h"

struct wut_StatList {
        s16 size;	/* The size of a single entry */

        s16 count;	/* The number of entries */
        s16 alloc;

        u8 *mask;
        u8 *data;
};


/*
 * Create a new list and preallocate the number of entries.
 *
 * @size: The size of a single entry in bytes
 * @alloc: The number of slots to preallocate
 *
 * Returns: Either a pointer to the newly created list or NULL if an error
 * 	    occurred
 */
WUT_API struct wut_StatList *wut_CreateStatList(s16 size, s16 alloc);


/*
 * Destroy a list and free the allocated memory.
 *
 * @lst: Pointer to the list
 */
WUT_API void wut_DestroyStatList(struct wut_StatList *lst);


/*
 * Push a new entry onto the list.
 *
 * @lst: Pointer to the list
 * @inp: Pointer to the new entry
 *
 * Returns: The index of the slot in the list or -1 if an error occurred
 */
WUT_API s16 wut_AddStatList(struct wut_StatList *lst, void *inp);


/*
 * Remove an entry from the list.
 *
 * @lst: Pointer to the list
 * @idx: The index of the slot to clear
 */
WUT_API void wut_RemoveStatList(struct wut_StatList *lst, s16 idx);


/*
 * Get an entry from the list.
 *
 * @lst: Pointer to the list
 * @idx: The index of the entry in the list
 * @out: A pointer to write the output to
 *
 * Returns: 1 if entry was returned and 0 if slot is empty and -1 if an error
 * 	    occurred
 */
WUT_API s8 wut_GetStatList(struct wut_StatList *lst, s16 idx, void *out);


typedef s8 (*wut_StatListFunc)(void *entry, s16 idx, void *data);


/*
 * Apply a function to all elements in the lits, from the oldest to the newest
 * entry. If the given function returns 1, the function will stop and return 0.
 *
 * @lst: Pointer to the list
 * @fnc: The function to apply to all entries
 * @data: A data-pointer passed on every call
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_ApplyStatList(struct wut_StatList *lst,
                wut_StatListFunc fnc, void *data);



#endif /* _WUT_UTILITY_STATIC_LIST_H */
