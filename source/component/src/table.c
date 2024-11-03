#include "component/inc/table.h"

#include "system/inc/system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WUT_INTERN u64 tbl_hash(char *str)
{
        u64 hash = 5381;
        int c;

        while((c = *(str++)))
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
}

WUT_API s8 wut_FindTable(struct wut_Table *tbl, char *name,
                struct wut_TableEntry **ent)
{
        u64 hash;
        struct wut_TableEntry *runner;

        if(!tbl || !name)
                return -1;

        hash = tbl_hash(name);


        runner = tbl->rows[hash % WUT_TBL_ROWS];

        while(runner) {
                if(runner->hash == hash) {
                        if(ent) *ent = runner;
                        return 1;
                }

                runner = runner->next;
        }

        return 0;
}


WUT_API struct wut_Table *wut_CreateTable(void (*fnc)(u32 size, void *ptr))
{
        struct wut_Table *tbl;
        u32 i;

        if(!(tbl = wut_malloc(sizeof(struct wut_Table))))
                return NULL;

        tbl->number = 0;
        for(i = 0; i < WUT_TBL_ROWS; i++)
                tbl->rows[i] = NULL;

        tbl->fnc = fnc;

        return tbl;
}


WUT_API void wut_DestroyTable(struct wut_Table *tbl)
{
        struct wut_TableEntry *runner;
        struct wut_TableEntry *next;
        u32 i;

        if(!tbl)
                return;

        printf("Destroy table\n");

        if(tbl->number < 1) {
                wut_free(tbl);
                printf("empty\n");
                return;
        }


        for(i = 0; i < WUT_TBL_ROWS; i++) {
                runner = tbl->rows[i];

                while(runner) {
                        next = runner->next;

                        if(tbl->fnc)
                                tbl->fnc(runner->size, runner->ptr);

                        wut_free(runner);

                        runner = next;
                }
        }

        wut_free(tbl);
}


WUT_API s8 wut_AddTable(struct wut_Table *tbl, char *name, u32 size, void **ptr)
{
        struct wut_TableEntry *ent;
        struct wut_TableEntry *runner;
        u32 row;

        if(!tbl)
                return -1;

        /* Check if there is already an entry with that name */
        if(wut_FindTable(tbl, name, NULL)) {
                printf("entry with same name already in table\n");
                return -1;
        }


        if(!(ent = wut_malloc(sizeof(struct wut_TableEntry)))) {
                printf("Failed to allocate memory for new entry\n");
                return -1;
        }


        strcpy(ent->name, name);
        ent->hash = tbl_hash(name);
        ent->next = NULL;
        ent->prev = NULL;
        ent->size = size;
        ent->ptr = *ptr;	

        row = ent->hash % WUT_TBL_ROWS;

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


WUT_API void wut_RemoveTable(struct wut_Table *tbl, char *name)
{
        struct wut_TableEntry *ent;
        u32 row;

        if(!tbl || !name)
                return;


        if(wut_FindTable(tbl, name, &ent) != 1)
                return;

        row = ent->hash % WUT_TBL_ROWS;

        if(ent->prev == NULL) {
                tbl->rows[row] = ent->next;
        }
        else {
                (ent->prev)->next = ent->next;
        }

        if(tbl->fnc)
                tbl->fnc(ent->size, ent->ptr);

        wut_free(ent);
        tbl->number--;
}


WUT_API s8 wut_GetTable(struct wut_Table *tbl, char *name, u32 *size, void **ptr)
{
        struct wut_TableEntry *ent;

        if(!tbl || !name)
                return -1;

        if(wut_FindTable(tbl, name, &ent) != 1)
                return 0;

        if(size) *size = ent->size;
        if(ptr) *ptr = ent->ptr;
        return 1;
}


WUT_API void wut_DumpTable(struct wut_Table *tbl)
{
        u32 i;
        struct wut_TableEntry *runner;	

        if(!tbl)
                return;

        for(i = 0; i < WUT_TBL_ROWS; i++) {
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
