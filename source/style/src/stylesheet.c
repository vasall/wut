#include "style/inc/stylesheet.h"

#include "style/inc/stylesheet_tables.h"
#include "style/inc/stylesheet_attribute.h"

#include "system/inc/system.h"

#include "utility/inc/alarm.h"
#include "utility/inc/utility.h"
#include "utility/inc/text_formatting.h"

#include <stdlib.h>




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
                default: return 0;
        }
}


WUT_INTERN void sht_read(struct wut_Stylesheet *sheet,
                enum wut_eSheetAttribId id, struct wut_SheetEntry *out)
{
        s16 off = sht_offset(id);
        s8 size = sht_sizeof(id);

        memcpy(&out->value, (u8 *)sheet + off, size);

        out->id = id;
        out->type = wut_sat_typeof(id);
}


WUT_INTERN void sht_write(struct wut_Stylesheet *sheet,
                struct wut_SheetEntry *ent)
{
        struct wut_SheetEntry swp;
        s16 off = sht_offset(ent->id);
        s8 size = sht_sizeof(ent->id);

        /* Read the current value from the stylesheet */
        sht_read(sheet, ent->id, &swp);

        if(ent->type == WUT_SHEET_FLEX && swp.value.flex.pointer) {
                wut_flx_destroy(swp.value.flex.pointer);
        }

        memcpy((u8 *)sheet + off, &ent->value, size);
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
                if(c == '\n' || wut_tfm_is_space(c))
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


WUT_INTERN s8 sht_cfnc_apply(void *ptr, s16 idx, void *data)
{
        struct wut_SheetEntry *ent = (struct wut_SheetEntry *)ptr;
        struct wut_Stylesheet *sht = (struct wut_Stylesheet *)data;

        WUT_IGNORE(idx);

        sht_write(sht, ent);

        return 0;
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

        struct wut_SheetEntry ent;

        if(!sheet || !s) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return;
        }

        strcpy(inp_s, s);

        /*
         * Sanitize the input string to make it conform with input requirements.
         */
        wut_tfm_to_lowercase(inp_s);

        run = inp_s;
        while((run = sht_next(run, attr_s, val_s))) {
                wut_sat_reset(&ent);

                if(wut_sat_parse(attr_s, val_s, &ent) >= 0) {
                        /* Write the data to the stylesheet */
                        sht_write(sheet, &ent);

                        /* Add the flag to the mask */
                        sheet->mask |= (1<<ent.id);
                }
        }
}


WUT_XMOD s8 wut_sht_set(struct wut_Stylesheet *sheet,
                struct wut_SheetEntry *ent)
{
        if(!sheet || !ent) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return -1;
        }

        sht_write(sheet, ent);
        return 0;
}


WUT_XMOD s8 wut_sht_get(struct wut_Stylesheet *sheet, enum wut_eSheetAttribId id,
                struct wut_SheetEntry *out)
{
        if(!sheet || !out)
                return -1;

        if(!(sheet->mask & (1<<id))) {
                return 0;
        }

        sht_read(sheet, id, out);

        return 1;
}


WUT_XMOD void wut_sht_apply(struct wut_Stylesheet *sht, struct wut_Class *cls)
{ 
        wut_ApplyList(cls->attributes, &sht_cfnc_apply, (void *)sht); 
}


WUT_XMOD void wut_sht_copy(struct wut_Stylesheet *dst,
                struct wut_Stylesheet *src)
{
        memcpy(dst, src, sizeof(struct wut_Stylesheet));
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
