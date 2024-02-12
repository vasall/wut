#include "utility/inc/flex.h"

#include "utility/alarm/inc/alarm.h"
#include "utility/inc/utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


FH_INTERN s8 flex_limit(char c, s8 low, s8 high)
{
	return ((c >= low) && (c <= high));
}


FH_INTERN s8 flex_is_operand(char c)
{
	/*
	 * The following characters are allowed:
	 * 
	 *   - numbers: 0..9
	 *   - spec. char: .
	 */
	return (flex_limit(c, 0x30, 0x39) || c == 0x2E);
}


FH_INTERN s8 flex_is_unit(char c)
{
	/*
	 * The following characters are allowed:
	 *
	 *   - letters: a..z and A..Z
	 *   - spec. char: %
	 */
	return (flex_limit(c, 0x61, 0x7A) || flex_limit(c, 0x41, 0x5A) ||
			c == 0x25);
}


FH_INTERN s8 flex_is_operator(char c)
{
	/*
	 * The following characters are allowed:
	 *
	 *   -spec. char: + - * / ( )
	 */
	return (c == 0x2B || c == 0x2D || c == 0x2A || c == 0x2F || c == 0x28 ||
			c == 0x29);
}


FH_INTERN void flex_sanitize(char *s)
{
	char *d = s;
	do {
		while (*d == ' ') {
			++d;
		}
	} while ((*(s++) = *(d++)));
}


FH_INTERN s8 flex_parse_token(u8 opt, char *s, struct fh_flex_token *tok)
{
	char buf[64];
	char *c;
	u8 tail = 0;

	/* 		OPERAND	W/O UNIT	 */
	if(opt == 1) {
		c = s;
		while(*c && (flex_limit(*c, 0x30, 0x39) || *c == 0x2E))
			buf[tail++] = *c++;
		buf[tail] = 0;

		if(tail == 0) {
			FH_ALARM(FH_ERROR, "No digits detected");
			return -1;
		}

		tok->code = 0x11;	/* const */
		tok->value = atof(buf);

		tail = 0;
		while(*c && flex_is_unit(*c))
			buf[tail++] = *c++;
		buf[tail] = 0;

		if(tail != 0) {
			if(strcmp(buf, "px") == 0) {
				if(ceil(tok->value) != tok->value) {
					FH_ALARM(FH_ERROR, "Pixels must be integer");
					return -1;
				}
				tok->code = 0x12;
			}
			else if(strcmp(buf, "pct") == 0) {
				tok->value /= 100.0;

				tok->code = 0x13;
			}
			else if(strcmp(buf, "em") == 0) {
				tok->code = 0x14;
			}
			else {
				FH_ALARM(FH_ERROR, "Unit invalid");
				return -1;
			}
		}

	}
	/* 		OPERATOR		 */
	else if(opt == 3) {
		tok->value = 0;

		switch(*s) {
			case 0x28: tok->code = 0x01; break; /* ( */
			case 0x29: tok->code = 0x02; break; /* ) */
			case 0x2A: tok->code = 0x03; break; /* * */
			case 0x2B: tok->code = 0x04; break; /* + */
			case 0x2D: tok->code = 0x05; break; /* - */
			case 0x2F: tok->code = 0x06; break; /* / */
		}
	}

	return 0;
}


