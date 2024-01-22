#ifndef _FH_UTILITY_FLEX_H
#define _FH_UTILITY_FLEX_H

#include "core/inc/define.h"
#include "core/inc/import.h"


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
 * 	0x07    const
 * 	0x08	PIXEL
 * 	0x09	PERCENT
 * 	0x0a	EM
 *
 * ----------------------
 */
struct fh_flex_token {
	u8 code;
	f32 value;
};


struct fh_flex {
	struct fh_list *list;
};


typedef struct fh_flex *fh_flex_t;


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
FH_XMOD fh_flex_t fh_flex_parse(fh_flex_t flx, char *inp);


/*
 * Destroy a flex expression and free the allocated memory.
 *
 * @flx: Pointer to the flex struct
 */
FH_XMOD void fh_flex_destroy(fh_flex_t flx);


/*
 * Process the flex expression with the given reference values.
 *
 * @flx: Pointer to the flex struct
 * @ref_pct: The reference value used for percentage calculations
 * @ref_txt: The reference text size used for em
 *
 * Returns: The result of the expression and 0 if an error occurred
 */
FH_XMOD s32 fh_flex_process(fh_flex_t flx, u16 ref_pct, u16 ref_txt);


/*
 * Stringify the flex expression and return it as a string.
 *
 * @flx: Pointer to the flex expression
 *
 * Returns: The stringified version of the flex expression
 */
FH_XMOD char *fh_flex_stringify(fh_flex_t flx);

#endif /* _FH_UTILITY_FLEX_H */
