#ifndef _WT_UTILITY_STATIC_LIST_H
#define _WT_UTILITY_STATIC_LIST_H

#include "core/inc/define.h"
#include "core/inc/import.h"

struct wt_statlist {
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
WT_API struct wt_statlist *wt_statlist_create(s16 size, s16 alloc);


/*
 * Destroy a list and free the allocated memory.
 *
 * @lst: Pointer to the list
 */
WT_API void wt_statlist_destroy(struct wt_statlist *lst);


/*
 * Push a new entry onto the list.
 *
 * @lst: Pointer to the list
 * @inp: Pointer to the new entry
 *
 * Returns: The index of the slot in the list or -1 if an error occurred
 */
WT_API s16 wt_statlist_add(struct wt_statlist *lst, void *inp);


/*
 * Remove an entry from the list.
 *
 * @lst: Pointer to the list
 * @idx: The index of the slot to clear
 */
WT_API void wt_statlist_rmv(struct wt_statlist *lst, s16 idx);


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
WT_API s8 wt_statlist_get(struct wt_statlist *lst, s16 idx, void *out);


typedef s8 (*wt_statlist_fnc_t)(void *entry, s16 idx, void *data);


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
WT_API s8 wt_statlist_apply(struct wt_statlist *lst, wt_statlist_fnc_t fnc, void *data);



#endif /* _WT_UTILITY_STATIC_LIST_H */
