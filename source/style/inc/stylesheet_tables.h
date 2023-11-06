#ifndef _FH_STYLESHEET_TABLES_H
#define _FH_STYLESHEET_TABLES_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "style/inc/stylesheet.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

#define FH_STYLESHEET_ATTRIB_LIM	27

FH_XMOD const struct fh_stylesheet_attribute fh_stylesheet_attributes[27];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_stylesheet_hme {
	u8 hash;
	u8 index;
};


struct fh_stylesheet_hm {
	struct fh_stylesheet_hme *entries;
	u8 number;
};

#define FH_STYLESHEET_HASH_ROWS		8

FH_XMOD const struct fh_stylesheet_hm fh_c_stylesheet_hm[8];


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

struct fh_stylesheet_kve {
	char *string;
	u8 value;
};

struct fh_stylesheet_kv {
	struct fh_stylesheet_kve *entries;
	u8 number;
};

#define FH_STYLESHEET_OPT_ROWS	8

FH_XMOD const struct fh_stylesheet_kv fh_c_stylesheet_kv[8];

#endif /* _FH_STYLESHEET_TABLES_H */
