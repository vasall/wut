#include "style/inc/style_utils.h"

#include "style/inc/stylesheet_tables.h"

#include "system/inc/system.h"

#include "parser/inc/parser.h"

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


FH_XMOD const struct fh_stylesheet_attribute *fh_style_utl_get(s8 index)
{
	if(index < 0 || index >= FH_STYLESHEET_ATTRIB_LIM)
		return NULL;

	return &fh_stylesheet_attributes[index];
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_style_mod *fh_CompressStyleMod(char *str)
{
	char *run;
	char attr[256];
	char val[256];
	s8 idx;
	const struct fh_stylesheet_attribute *ent;

	s16 req_len;

	u8 *cur;
	s8 s;

	struct fh_style_mod *mod;

	if(!str) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * First determine the necessary buffer-length by iterating through the
	 * given input and counting the element plus their individual required
	 * size.
	 */
	run = str;
	req_len = 0;
	while((run = fh_parser_attribute(run, attr, val))) {
		if((idx = fh_style_utl_find(attr)) < 0)
			continue;

		ent = fh_style_utl_get(idx);
		req_len += ent->size + 1;
	}

	/*
	 * Allocate memory for the modification-struct.
	 */
	if(!(mod = fh_malloc(sizeof(struct fh_style_mod)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for style_mod");
		goto err_return;
	}


	/*
	 * Now allocate the buffer.
	 */
	if(!(mod->buf = fh_malloc(req_len))) {
		ALARM(ALARM_ERR, "Failed to allocate memory");
		goto err_free_mod;
	}

	/*
	 * Now go through the list again and convert everything.
	 */
	run = str;
	cur = mod->buf;
	while((str = fh_parser_attribute(run, attr, val))) {
		/*
		 * First extract the attribute and retrieve the corresponding
		 * code. Then write this code to the buffer.
		 */
		if((idx = fh_style_utl_find(attr)) < 0)
			continue;

		ent = fh_style_utl_get(idx);

		*cur = (u8)idx;
		cur++;

		/*
		 * Second extract the attribute value and convert it into the
		 * according compact format. Then also write it to the buffer.
		 */
		if((s = fh_parser_value(val, ent->input, ent->category, cur)) < 0)
			continue;

		cur += s;	
	}

	mod->len = req_len;


	return mod;

err_free_mod:
	fh_free(mod);

err_return:
	ALARM(ALARM_ERR, "Failed to compress style modifications");
	return NULL;
}


FH_API void fh_FreeStyleMod(struct fh_style_mod *mod)
{
	if(!mod)
		return;

	fh_free(mod->buf);
	fh_free(mod);
}


FH_API s8 fh_ApplyStyleMod(struct fh_style *style, struct fh_style_mod *mod)
{
	u8 *run;
	s16 c;

	s8 idx;
	const struct fh_stylesheet_attribute *ent;

	if(!style || !mod) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	run = mod->buf;
	c = 0;
	while(c < mod->len) {
		/* First get the attribute index */
		idx = *(s8 *)run;

		/* Get the style attribute */
		ent = fh_style_utl_get(idx);

		/* Write the data to the stylesheet */
		memcpy(((u8 *)&style->sheet) + ent->offset, run, ent->size);
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to apply mods to style");
	return -1;
}
