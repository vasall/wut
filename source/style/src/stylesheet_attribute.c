#include "style/inc/stylesheet_attribute.h"

#include "style/inc/stylesheet_tables.h"

#include "utility/inc/alarm.h"
#include "utility/inc/text_formatting.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


WUT_INTERN u8 sat_hash(char *in)
{
        u64 hash = 5381;
        s32 c;

        while ((c = *in++))
                hash = ((hash << 5) + hash) + c; /* hash *+ c */

        return hash % 0xff;
}


WUT_INTERN u8 sat_category(enum wut_eSheetAttribId id)
{
        return wut_c_sheet_attribs[id].category;
}


WUT_INTERN s8 sat_parse_flex(struct wut_SheetEntry *ent, char *val)
{
        struct wut_Flex *flx;

        flx = wut_flx_parse(WUT_FLX_UNDEF, val);

        ent->type = WUT_SHEET_FLEX;
        ent->value.flex.pointer = flx;
        return 0;
}


WUT_INTERN u8 sat_fromhex(char c)
{
        /* Small letters */
        if(c >= 0x61)
                return (c - 0x61) + 10;

        /* Big letters */
        if(c >= 0x41)
                return (c - 0x41) + 10;

        /* Numbers */
        return c - 0x30;	
}


WUT_INTERN s8 sat_convhex(char *val, u32 *out)
{
        s32 i;
        char *run;
        char c;
        char *head = NULL;

        u32 buf = 0x000000FF;

        run = val;

        /* Find the beginning of the hex */
        while((c = *run)) {
                if(wut_tfm_is_hexcode(c)) {
                        head = run;
                        break;
                }
                run++;
        }
        if(!head)
                return -1;

        for(i = 2; i >= 0; i--) {
                if(!wut_tfm_is_hexcode(head[i*2])) {
                        return 0;
                }

                if(!wut_tfm_is_hexcode(head[i*2+1])) {
                        return -1;
                }

                ((u8 *)&buf)[3 - i] = sat_fromhex(head[i*2]) * 16;
                ((u8 *)&buf)[3 - i] += sat_fromhex(head[i*2+1]);
        }

        *out = buf;

        return 0;
}


WUT_INTERN s8 sat_parse_hexcode(struct wut_SheetEntry *ent, char *val)
{
        u32 hex;

        sat_convhex(val, &hex);

        ent->type = WUT_SHEET_HEXCODE;
        ent->value.hexcode.code = hex;
        return 0; 
}


WUT_INTERN char *sat_next_keyword(char *s, char *buf)
{
        char *run = s;
        char *head = buf;

        /* If everything has been read, return NULL */
        if(*run == 0x00)
                return NULL;

        /* Remove leading spaces */
        while(wut_tfm_is_space(*run))
                run++;

        /* Read the text */
        while(wut_tfm_is_letter(*run)) {
                *(head++) = *(run++);
        }
        *head = 0;

        return run;
}


WUT_INTERN u8 sat_find_keyword(u8 ctg, char *val)
{
        s8 i;

        for(i = 0; i < wut_c_sheet_keywords[ctg].number; i++) {
                if(!strcmp(wut_c_sheet_keywords[ctg].entries[i].string, val)) {
                        return wut_c_sheet_keywords[ctg].entries[i].value;
                }
        }

        return WUT_KW_UNDEFINED;	
}


WUT_INTERN s8 sat_parse_keyword(struct wut_SheetEntry *ent, char *val)
{
        u8 kw = WUT_KW_UNDEFINED;
        u8 ctg;

        char *ptr;
        char buf[127];

        ctg = sat_category(ent->id);

        /* First process the input and set the according keyword flags */
        ptr = val;
        while((ptr = sat_next_keyword(ptr, buf))) {
                kw |= sat_find_keyword(ctg, buf);
        }

        ent->type = WUT_SHEET_KEYWORD;
        ent->value.keyword.code = kw;
        return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_XMOD void wut_sat_reset(struct wut_SheetEntry *ent)
{
        if(!ent)
                return;

        ent->type = WUT_SHEET_UNDEF;
}


WUT_XMOD void wut_sat_cleanup(struct wut_SheetEntry *ent)
{
        if(!ent)
                return;

        if(ent->type == WUT_SHEET_FLEX)
                wut_flx_destroy(ent->value.flex.pointer);
        
        wut_sat_reset(ent);
}


WUT_XMOD s8 wut_sat_parse(char *attr, char *val, struct wut_SheetEntry *out)
{
        if(!attr || !val || !out) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return -1;
        }

        /* First get the Id for the attribute */
        if((out->id = wut_sat_get_id(attr)) == WUT_SHEET_UNDEFINED) {
                char tmp[512];
                sprintf(tmp, "Attribute name \"%s\" unknown", attr);
                WUT_ALARM(WUT_WARNING, tmp);
                return -1;
        }

        /* Then parse the data depending on the type */
        switch(wut_sat_typeof(out->id)) {
                case WUT_SHEET_FLEX:
                        return sat_parse_flex(out, val);
                case WUT_SHEET_HEXCODE:
                        return sat_parse_hexcode(out, val);
                case WUT_SHEET_KEYWORD:
                        return sat_parse_keyword(out, val);
        }

        return -1;
}


WUT_XMOD enum wut_eSheetAttribId wut_sat_get_id(char *s)
{
        u8 hash;
        u8 row;
        u8 i;

        hash = sat_hash(s);
        row = hash %  WUT_SHEET_ROWS;

        for(i = 0; i < wut_c_sheet_ids[row].number; i++) {
                if(wut_c_sheet_ids[row].entries[i].hash == hash) {
                        return wut_c_sheet_ids[row].entries[i].id;
                }
        }

        return WUT_SHEET_UNDEFINED;
}


WUT_XMOD u8 wut_sat_typeof(enum wut_eSheetAttribId id)
{
        return wut_c_sheet_attribs[id].type;
}


WUT_XMOD void wut_sat_print(struct wut_SheetEntry *ent)
{
        printf("%20s: ", wut_sheet_attr_name[ent->id]);

        switch(ent->type) {
                case WUT_SHEET_FLEX:
                        wut_flx_print(ent->value.flex.pointer);
                        break;

                case WUT_SHEET_HEXCODE:
                        printf("%08x", ent->value.hexcode.code);
                        break;

                case WUT_SHEET_KEYWORD:
                        printf("%02x", ent->value.keyword.code);
                        break;

                default:
                        printf("wrongtype");
        }

        printf("\n");
}
