#ifndef _WUT_UTILITY_FLEX_H
#define _WUT_UTILITY_FLEX_H

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
struct wut_FlexToken {
	u8 	code;
	f32 	value;
};


struct wut_Flex {
	struct wut_List *list;
};


#define WUT_FLX_UNDEF	NULL


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
WUT_XMOD struct wut_Flex *wut_flx_parse(struct wut_Flex *flx, char *inp);


/*
 * Destroy a flex expression and free the allocated memory.
 *
 * @flx: Pointer to the flex struct
 */
WUT_XMOD void wut_flx_destroy(struct wut_Flex *flx);


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
WUT_XMOD s32 wut_flx_process(struct wut_Flex *flx, u16 *ref);


/*
 * Print the flex in the console without newline.
 *
 * @flx: Pointer to the flex struct
 */
WUT_XMOD void wut_flx_print(struct wut_Flex *flx);


#endif /* _WUT_UTILITY_FLEX_H */
