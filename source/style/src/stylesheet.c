#include "style/inc/stylesheet.h"

#include "style/inc/stylesheet_tables.h"

#include "system/inc/system.h"

#include "utility/inc/alarm.h"

#include <stdlib.h>


WUT_INTERN void sht_sanitize(char *s_in, char *s_out)
{
        u8 i;

        for(i = 0; i < strlen(s_in); i++) {
                s_out[i] = s_in[i];

                if(s_in[i] >= 0x41 && s_in[i] <= 0x5A) {
                        s_out[i] += 0x20;
                }
        }
        s_out[i] = 0;
}

WUT_INTERN s8 sht_isspace(char c)
{
        return c == 0x20;
}


WUT_INTERN s8 sht_isletter(char c)
{
        if(c >= 0x61 && c <= 0x7A)
                return 1;

        return 0;
}


WUT_INTERN s8 sht_ishex(char c)
{
        /* Numbers */
        if(c >= 0x30 && c <= 0x39)
                return 1;

        /* Letters */
        return sht_isletter(c);
}


WUT_INTERN s16 sht_offset(enum wut_eSheetAttribId id)
{
        return wut_c_sheet_attribs[id].offset;
}


WUT_INTERN s8 sht_sizeof(enum wut_eSheetAttribId id)
{
        switch(wut_c_sheet_attribs[id].type) {
                case WUT_SHEET_FLEX:	return sizeof(struct wut_Flex *);
                case WUT_SHEET_HEXCODE:	return U32_S;
                case WUT_SHEET_KEYWORD:	return U8_S;
        }
        return 0;
}


WUT_INTERN u8 sht_typeof(enum wut_eSheetAttribId id)
{
        return wut_c_sheet_attribs[id].type;
}


WUT_INTERN void sht_read(struct wut_Stylesheet *sheet, enum wut_eSheetAttribId id,
                void *ptr)
{
        memcpy(ptr, ((u8 *)sheet) + sht_offset(id), sht_sizeof(id));
}


WUT_INTERN void sht_write(struct wut_Stylesheet *sheet, enum wut_eSheetAttribId id,
                void *ptr)
{		
        memcpy(((u8 *)sheet) + sht_offset(id), ptr, sht_sizeof(id));
}


WUT_INTERN u8 sht_category(enum wut_eSheetAttribId id)
{
        return wut_c_sheet_attribs[id].category;
}


/*
 * This function will get the next key-value-pair from the input and extract
 * both of them. After that it will set the pointer to the end of the read part,
 * so it can be called again to extract the next key-value-pair.
 *
 * @s: The input string
 * @attr: A pointer to write the attribute string to
 * @val: A pointer to write the value string to
 *
 * Returns: The updated pointer or NULL if an error occurred
 */
WUT_INTERN char *sht_next(char *s, char *attr, char *val)
{
        char *run;
        char c;
        char *ptr;

        run = s;

        /* Extract the attribute string */
        ptr = attr;
        while((c = *(run++)) != ':') {
                if(c == 0)
                        return NULL;

                /* Skip */
                if(c == '\n' || sht_isspace(c))
                        continue;

                *(ptr++) = c;
        }
        *(ptr) = 0;

        /* Extract the value string */
        ptr = val;
        while((c = *(run++)) != ';') {
                if(c == 0)
                        return NULL;

                /* Skip */
                if(c == '\n')
                        continue;

                *(ptr++) = c;
        }
        *(ptr) = 0;

        return run;
}

WUT_INTERN u8 sht_hash(char *in)
{
        u64 hash = 5381;
        s32 c;

        while ((c = *in++))
                hash = ((hash << 5) + hash) + c; /* hash *+ c */

        return hash % 0xff;
}


WUT_INTERN enum wut_eSheetAttribId sht_get_id(char *s)
{
        u8 hash;
        u8 row;
        u8 i;

        hash = sht_hash(s);
        row = hash %  WUT_SHEET_ROWS;

        for(i = 0; i < wut_c_sheet_ids[row].number; i++) {
                if(wut_c_sheet_ids[row].entries[i].hash == hash) {
                        return wut_c_sheet_ids[row].entries[i].id;
                }
        }

        return WUT_SHEET_UNDEFINED;
}


WUT_INTERN void sht_parse_flex(struct wut_Stylesheet *sheet,
                enum wut_eSheetAttribId id, char *val)
{
        struct wut_Flex *flx = WUT_FLX_UNDEF;

        sht_read(sheet, id, &flx);

        flx = wut_flx_parse(flx, val);

        sht_write(sheet, id, &flx);
}


