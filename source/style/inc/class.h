#ifndef _WUT_STYLE_CLASS_H
#define _WUT_STYLE_CLASS_H

#include "core/inc/define.h"

#include "utility/inc/list.h"

#include "document/inc/document.h"

#define WUT_CLASS_NAME_LIMIT    128
#define WUT_CLASS_LIMIT         16

struct wut_Class {
        char                    name[WUT_CLASS_NAME_LIMIT];

        struct wut_List         *attributes;
};

struct wut_ClassTable {
        s16                     number;
        struct wut_Class        *list[WUT_CLASS_LIMIT]; 
};



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

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
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Load style classes from a file and add them to the class table of a document.
 *
 * @doc: Pointer to the document
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_LoadClasses(struct wut_Document *doc, char *pth);


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
