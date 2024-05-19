#ifndef _WT_STYLE_STYLESHEET_TABLES_H
#define _WT_STYLE_STYLESHEET_TABLES_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "style/inc/stylesheet.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

#define WT_SHEET_ATTRIBS	33

WT_XMOD const struct wt_sheet_attr wt_c_sheet_attribs[WT_SHEET_ATTRIBS];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wt_sheet_id_entry {
	u8 				hash;
	enum wt_sheet_id 		id;
};


struct wt_sheet_id_table {
	struct wt_sheet_id_entry 	*entries;
	u8 				number;
};

#define WT_SHEET_ROWS		8

WT_XMOD const struct wt_sheet_id_table wt_c_sheet_ids[WT_SHEET_ROWS];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct wt_sheet_kw {
	char 	*string;
	u8 	value;
};

struct wt_sheet_kw_table {
	struct wt_sheet_kw 	*entries;
	u8 			number;
};

#define WT_SHEET_KW_ROWS	10

WT_XMOD const struct wt_sheet_kw_table wt_c_sheet_keywords[WT_SHEET_KW_ROWS];

#endif /* _WT_STYLE_STYLESHEET_TABLES_H */
