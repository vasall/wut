#ifndef _WUT_STYLE_CLASS_H
#define _WUT_STYLE_CLASS_H

struct wut_Class;
struct wut_ClassTable;

#include "core/inc/define.h"

#include "utility/inc/list.h"

#include "style/inc/stylesheet.h"


#define WUT_CLASS_NAME_LIMIT    128
#define WUT_CLASS_LIMIT         16

struct wut_Class {
        char                    name[WUT_CLASS_NAME_LIMIT];
        u64                     mask;

        struct wut_List         *attributes;
};

struct wut_ClassTable {
        s16                     number;
        struct wut_Class        *list[WUT_CLASS_LIMIT]; 
};

struct wut_ClassSheet {
        u64                     mask;
        s16                     number;

        char                    names[8][64];
        struct wut_Class        *links[8];
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Create a new class and initialize the contained attribute list.
 *
 * @name: The name of the new class
 *
 * Returns: Either a pointer to the new class or NULL if an error occurred
 */
WUT_XMOD struct wut_Class *wut_cls_create(char *name);


/*
 * Destroy a class, destroy the contained list and free the allocated memory.
 *
 * @cls: Pointer to the class
 */
WUT_XMOD void wut_cls_destroy(struct wut_Class *cls);


/*
 * Push a new attribute to the class.
 *
 * @cls: Pointer to the class
 * @ent: Pointer to the attribute
 */
WUT_XMOD void wut_cls_push_attr(struct wut_Class *cls,
                struct wut_SheetEntry *ent);


/*
 * Push a new class into the class table.
 *
 * @tbl: Pointer to the class table
 * @cls: Pointer to the class
 */
WUT_XMOD void wut_cls_push_table(struct wut_ClassTable *tbl,
                struct wut_Class *cls);


/*
 * Create a new class table.
 *
 * Returns: Either a pointer to the new class table or NULL if an error occurred 
 */
WUT_XMOD struct wut_ClassTable *wut_cls_create_table(void);


/*
 * Remove and destroy all classes contained in the table and then destroy the
 * table itself.
 *
 * @tbl: Pointer to the class table
 */
WUT_XMOD void wut_cls_destroy_table(struct wut_ClassTable *tbl);


/*
 * Retrieve a class from the class table via the name.
 *
 * @tbl: Pointer to the class table
 * @name: The name of the class
 *
 * Returns: Either a pointer to the class or NULL if an error occurred
 */
WUT_XMOD struct wut_Class *wut_cls_get(struct wut_ClassTable *tbl, char *name);


/*
 * Write the attributes to the stylesheet.
 *
 * @cls: Pointer to the class
 * @sht: Pointer to the stylesheet
 */
WUT_XMOD void wut_cls_apply(struct wut_Class *cls, struct wut_Stylesheet *sht);


/*
 * Reset the reference sheet for the classes.
 *
 * @ref: Pointer to the reference sheet
 */
WUT_XMOD void wut_cls_reset_references(struct wut_ClassSheet *ref);


/*
 * Add class names to a reference sheet. This will not aquire the pointers to
 * the classes.
 *
 * @ref: Pointer to the reference sheet
 * @name: A string containing the class names seperated by a space
 */
WUT_XMOD void wut_cls_add_names(struct wut_ClassSheet *ref, char *names);


/*
 * Get pointers to all the classes by getting the classes from the class table.
 *
 * @tbl: pointer to the class table
 * @ref: Pointer to the reference sheet
 */
WUT_XMOD void wut_cls_link(struct wut_ClassTable *tbl,
                struct wut_ClassSheet *ref);


/*
 * Apply the classes and write them to the stylesheet.
 *
 * @ref: Pointer to the reference sheet
 * @sht: Pointer to the stylesheet
 */
WUT_XMOD void wut_cls_apply_references(struct wut_ClassSheet *ref,
                struct wut_Stylesheet *sht);


/*
 * Find an attribute in a class via the reference sheet.
 *
 * @ref: Pointer to the reference sheet
 * @id: The attribute id to look for
 * @ret: The return struct
 *
 * Returns: 1 if the attribute was found, 0 if not and -1 if an error occurred
 */
WUT_XMOD s8 wut_cls_find(struct wut_ClassSheet *ref,
                enum wut_eSheetAttribId id, struct wut_SheetEntry *ret);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Show all attributes contained in a class.
 *
 * @cls: Pointer to the class
 */
WUT_API void wut_PrintClass(struct wut_Class *cls);


/*
 * Show all classes with their attributes.
 *
 * @tbl: Pointer to the class table
 */
WUT_API void wut_PrintClasses(struct wut_ClassTable *tbl);

#endif /* _WUT_STYLE_CLASS_H */
