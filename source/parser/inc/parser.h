#ifndef _FH_PARSER_H
#define _FH_PARSER_H

#include "core/inc/define.h"
#include "core/inc/import.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_XMOD s8 fh_parser_decimal(char *in, u8 ctg, u8 *out);

FH_XMOD s8 fh_parser_percent(char *in, u8 ctg, u8 *out);

FH_XMOD s8 fh_parser_hexcode(char *in, u8 ctg, u8 *out);

FH_XMOD s8 fh_parser_keyword(char *in, u8 ctg, u8 *out);

/*
 * This function takes a string and converts the input into the requested format
 * depending on the given type. Note that the output buffer is required to be
 * big enough to fit the potential output.
 *
 * @in: The input string
 * @type: The type of input to convert
 * @ctg: The category used by some formats
 * @out: A pointer to write the output to
 *
 * Returns: The number of bytes written to the output pointer or -1 if an error
 * 	    occurred
 */
FH_XMOD s8 fh_parser_value(char *in, u8 type, u8 ctg, u8 *out);

FH_XMOD char *fh_parser_attribute(char *in, char *attr, char *val);




#endif /* _FH_PARSER_H */
