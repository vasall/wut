#ifndef _FH_STYLE_UTILS_H
#define _FH_STYLE_UTILS_H

#include "stdinc.h"

#include "stylesheet.h"
#include "style.h"

struct fh_style_mod {
	u8 *buf;
	s16 len;
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
 * @index: The index of the attribute
 *
 * Returns: Either a pointer to the entry in the attribute table or NULL if an
 * 	    error occurred
 */
FH_XMOD const struct fh_stylesheet_attribute *fh_style_utl_get(s8 index);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Compress style modifications into a simple change buffer. This buffer can
 * then be easily applied to any stylesheet. Note that after usage the returned
 * buffer needs to be cleared manually.
 *
 * @str: The modifications in string format
 *
 * Returns: A style-modification-struct or NULL if an error occurred
 */
FH_API struct fh_style_mod *fh_CompressStyleMod(char *str);


/*
 * Free a given style-modification-struct and free up the allocated memory.
 *
 * @mod: Pointer to the mod to free
 */
FH_API void fh_FreeStyleMod(struct fh_style_mod *mod);


/*
 * Apply a style-modification onto a style struct.
 *
 * @style: Pointer to the style struct to modify
 * @mod: Pointer to the modifications to apply
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ApplyStyleMod(struct fh_style *style, struct fh_style_mod *mod);


#endif /* _FH_STYLE_UTILS_H */
