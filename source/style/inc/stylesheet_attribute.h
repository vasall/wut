#ifndef _WUT_STYLE_STYLESHEET_ATTRIBUTE_H
#define _WUT_STYLE_STYLESHEET_ATTRIBUTE_H

#include "core/inc/define.h"

#include "style/inc/stylesheet_tables.h"

#include "utility/inc/flex.h"

/*
 * An entry containing data used when adding, retrieving and updating data in
 * the table.
 */
struct wut_SheetEntry {
        enum wut_eSheetAttribId 	id;

        enum wut_eSheetDatatype 	type;
        
        union {
                struct {
                        struct wut_Flex         *pointer;
                } flex;

                struct {
                        u32                     code;
                        u32 _pad;
                } hexcode;

                struct {
                        u8                      code;
                        u8 _pad[7];
                } keyword;
        } value;
};


/*
 * Reset the attributes of the entry. This will not free allocated memory but
 * just zero everything. This function should be used before parsing.
 *
 * @ent: Pointer to the entry to reset
 */
WUT_XMOD void wut_sat_reset(struct wut_SheetEntry *ent);


/*
 * This function will cleanup and free the allocated memory in the entry. This
 * function should be used after parsing to prevent possible memory leaks.
 *
 * @ent: Pointer to the entry
 */
WUT_XMOD void wut_sat_cleanup(struct wut_SheetEntry *ent);


/*
 * Parse the given attribute name and value string and convert that to a sheet
 * entry, which can be used when working with the stylesheet.
 *
 * @attr: A string containing the attribute name
 * @val: A string containing the attribute value
 * @out: A pointer to write the resulting entry to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_XMOD s8 wut_sat_parse(char *attr, char *val, struct wut_SheetEntry *out);


/*
 * Get the attribute id from a given attribute name.
 *
 * @s: A string containing the attribute name
 *
 * Returns: The attribute id
 */
WUT_XMOD enum wut_eSheetAttribId wut_sat_get_id(char *s);


/*
 * Get the datatype of an attribute.
 *
 * @id: The attribute id
 *
 * Returns: The datatype for the attribute
 */
WUT_XMOD u8 wut_sat_typeof(enum wut_eSheetAttribId id);

/*
 * Print a style-attribute in the console.
 *
 * @ent: Pointer to the style attribute
 */
WUT_XMOD void wut_sat_print(struct wut_SheetEntry *ent);

#endif /* _WUT_STYLE_STYLESHEET_ATTRIBUTE_H */
