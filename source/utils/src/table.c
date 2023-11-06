#include "utils/inc/table.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FH_API u64 fh_table_hash(char *str)
{
	u64 hash = 5381;
	int c;

	while((c = *(str++)))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

FH_API s8 fh_table_find(struct fh_table *tbl, char *name,
		struct fh_table_entry **ent)
{
	u64 hash;
	struct fh_table_entry *runner;

	if(!tbl || !name)
		return -1;

	hash = fh_table_hash(name);

	
	runner = tbl->rows[hash % FH_TABLE_ROWS];

	while(runner) {
		if(runner->hash == hash) {
			if(ent) *ent = runner;
			return 1;
		}

		runner = runner->next;
	}

	return 0;
}


FH_API struct fh_table *fh_tbl_create(void (*fnc)(u32 size, void *ptr))
{
	struct fh_table *tbl;
	u32 i;

	if(!(tbl = fh_malloc(sizeof(struct fh_table))))
		return NULL;

	tbl->number = 0;
	for(i = 0; i < FH_TABLE_ROWS; i++)
		tbl->rows[i] = NULL;

	tbl->fnc = fnc;

	return tbl;
}


FH_API void fh_tbl_destroy(struct fh_table *tbl)
{
	struct fh_table_entry *runner;
	struct fh_table_entry *next;
	u32 i;

	if(!tbl)
		return;

	printf("Destroy table\n");

	if(tbl->number < 1) {
		fh_free(tbl);
		printf("empty\n");
		return;
	}


	for(i = 0; i < FH_TABLE_ROWS; i++) {
		runner = tbl->rows[i];

		while(runner) {
			next = runner->next;

			if(tbl->fnc)
				tbl->fnc(runner->size, runner->ptr);

			fh_free(runner);

			runner = next;
		}
	}

	fh_free(tbl);
}


FH_API s8 fh_tbl_add(struct fh_table *tbl, char *name, u32 size, void **ptr)
{
	struct fh_table_entry *ent;
	struct fh_table_entry *runner;
	u32 row;

	if(!tbl)
		return -1;

	/* Check if there is already an entry with that name */
	if(fh_table_find(tbl, name, NULL))
		return -1;


	if(!(ent = fh_malloc(sizeof(struct fh_table_entry))))
		return -1;


	strcpy(ent->name, name);
	ent->hash = fh_table_hash(name);
	ent->next = NULL;
	ent->prev = NULL;
	ent->size = size;
	ent->ptr = *ptr;	

	row = ent->hash % FH_TABLE_ROWS;
	
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

	return -1;
}


FH_API void fh_tbl_rmv(struct fh_table *tbl, char *name)
{
	struct fh_table_entry *ent;
	u32 row;

	if(!tbl || !name)
		return;


	if(fh_table_find(tbl, name, &ent) != 1)
		return;

	row = ent->hash % FH_TABLE_ROWS;

	if(ent->prev == NULL) {
		tbl->rows[row] = ent->next;
	}
	else {
		(ent->prev)->next = ent->next;
	}

	if(tbl->fnc)
		tbl->fnc(ent->size, ent->ptr);

	fh_free(ent);
	tbl->number--;
}


FH_API s8 fh_tbl_get(struct fh_table *tbl, char *name, u32 *size, void **ptr)
{
	struct fh_table_entry *ent;

	if(!tbl || !name)
		return -1;

	if(fh_table_find(tbl, name, &ent) != 1)
		return 0;

	if(size) *size = ent->size;
	if(ptr) *ptr = ent->ptr;
	return 1;
}


FH_API void fh_tbl_dump(struct fh_table *tbl)
{
	u32 i;
	struct fh_table_entry *runner;	

	if(!tbl)
		return;

	for(i = 0; i < FH_TABLE_ROWS; i++) {
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