WUT_INTERN u8 sht_fromhex(char c)
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


WUT_INTERN s8 sht_convhex(char *val, u32 *out)
{
        s32 i;
        char *run;
        char c;
        char *head = NULL;

        u32 buf = 0x000000FF;

        run = val;

        /* Find the beginning of the hex */
        while((c = *run)) {
                if(sht_ishex(c)) {
                        head = run;
                        break;
                }
                run++;
        }
        if(!head)
                return -1;

        for(i = 2; i >= 0; i--) {
                if(!sht_ishex(head[i*2])) {
                        return 0;
                }

                if(!sht_ishex(head[i*2+1])) {
                        return -1;
                }

                ((u8 *)&buf)[3 - i] = sht_fromhex(head[i*2]) * 16;
                ((u8 *)&buf)[3 - i] += sht_fromhex(head[i*2+1]);
        }

        *out = buf;

        return 0;
}


WUT_INTERN void sht_parse_hexcode(struct wut_Stylesheet *sheet,
                enum wut_eSheetAttribId id, char *val)
{
        u32 hex;

        sht_convhex(val, &hex);

        sht_write(sheet, id, &hex);
}


WUT_INTERN char *sht_next_keyword(char *s, char *buf)
{
        char *run = s;
        char *head = buf;

        /* If everything has been read, return NULL */
        if(*run == 0x00)
                return NULL;

        /* Remove leading spaces */
        while(sht_isspace(*run))
                run++;

        /* Read the text */
        while(sht_isletter(*run)) {
                *(head++) = *(run++);
        }
        *head = 0;

        return run;
}


WUT_INTERN u8 sht_find_keyword(u8 ctg, char *val)
{
        s8 i;

        for(i = 0; i < wut_c_sheet_keywords[ctg].number; i++) {
                if(!strcmp(wut_c_sheet_keywords[ctg].entries[i].string, val)) {
                        return wut_c_sheet_keywords[ctg].entries[i].value;
                }
        }

        return WUT_KW_UNDEFINED;	
}


WUT_INTERN void sht_parse_keyword(struct wut_Stylesheet *sheet,
                enum wut_eSheetAttribId id, char *val)
{
        u8 kw = WUT_KW_UNDEFINED;
        u8 ctg;

        char *ptr;
        char buf[127];

        ctg = sht_category(id);

        /* First process the input and set the according keyword flags */
        ptr = val;
        while((ptr = sht_next_keyword(ptr, buf))) {
                kw |= sht_find_keyword(ctg, buf);
        }

        sht_write(sheet, id, &kw);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */



WUT_XMOD void wut_sht_reset(struct wut_Stylesheet *sheet)
{
        if(!sheet) return;

        sheet->mask = 0;

        sheet->display_mode = 0;

        sheet->width = WUT_FLX_UNDEF;
        sheet->height = WUT_FLX_UNDEF;

        sheet->reference_mode = 0;

        sheet->spacing_top = WUT_FLX_UNDEF;
        sheet->spacing_right = WUT_FLX_UNDEF;
        sheet->spacing_bottom = WUT_FLX_UNDEF;
        sheet->spacing_left = WUT_FLX_UNDEF;

        sheet->padding_top = WUT_FLX_UNDEF;
        sheet->padding_right = WUT_FLX_UNDEF;
        sheet->padding_bottom = WUT_FLX_UNDEF;
        sheet->padding_left = WUT_FLX_UNDEF;

        sheet->border_mode = 0;
        sheet->border_width = WUT_FLX_UNDEF;
        sheet->border_color = wut_SetColor(0, 0, 0, 1);

        sheet->radius_top_left = WUT_FLX_UNDEF;
        sheet->radius_top_right = WUT_FLX_UNDEF;
        sheet->radius_bottom_right = WUT_FLX_UNDEF;
        sheet->radius_bottom_left = WUT_FLX_UNDEF;

        sheet->infill_mode = 0;
        sheet->infill_color = wut_SetColor(0, 0, 0, 1);

        sheet->layout_mode = 0;

        sheet->align_v = 0;
        sheet->align_h = 0;

        sheet->scrollbar_mode = 0;

        sheet->text_size = WUT_FLX_UNDEF;
        sheet->text_color = wut_SetColor(0, 0, 0, 1);
        sheet->text_mass = WUT_FLX_UNDEF;
        sheet->text_options = 0;
        sheet->text_wrap_mode = WUT_KW_TEXT_WORDWRAP;
        sheet->text_spacing = WUT_FLX_UNDEF;
        sheet->line_height = WUT_FLX_UNDEF;
}


WUT_XMOD void wut_sht_parse(struct wut_Stylesheet *sheet, char *s)
{
        char *run;
        char inp_s[106];
        char attr_s[64];
        char val_s[64];

        enum wut_eSheetAttribId id;

        if(!sheet || !s) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return;
        }

