#ifndef _WUT_STYLE_STYLESHEET_TABLES_H
#define _WUT_STYLE_STYLESHEET_TABLES_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "style/inc/stylesheet.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

#define WUT_SHEET_ATTRIBS	33

WUT_XMOD const struct wut_SheetAttrib wut_c_sheet_attribs[WUT_SHEET_ATTRIBS];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wut_SheetAttribIdEntry {
	u8 				hash;
	enum wut_eSheetAttribId 	id;
};


struct wut_SheetAttribIdTable {
	struct wut_SheetAttribIdEntry 	*entries;
	u8 				number;
};

#define WUT_SHEET_ROWS		8

WUT_XMOD const struct wut_SheetAttribIdTable wut_c_sheet_ids[WUT_SHEET_ROWS];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wut_sheet_kw {
	char 	*string;
	u8 	value;
};

struct wut_sheet_kw_table {
	struct wut_sheet_kw 	*entries;
	u8 			number;
};

#define WUT_SHEET_KW_ROWS	10

WUT_XMOD const struct wut_sheet_kw_table wut_c_sheet_keywords[WUT_SHEET_KW_ROWS];

#endif /* _WUT_STYLE_STYLESHEET_TABLES_H */
