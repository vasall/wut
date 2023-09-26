#ifndef _FH_STYLE_UTILS_H
#define _FH_STYLE_UTILS_H

#include "stdinc.h"
#include "stylesheet.h"


struct fh_style_mod {
	u8 *buf;
	u32 len;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Get the index of an attribute
 *
 * @attrib_name: The name of the attribute
 *
 * Returns: Either the index or -1 if an error occurred
 */
FH_XMOD s8 fh_style_utl_find(char *attrib_name);


/*
 * Get an attribute via the attribute index.
 *
 * @attr: A pointer to write the attribute to
 * @index: The index of the attribute
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_XMOD s8 fh_style_utl_get(struct fh_stylesheet_attribute *attr, s8 index);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Compress style modifications into a simple change buffer. This buffer can
 * then be easily applied to any stylesheet.
 *
 * @str: The modifications in string format
 * @out: A pointer to attach the change buffer to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_CompressStyleMod(char *str, char **out);




#endif /* _FH_STYLE_UTILS_H */
