#ifndef _WUT_UTILITY_TABLE_H
#define _WUT_UTILITY_TABLE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#define WUT_TBL_NAME_LIM	128
#define WUT_TBL_ROWS		128

struct wut_TableEntry {
        /* The name of the entry */
        char name[WUT_TBL_NAME_LIM];

        /* The hash-value for this entry */
        u64 hash;

        /* The size of the data buffer in bytes */
        u32 size;

        /* A pointer to the data buffer for this entry */
        void *ptr;

        /* References to the previous and next entry in the list */
        struct wut_TableEntry *next;
        struct wut_TableEntry *prev;
};


struct wut_Table {
        /* The total number of entries in the table */
        u32 number;

        /* The rows in the table */
        struct wut_TableEntry *rows[WUT_TBL_ROWS];

        /* The remove callback function */
        void (*fnc)(u32 size, void *data);
};



WUT_API s8 wut_FindTable(struct wut_Table *tbl, char *name,
                struct wut_TableEntry **ent);


/*
 * Create a new table. Optionally, a callback function can be specified, which
 * will be executed in case an element will be removed.
 *
 * @fnc: The callback function to execute if an entry will be removed
 *
 * Returns: Either a pointer to the newly created table or NULL if an error
 * 	    occurred
 */
WUT_API struct wut_Table *wut_CreateTable(void (*fnc)(u32 size, void *ptr));


/*
 * Destroy a table and free the allocated memory. This function is save to call,
 * even if tbl is NULL.
 *
 * @tbl: Pointer to the table
 */
WUT_API void wut_DestroyTable(struct wut_Table *tbl);


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
WUT_API s8 wut_AddTable(struct wut_Table *tbl, char *name,
                u32 size, void **ptr);


/*
 * Remove an entry from the table.
 *
 * @tbl: Pointer to the table
 * @name: The name of the entry
 */
WUT_API void wut_RemoveTable(struct wut_Table *tbl, char *name);


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
WUT_API s8 wut_GetTable(struct wut_Table *tbl, char *name,
                u32 *size, void **ptr);


/*
 * Dump the content of the table in the console.
 *
 * @tbl: Pointer to the table
 */
WUT_API void wut_DumpTable(struct wut_Table *tbl);

#endif /* _WUT_UTILITY_TABLE_H */