FH_INTERN struct fh_list *flex_tokenize(char *inp)
{
	struct fh_list *lst;
	struct fh_flex_token tok;

	char *c;
	char buf[64];
	u8 buf_tail;
	u8 read;	
	u8 fin;

	if(!(lst = fh_list_create(sizeof(struct fh_flex_token), 64))) {
		FH_ALARM(FH_ERROR, "Failed to create list for tokens");
		return NULL;
	}

	c = inp;
	buf_tail = 0;

	read = 0;
	fin = 0;

	do {
		if(*c == 0x00) {
			fin = read;
		}
		/* 		SPACE		 */
		else if(*c == 0x20) {
			/* Do nothing */
		}
		/* 		OPERAND		 */
		else if(flex_is_operand(*c) || flex_is_unit(*c)) {
			/* Start reading operand */
			if(read == 0) {
				read = 1;
				buf_tail = 0;
			}
			else if(read != 1) {
				fin = read;
				read = 1;

			}
		}
		/* 		OPERATOR	 */
		else if(flex_is_operator(*c)) {
			/* Start reading operator */
			if(read == 0) {
				read = 3;
				buf_tail = 0;
			}
			else if(read != 3) {
				fin = read;
				read = 3;
			}
		}
		else {
			FH_ALARM(FH_ERROR, "Invalid character");
			goto err_destroy_list;
		}


		if(fin) {
			buf[buf_tail] = 0;

			flex_sanitize(buf);
			if(flex_parse_token(fin, buf, &tok) < 0) {
				FH_ALARM(FH_ERROR, "Invalid expression");
				goto err_destroy_list;
			}

			if(fh_list_push(lst, &tok) < 0) {
				FH_ALARM(FH_ERROR, "Failed to add token to list");
				goto err_destroy_list;
			}

			buf_tail = 0;
			fin = 0;
		}


		if(read) {
			buf[buf_tail] = *c;
			buf_tail++;
		}

	} while(*(c++));

	return lst;

err_destroy_list:
	fh_list_destroy(lst);
	return NULL;
}


FH_INTERN s8 flex_operator_prio(struct fh_flex_token *tok)
{
	switch(tok->code) {
		case 0x03: return 0x03;	/* * */
		case 0x04: return 0x02;	/* + */
		case 0x05: return 0x02;	/* - */
		case 0x06: return 0x03;	/* / */
	}
	return 0x00;
}


FH_INTERN s8 flex_shunting_yard(struct fh_list *inp, struct fh_list **out)
{
	struct fh_list *output;
	struct fh_list *operators;
	struct fh_flex_token tok;
	struct fh_flex_token tok_swp;
	u8 prio[2];
	u8 check;
	u8 opensign = 0; /* 0: positive, 1: negative */

	if(!(output = fh_list_create(sizeof(struct fh_flex_token), 10))) {
		FH_ALARM(FH_ERROR, "Failed to create output list");
		return -1;
	}

	if(!(operators = fh_list_create(sizeof(struct fh_flex_token), 10))) {
		FH_ALARM(FH_ERROR, "Failed to create operator list");
		goto err_destroy_output;
	}

	while(fh_list_shift(inp, &tok)) {
		/* Push operand to output */
		if(tok.code > 0x06) {
			/* To indicate the first operand is negative */
			if(opensign) {
				tok.value *= -1.0;
				opensign = 0;
			}

			fh_list_push(output, &tok);
		}
		/* Push operator into operator-stack */
		else if(tok.code >= 0x03 && tok.code <= 0x06) {
			if(output->count < 1) {
				if(tok.code != 0x04 && tok.code != 0x05) {
					FH_ALARM(FH_ERROR, "Missing operand");
					goto err_destroy_operators;
				}
				/*
				 * Hotfix, so that the initial operand can be
				 * negative.
				 */
				else if(tok.code == 0x05) {
					opensign = 1;
				}
				continue;
			}

			while(fh_list_pop(operators, &tok_swp)) {
				prio[0] = flex_operator_prio(&tok);
				prio[1] = flex_operator_prio(&tok_swp);

				if(tok.code == 1 || prio[0] > prio[1]) {
					fh_list_push(operators, &tok_swp);
					break;
				}

				fh_list_push(output, &tok_swp);
			}

			fh_list_push(operators, &tok);
		}
		/* Handle opening-bracket '(' */
		else if(tok.code == 0x01) {
			fh_list_push(operators, &tok);
		}
		/* Handle closing-bracket ')' */
		else if(tok.code == 0x02) {
			if(operators->count < 1) {
				FH_ALARM(FH_ERROR, "Missing opening bracket");
				goto err_destroy_operators;
			}

			check = 0;
			while(fh_list_pop(operators, &tok_swp)) {
				if(tok_swp.code == 0x01) {
					check = 1;
					break;
				}
				fh_list_push(output, &tok_swp);
			}
			if(!check) {
				FH_ALARM(FH_ERROR, "Missing opening bracket");
				goto err_destroy_operators;
			}
		}
	}

	while(fh_list_pop(operators, &tok))
		fh_list_push(output, &tok);

	
	fh_list_destroy(operators);
	*out = output;

	return 0;

err_destroy_operators:
	fh_list_destroy(operators);

err_destroy_output:
	fh_list_destroy(output);
	return -1;
}


