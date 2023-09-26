#ifndef _FH_PARSER_H
#define _FH_PARSER_H

#include "stdinc.h"


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

FH_XMOD char *fh_parser_attribute(char *in, char *attr, char *val);




#endif /* _FH_PARSER_H */
