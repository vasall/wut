#include "style_utils.h"

#include "stylesheet_tables.h"

#include <stdlib.h>



FH_INTERN u8 style_utl_hash(char *in)
{
        u64 hash = 5381;
        s32 c;

        while ((c = *in++))
                hash = ((hash << 5) + hash) + c; /* hash *+ c */

        return hash % 0xff;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_XMOD s8 fh_style_utl_find(char *attrib_name)
{
	u8 hash;
	u8 row;
	u8 i;

	if(!attrib_name)
		return -1;

	hash = style_utl_hash(attrib_name);
	row = hash % FH_STYLESHEET_HASH_ROWS;

	for(i = 0; i < fh_c_stylesheet_hm[row].number; i++) {
		if(fh_c_stylesheet_hm[row].entries[i].hash == hash) {
			return fh_c_stylesheet_hm[row].entries[i].index;
		}
	}

	return -1;
}


FH_XMOD s8 fh_style_utl_get(struct fh_stylesheet_attribute *attr, s8 index)
{
	if(!attr || index < 0 || index >= FH_STYLESHEET_ATTRIB_LIM)
		return -1;

	memcpy(attr, &fh_stylesheet_attributes[index],
			sizeof(struct fh_stylesheet_attribute));

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_style_mod fh_CompressStyleMod(char *str, u8 **out)
{
	if(!str || !out) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to compress style modifications");
	return -1;
}
