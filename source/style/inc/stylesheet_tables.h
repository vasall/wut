#ifndef _FH_STYLE_STYLESHEET_TABLES_H
#define _FH_STYLE_STYLESHEET_TABLES_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "style/inc/stylesheet.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

#define FH_SHEET_ATTRIBS	33

FH_XMOD const struct fh_sheet_attr fh_c_sheet_attribs[FH_SHEET_ATTRIBS];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_sheet_id_entry {
	u8 				hash;
	enum fh_sheet_id 		id;
};


struct fh_sheet_id_table {
	struct fh_sheet_id_entry 	*entries;
	u8 				number;
};

#define FH_SHEET_ROWS		8

FH_XMOD const struct fh_sheet_id_table fh_c_sheet_ids[FH_SHEET_ROWS];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_sheet_kw {
	char 	*string;
	u8 	value;
};

struct fh_sheet_kw_table {
	struct fh_sheet_kw 	*entries;
	u8 			number;
};

#define FH_SHEET_KW_ROWS	10

FH_XMOD const struct fh_sheet_kw_table fh_c_sheet_keywords[FH_SHEET_KW_ROWS];

#endif /* _FH_STYLE_STYLESHEET_TABLES_H */
