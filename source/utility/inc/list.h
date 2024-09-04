#ifndef _WUT_UTILITY_LIST_H
#define _WUT_UTILITY_LIST_H

#include "core/inc/define.h"
#include "core/inc/import.h"


struct wut_List {
        s16 size;	/* The size of a single entry */

        s16 count;	/* The number of entries */
        s16 alloc;	/* The allocated number of entries */

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
WUT_API struct wut_List *wut_CreateList(s16 size, s16 alloc);


/*
 * Destroy a list and free the allocated memory.
 *
 * @lst: Pointer to the list
 */
WUT_API void wut_DestroyList(struct wut_List *lst);


/*
 * Push a new entry onto the list.
 *
 * @lst: Pointer to the list
 * @inp: Pointer to the new entry
 *
 * Returns: On success the number of elements in the list and if an error
 *          occurred -1
 */
WUT_API s16 wut_PushList(struct wut_List *lst, void *inp);


/*
 * Pop the newest entry from the list and write it to the given pointer. Note,
 * this function returns 0 if the list is empty on call.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the popped entry to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
WUT_API s16 wut_PopList(struct wut_List *lst, void *out);


/*
 * Get the oldest element in the list.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the shifted element to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
WUT_API s16 wut_ShiftList(struct wut_List *lst, void *out);


/*
 * Get the oldest element in the list without removing it.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the element to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
WUT_API s16 wut_TestListHead(struct wut_List *lst, void *out);


/*
 * Get the newest element in the list without removing it.
 *
 * @lst: Pointer to the list
 * @out: A pointer to write the element to
 *
 * Returns: 1 on success, 0 if list is empty and -1 if an error occurred
 */
WUT_API s16 wut_TestListTail(struct wut_List *lst, void *out);


/*
 * Get the element at the index from the list without removing it and set the
 * given pointer to the address of the element in the list.
 * Note that if either an error occurred or the element doesn't exist, the
 * pointer will not be updated.
 *
 * @lst: Pointer to the list
 * @idx: The index of the element to get
 * @out: A pointer to write the element address to
 *
 * Returns: 1 if an element has been returned, 0 if empty or out of bounds and
 * 	    -1 if an error occurred
 */
WUT_API s8 wut_GetList(struct wut_List *lst, u16 idx, void **out);


/*
 * Create an identical copy of the given list and return it. This will not
 * modify or change the original list in any way.
 *
 * @src: The source list to copy
 *
 * Returns: Either the created copy or NULL if an error occurred
 */
WUT_API struct wut_List *wut_DuplicateList(struct wut_List *src);

typedef s8 (*wut_ListFunc)(void *entry, s16 idx, void *data);


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
WUT_API s8 wut_ApplyList(struct wut_List *lst, wut_ListFunc fnc, void *data);

#endif /* _WUT_UTILITY_LIST_H */
