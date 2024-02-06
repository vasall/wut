#ifndef _FH_UTILITY_STATIC_LIST_H
#define _FH_UTILITY_STATIC_LIST_H

#include "core/inc/define.h"
#include "core/inc/import.h"

struct fh_statlist {
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
FH_API struct fh_statlist *fh_statlist_create(s16 size, s16 alloc);


/*
 * Destroy a list and free the allocated memory.
 *
 * @lst: Pointer to the list
 */
FH_API void fh_statlist_destroy(struct fh_statlist *lst);


/*
 * Push a new entry onto the list.
 *
 * @lst: Pointer to the list
 * @inp: Pointer to the new entry
 *
 * Returns: The index of the slot in the list or -1 if an error occurred
 */
FH_API s16 fh_statlist_add(struct fh_statlist *lst, void *inp);


/*
 * Remove an entry from the list.
 *
 * @lst: Pointer to the list
 * @idx: The index of the slot to clear
 */
FH_API void fh_statlist_rmv(struct fh_statlist *lst, s16 idx);


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
FH_API s8 fh_statlist_get(struct fh_statlist *lst, s16 idx, void *out);


typedef s8 (*fh_statlist_fnc_t)(void *entry, s16 idx, void *data);


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
FH_API s8 fh_statlist_apply(struct fh_statlist *lst, fh_statlist_fnc_t fnc, void *data);



#endif /* _FH_UTILITY_STATIC_LIST_H */