FH_INTERN s8 flex_clbk_print(void *ptr, s16 idx, void *data)
{
	struct fh_flex_token *tok = (struct fh_flex_token *)ptr;

	fh_Ignore(idx);
	fh_Ignore(data);

	switch(tok->code) {
		case 0x01: printf("("); break;
		case 0x02: printf(")"); break;
		case 0x03: printf("*"); break;
		case 0x04: printf("+"); break;
		case 0x05: printf("-"); break;
		case 0x06: printf("/"); break;

		case 0x11: printf("%f", tok->value); break;
		case 0x12: printf("%fpx", tok->value); break;
		case 0x13: printf("%fpct", tok->value); break;
		case 0x14: printf("%fem", tok->value); break;

		default: printf("?");
	}

	return 0;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_XMOD fh_flex_t fh_flex_parse(fh_flex_t flx, char *inp)
{
	struct fh_list *tokens;
	fh_flex_t f = flx;

	if(!inp || !(inp)) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return flx;
	}

	if(!(tokens = flex_tokenize(inp)))
		return flx;

	if(!f) {
		if(!(f = malloc(sizeof(struct fh_flex)))) {
			FH_ALARM(FH_ERROR, "Failed to allocate memory for flex");
			return flx;
		}
	}
	else {
		fh_list_destroy(f->list);
	}

	if(flex_shunting_yard(tokens, &f->list) < 0) {
		goto err_destroy_tokens_raw;
	}

	fh_list_destroy(tokens);
	return f;

err_destroy_tokens_raw:
	fh_list_destroy(tokens);
	return f;
}


FH_XMOD void fh_flex_destroy(fh_flex_t flx)
{
	if(!flx) return;

	fh_list_destroy(flx->list);
	free(flx);
}


FH_XMOD s32 fh_flex_process(fh_flex_t flx, u16 *ref)
{
	struct fh_flex_token tok;
	struct fh_list *stk;
	f32 value;
	f32 opd[2];
	u16 i = 0;

	if(!flx) return 0;

	if(!(stk = fh_list_create(F32_S, 64))) {
		FH_ALARM(FH_ERROR, "Failed to create operand stack");
		return 0;
	}

	while(fh_list_get(flx->list, i++, &tok)) {
		/*  Handle operands  */
		if(tok.code > 0x06) {
			switch(tok.code) {
				case 0x11: value = tok.value; break;
				case 0x12: value = tok.value; break;
				case 0x13: value = tok.value * ref[0]; break;
				case 0x14: value = tok.value * ref[1]; break;
				default:  value = tok.value; break;
			}
			fh_list_push(stk, &value);
		}
		/* Handle operators */
		else {
			fh_list_pop(stk, &opd[0]);
			fh_list_pop(stk, &opd[1]);

			switch(tok.code) {
				case 0x03: value = opd[1] * opd[0]; break;
				case 0x04: value = opd[1] + opd[0]; break;
				case 0x05: value = opd[1] - opd[0]; break;
				case 0x06: value = opd[1] / opd[0]; break;
			}

			fh_list_push(stk, &value);
		}
	}

	fh_list_pop(stk, &value);
	fh_list_destroy(stk);

	return (s32)value;
}


FH_XMOD void fh_flex_print(fh_flex_t flx)
{
	if(!flx) {
		printf("undefined");
		return;
	}

	if(flx->list->count < 1) {
		printf("empty");
		return;
	}

	printf("%d tokens: ", flx->list->count);

	fh_list_apply(flx->list, &flex_clbk_print, NULL);
}
