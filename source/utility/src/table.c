#include "utility/inc/table.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WT_API u64 wt_table_hash(char *str)
{
	u64 hash = 5381;
	int c;

	while((c = *(str++)))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

WT_API s8 wt_table_find(struct wt_table *tbl, char *name,
		struct wt_table_entry **ent)
{
	u64 hash;
	struct wt_table_entry *runner;

	if(!tbl || !name)
		return -1;

	hash = wt_table_hash(name);

	
	runner = tbl->rows[hash % WT_TABLE_ROWS];

	while(runner) {
		if(runner->hash == hash) {
			if(ent) *ent = runner;
			return 1;
		}

		runner = runner->next;
	}

	return 0;
}


WT_API struct wt_table *wt_tbl_create(void (*fnc)(u32 size, void *ptr))
{
	struct wt_table *tbl;
	u32 i;

	if(!(tbl = wt_malloc(sizeof(struct wt_table))))
		return NULL;

	tbl->number = 0;
	for(i = 0; i < WT_TABLE_ROWS; i++)
		tbl->rows[i] = NULL;

	tbl->fnc = fnc;

	return tbl;
}


WT_API void wt_tbl_destroy(struct wt_table *tbl)
{
	struct wt_table_entry *runner;
	struct wt_table_entry *next;
	u32 i;

	if(!tbl)
		return;

	printf("Destroy table\n");

	if(tbl->number < 1) {
		wt_free(tbl);
		printf("empty\n");
		return;
	}


	for(i = 0; i < WT_TABLE_ROWS; i++) {
		runner = tbl->rows[i];

		while(runner) {
			next = runner->next;

			if(tbl->fnc)
				tbl->fnc(runner->size, runner->ptr);

			wt_free(runner);

			runner = next;
		}
	}

	wt_free(tbl);
}


WT_API s8 wt_tbl_add(struct wt_table *tbl, char *name, u32 size, void **ptr)
{
	struct wt_table_entry *ent;
	struct wt_table_entry *runner;
	u32 row;

	if(!tbl)
		return -1;

	/* Check if there is already an entry with that name */
	if(wt_table_find(tbl, name, NULL)) {
		printf("entry with same name already in table\n");
		return -1;
	}


	if(!(ent = wt_malloc(sizeof(struct wt_table_entry)))) {
		printf("Failed to allocate memory for new entry\n");
		return -1;
	}


	strcpy(ent->name, name);
	ent->hash = wt_table_hash(name);
	ent->next = NULL;
	ent->prev = NULL;
	ent->size = size;
	ent->ptr = *ptr;	

	row = ent->hash % WT_TABLE_ROWS;
	
	if(tbl->rows[row] == NULL) {
		tbl->rows[row] = ent;
		tbl->number++;
		return 0;
	}
	
	runner = tbl->rows[row];

	while(runner) {
		if(runner->next == NULL) {
			runner->next = ent;
			ent->prev = runner;
			tbl->number++;
			return 0;
		}

		runner = runner->next;
	}

	printf("WTF¿n\n");
	return -1;
}


WT_API void wt_tbl_rmv(struct wt_table *tbl, char *name)
{
	struct wt_table_entry *ent;
	u32 row;

	if(!tbl || !name)
		return;


	if(wt_table_find(tbl, name, &ent) != 1)
		return;

	row = ent->hash % WT_TABLE_ROWS;

	if(ent->prev == NULL) {
		tbl->rows[row] = ent->next;
	}
	else {
		(ent->prev)->next = ent->next;
	}

	if(tbl->fnc)
		tbl->fnc(ent->size, ent->ptr);

	wt_free(ent);
	tbl->number--;
}


WT_API s8 wt_tbl_get(struct wt_table *tbl, char *name, u32 *size, void **ptr)
{
	struct wt_table_entry *ent;

	if(!tbl || !name)
		return -1;

	if(wt_table_find(tbl, name, &ent) != 1)
		return 0;

	if(size) *size = ent->size;
	if(ptr) *ptr = ent->ptr;
	return 1;
}


WT_API void wt_tbl_dump(struct wt_table *tbl)
{
	u32 i;
	struct wt_table_entry *runner;	

	if(!tbl)
		return;

	for(i = 0; i < WT_TABLE_ROWS; i++) {
		printf("Row %d:\n", i);

		if(!(runner = tbl->rows[i])) {
			printf("EMPTY\n");
			continue;
		}
		
		while(runner) {
			printf(" %s\n", runner->name);
			runner = runner->next;
		}
	}
}
