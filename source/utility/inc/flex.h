#ifndef _WT_UTILITY_FLEX_H
#define _WT_UTILITY_FLEX_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/list.h"


/*
 * ----  CODE-TABLE  ----
 *
 * OPERATOR
 * 	
 * 	0x01	(	
 * 	0x02	)
 * 	0x03	*
 * 	0x04	+
 * 	0x05	-
 * 	0x06	/
 *
 * OPERAND
 *
 * 	0x11    const		Constant value
 * 	0x12	PIXEL		Pixel value, works like const
 * 	0x13	PERCENT		Pct. used to scale with size value
 * 	0x14	EM		Faktor multp. with text size
 *
 * ----------------------
 */
struct wt_flex_token {
	u8 	code;
	f32 	value;
};


struct wt_flex {
	struct wt_list *list;
};


typedef struct wt_flex *wt_flex_t;

#define WT_FLEX_UNDEF	NULL


/*
 * This function will parse the input and set the flex expression. This will
 * create a new flex expression if necessary or just update it if it already
 * exists. If input is empty, the input is output.
 *
 * @flx: The old flex to update or NULL if one should be created
 * @inp: The input string with the infix expression
 *
 * Returns: The updated or newly created flex expression
 */
WT_XMOD wt_flex_t wt_flex_parse(wt_flex_t flx, char *inp);


/*
 * Destroy a flex expression and free the allocated memory.
 *
 * @flx: Pointer to the flex struct
 */
WT_XMOD void wt_flex_destroy(wt_flex_t flx);


/*
 * Process the flex expression with the given reference values.
 *
 * Note, the reference array has to contain the following values:
 *
 * 	0: pct_ref
 * 	1: text_ref
 *
 *
 * @flx: Pointer to the flex struct
 * @ref: An array with the reference values
 *
 * Returns: The result of the expression and 0 if an error occurred
 */
WT_XMOD s32 wt_flex_process(wt_flex_t flx, u16 *ref);


/*
 * Print the flex in the console without newline.
 *
 * @flx: Pointer to the flex struct
 */
WT_XMOD void wt_flex_print(wt_flex_t flx);


#endif /* _WT_UTILITY_FLEX_H */