        /*
         * Sanitize the input string to make it conform with input requirements.
         * This will for example convert letters from big(A..Z) to small(a..z).
         */
        sht_sanitize(s, inp_s);

        run = inp_s;
        while((run = sht_next(run, attr_s, val_s))) {
                if((id = sht_get_id(attr_s)) == WUT_SHEET_UNDEFINED)
                        continue;

                switch(sht_typeof(id)) {
                        case WUT_SHEET_FLEX:
                                sht_parse_flex(sheet, id, val_s);
                                break;
                        case WUT_SHEET_HEXCODE:
                                sht_parse_hexcode(sheet, id, val_s);
                                break;
                        case WUT_SHEET_KEYWORD:
                                sht_parse_keyword(sheet, id, val_s);
                                break;
                }

                /* Don't forget to add the flag to the mask */
                sheet->mask |= (1<<id);
        }
}



WUT_XMOD s8 wut_sht_get(struct wut_Stylesheet *sheet, enum wut_eSheetAttribId id,
                struct wut_SheetReturn *ret)
{
        if(!sheet || !ret)
                return -1;

        if(!(sheet->mask & (1<<id))) {
                return 0;
        }

        switch(sht_typeof(id)) {
                case WUT_SHEET_FLEX:
                        sht_read(sheet, id, &ret->flex);
                        ret->type = WUT_SHEET_FLEX;
                        break;
                case WUT_SHEET_HEXCODE:
                        sht_read(sheet, id, &ret->hexcode);
                        ret->type = WUT_SHEET_HEXCODE;
                        break;
                case WUT_SHEET_KEYWORD:
                        sht_read(sheet, id, &ret->keyword);
                        ret->type = WUT_SHEET_KEYWORD;
                        break;
        }

        return 1;
}


WUT_XMOD void wut_sht_print(struct wut_Stylesheet *sheet)
{
        if(!sheet)
                return;

        printf("Mask: %016lx", sheet->mask);

        printf("\ndisplay_mode: %02x", sheet->display_mode);


        printf("\nwidth: "); wut_flx_print(sheet->width);
        printf("\nheight: "); wut_flx_print(sheet->height);

        printf("\nspacing_top: "); wut_flx_print(sheet->spacing_top);
        printf("\nspacing_right: "); wut_flx_print(sheet->spacing_right);
        printf("\nspacing_bottom: "); wut_flx_print(sheet->spacing_bottom);
        printf("\nspacing_left: "); wut_flx_print(sheet->spacing_left);

        printf("\npadding_top: "); wut_flx_print(sheet->padding_top);
        printf("\npadding_right: "); wut_flx_print(sheet->padding_right);
        printf("\npadding_bottom: "); wut_flx_print(sheet->padding_bottom);
        printf("\npadding_left: "); wut_flx_print(sheet->padding_left);

        printf("\nborder_mode: %02x", sheet->border_mode);
        printf("\nborder_width: "); wut_flx_print(sheet->border_width);
        printf("\nborder_color: %08x", wut_GetColorU32(sheet->border_color));

        printf("\nradius_top_left: ");
        wut_flx_print(sheet->radius_top_left);
        printf("\nradius_top_right: ");
        wut_flx_print(sheet->radius_top_right);
        printf("\nradius_bottom_right: ");
        wut_flx_print(sheet->radius_bottom_right);
        printf("\nradius_bottom_left: ");
        wut_flx_print(sheet->radius_bottom_left);

        printf("\ninfill_mode: %02x", sheet->infill_mode);
        printf("\ninfill_color: %08x", wut_GetColorU32(sheet->infill_color));

        printf("\nlayout_mode: %02x", sheet->layout_mode);

        printf("\nalign_v: %02x", sheet->align_v);
        printf("\nalign_h: %02x", sheet->align_h);

        printf("\nscrollbar_mode: %02x", sheet->scrollbar_mode);

        printf("\ntext_size: "); wut_flx_print(sheet->text_size);
        printf("\ntext_color: %08x", wut_GetColorU32(sheet->text_color));
        printf("\ntext_mass: "); wut_flx_print(sheet->text_mass);
        printf("\ntext_options: %02x", sheet->text_options);
        printf("\ntext_wrap_mode: %02x", sheet->text_wrap_mode);
}
