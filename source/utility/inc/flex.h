#ifndef _WUT_UTILITY_FLEX_H
#define _WUT_UTILITY_FLEX_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/component/inc/list.h"


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
 * 	0x11    const	     12	  Constant value
 * 	0x12	PIXEL	    9px	  Pixel value, works like const
 * 	0x13	PERCENT	  10pct	  Pct. used to scale with size value
 * 	0x14	EM	    4em	  Faktor multp. with text size
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
 * Copy the content of the given flex struct to the destination struct without
 * modifying the source struct. This will however overwrite the original content
 * of the destination struct if there is any. If there is no destination, then
 * set parameter to NULL and a new one will be created.
 *
 * @dst: The flex struct to copy into
 * @src: The flex struct to copy from
 *
 * Returns: The modified destination struct
 */
WUT_XMOD struct wut_Flex *wut_flx_duplicate(struct wut_Flex *dst, 
                struct wut_Flex *src);


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
