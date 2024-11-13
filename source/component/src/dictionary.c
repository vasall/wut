#include "source/component/inc/dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




WUT_API void wut_ClearDictionary(struct wut_Dictionary *dic)
{
        s16 i;

        if(!dic) return;

        /* Set all keywords as empty */
        for(i = 0; i < WUT_DIC_MAX_NUMBER; i++) {
                *dic->entries[0][i] = 0;
        }

        dic->number = 0;
}


WUT_API s8 wut_SetDictionary(struct wut_Dictionary *dic,
                char *key, char *value)
{ 
        s16 i;

        if(!dic) return -1;

        /* 
         * First check if the keyword is already registered, and if yes,
         * overwrite the value. 
         */
        for(i = 0; i < WUT_DIC_MAX_NUMBER; i++) {
                if(strcmp(key, dic->entries[0][i]) == 0) {
                        strcpy(dic->entries[1][i], value);
                        return 0;
                }
        }

        /*
         * If the keyword is not yet registered, find an empty slot and write
         * it.
         */
        for(i = 0; i < WUT_DIC_MAX_NUMBER; i++) {
                if(*dic->entries[0][i] == 0) {
                        strcpy(dic->entries[0][i], key);
                        strcpy(dic->entries[1][i], value);
                        dic->number++;
                        return 0;
                }
        }

        /*
         * If neither option was successfull, return error.
         */
        return -1;
}


WUT_API void wut_ResetDictionary(struct wut_Dictionary *dic,
                char *key)
{
        s16 i;

        if(!dic) return;

        /*
         * Search list for keyword and then clear it.
         */
        for(i = 0; i < WUT_DIC_MAX_NUMBER; i++) {
                if(strcmp(dic->entries[0][i], key) == 0) {
                        *dic->entries[0][i] = 0;
                        dic->number--;
                        return;
                }
        }
}


WUT_API char *wut_GetDictionary(struct wut_Dictionary *dic,
                char *key)
{
        s16 i;

        if(!dic) return NULL;

        for(i = 0; i < WUT_DIC_MAX_NUMBER; i++) {
                if(strcmp(dic->entries[0][i], key) == 0) {
                        return dic->entries[1][i];
                }
        }

        return NULL;
}


WUT_API void wut_PrintDictionary(struct wut_Dictionary *dic)
{
        s16 i;

        if(!dic) return;

        for(i = 0; i < WUT_DIC_MAX_NUMBER; i++) {
                if(strlen(dic->entries[0][i]) > 0) {
                        printf("%s: %s\n", 
                                        dic->entries[0][i],
                                        dic->entries[1][i]);
                }
        }
}
