#ifndef _WT_UTILITY_TABLE_H
#define _WT_UTILITY_TABLE_H


#include "core/inc/define.h"
#include "core/inc/import.h"

#define WT_TABLE_NAME_LIM	128
#define WT_TABLE_ROWS		128

struct wt_table_entry {
	/* The name of the entry */
	char name[WT_TABLE_NAME_LIM];

	/* The hash-value for this entry */
	u64 hash;

	/* The size of the data buffer in bytes */
	u32 size;

	/* A pointer to the data buffer for this entry */
	void *ptr;

	/* References to the previous and next entry in the list */
	struct wt_table_entry *next;
	struct wt_table_entry *prev;
};


struct wt_table {
	/* The total number of entries in the table */
	u32 number;

	/* The rows in the table */
	struct wt_table_entry *rows[WT_TABLE_ROWS];

	/* The remove callback function */
	void (*fnc)(u32 size, void *data);
};



WT_API u64 wt_table_hash(char *str);
WT_API s8 wt_table_find(struct wt_table *tbl, char *name,
		struct wt_table_entry **ent);


/*
 * Create a new table. Optionally, a callback function can be specified, which
 * will be executed in case an element will be removed.
 *
 * @fnc: The callback function to execute if an entry will be removed
 *
 * Returns: Either a pointer to the newly created table or NULL if an error
 * 	    occurred
 */
WT_API struct wt_table *wt_tbl_create(void (*fnc)(u32 size, void *ptr));


/*
 * Destroy a table and free the allocated memory. This function is save to call,
 * even if tbl is NULL.
 *
 * @tbl: Pointer to the table
 */
WT_API void wt_tbl_destroy(struct wt_table *tbl);


/*
 * Add a new entry to the table. Note that the function will only attach the
 * pointer, but will not allocate a data buffer by itself.
 *
 * @tbl: Pointer to the table
 * @name: The name of the entry
 * @size: The size of the data buffer in bytes
 * @ptr: A pointer that will be attached to the entry
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_tbl_add(struct wt_table *tbl, char *name, u32 size, void **ptr);


/*
 * Remove an entry from the table.
 *
 * @tbl: Pointer to the table
 * @name: The name of the entry
 */
WT_API void wt_tbl_rmv(struct wt_table *tbl, char *name);


/*
 * Retrieve the data pointer of an entry.
 *
 * @tbl: Pointer to the table
 * @name: The name of the entry
 * @size: A pointer to write the size of the data-buffer
 * @ptr: A pointer to write the data-pointer to
 *
 * Returns: 1 if entry has been found, 0 if not and -1 if an error occurred
 */
WT_API s8 wt_tbl_get(struct wt_table *tbl, char *name, u32 *size, void **ptr);


/*
 * Dump the content of the table in the console.
 *
 * @tbl: Pointer to the table
 */
WT_API void wt_tbl_dump(struct wt_table *tbl);

#endif /* _WT_UTILITY_TABLE_H */
