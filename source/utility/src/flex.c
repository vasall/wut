#include "utility/inc/flex.h"

#include "utility/inc/alarm.h"
#include "utility/inc/utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


WUT_INTERN s8 flx_limit(char c, s8 low, s8 high)
{
        return ((c >= low) && (c <= high));
}


WUT_INTERN s8 flx_is_operand(char c)
{
        /*
         * The following characters are allowed:
         * 
         *   - numbers: 0..9
         *   - spec. char: .
         */
        return (flx_limit(c, 0x30, 0x39) || c == 0x2E);
}


WUT_INTERN s8 flx_is_unit(char c)
{
        /*
         * The following characters are allowed:
         *
         *   - letters: a..z and A..Z
         *   - spec. char: %
         */
        return (flx_limit(c, 0x61, 0x7A) || flx_limit(c, 0x41, 0x5A) ||
                        c == 0x25);
}


WUT_INTERN s8 flx_is_operator(char c)
{
        /*
         * The following characters are allowed:
         *
         *   -spec. char: + - * / ( )
         */
        return (c == 0x2B || c == 0x2D || c == 0x2A || c == 0x2F || c == 0x28 ||
                        c == 0x29);
}


WUT_INTERN void flx_sanitize(char *s)
{
        char *d = s;
        do {
                while (*d == ' ') {
                        ++d;
                }
        } while ((*(s++) = *(d++)));
}


WUT_INTERN s8 flx_parse_token(u8 opt, char *s, struct wut_FlexToken *tok)
{
        char buf[64];
        char *c;
        u8 tail = 0;

        /* 		OPERAND	W/O UNIT	 */
        if(opt == 1) {
                c = s;
                while(*c && (flx_limit(*c, 0x30, 0x39) || *c == 0x2E))
                        buf[tail++] = *c++;
                buf[tail] = 0;

                if(tail == 0) {
                        WUT_ALARM(WUT_ERROR, "No digits detected");
                        return -1;
                }

                tok->code = 0x11;	/* const */
                tok->value = atof(buf);

                tail = 0;
                while(*c && flx_is_unit(*c))
                        buf[tail++] = *c++;
                buf[tail] = 0;

                if(tail != 0) {
                        if(strcmp(buf, "px") == 0) {
                                if(ceil(tok->value) != tok->value) {
                                        WUT_ALARM(WUT_ERROR, "Pixels must be integer");
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
                                WUT_ALARM(WUT_ERROR, "Unit invalid");
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


WUT_INTERN struct wut_List *flx_tokenize(char *inp)
{
        struct wut_List *lst;
        struct wut_FlexToken tok;

        char *c;
        char buf[64];
        u8 buf_tail;
        u8 read;	
        u8 fin;

        if(!(lst = wut_CreateList(sizeof(struct wut_FlexToken), 64))) {
                WUT_ALARM(WUT_ERROR, "Failed to create list for tokens");
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
                else if(flx_is_operand(*c) || flx_is_unit(*c)) {
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
                else if(flx_is_operator(*c)) {
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
                        WUT_ALARM(WUT_ERROR, "Invalid character");
                        goto err_destroy_list;
                }


                if(fin) {
                        buf[buf_tail] = 0;

                        flx_sanitize(buf);
                        if(flx_parse_token(fin, buf, &tok) < 0) {
                                WUT_ALARM(WUT_ERROR, "Invalid expression");
                                goto err_destroy_list;
                        }

                        if(wut_PushList(lst, &tok) < 0) {
                                WUT_ALARM(WUT_ERROR, "Failed to add token to list");
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
        wut_DestroyList(lst);
        return NULL;
}


WUT_INTERN s8 flx_operator_prio(struct wut_FlexToken *tok)
{
        switch(tok->code) {
                case 0x03: return 0x03;	/* * */
                case 0x04: return 0x02;	/* + */
                case 0x05: return 0x02;	/* - */
                case 0x06: return 0x03;	/* / */
        }
        return 0x00;
}


WUT_INTERN s8 flx_shunting_yard(struct wut_List *inp, struct wut_List **out)
{
        struct wut_List *output;
        struct wut_List *operators;
        struct wut_FlexToken tok;
        struct wut_FlexToken tok_swp;
        u8 prio[2];
        u8 check;
        u8 opensign = 0; /* 0: positive, 1: negative */

        if(!(output = wut_CreateList(sizeof(struct wut_FlexToken), 10))) {
                WUT_ALARM(WUT_ERROR, "Failed to create output list");
                return -1;
        }

        if(!(operators = wut_CreateList(sizeof(struct wut_FlexToken), 10))) {
                WUT_ALARM(WUT_ERROR, "Failed to create operator list");
                goto err_destroy_output;
        }

        while(wut_ShiftList(inp, &tok)) {
                /* Push operand to output */
                if(tok.code > 0x06) {
                        /* To indicate the first operand is negative */
                        if(opensign) {
                                tok.value *= -1.0;
                                opensign = 0;
                        }

                        wut_PushList(output, &tok);
                }
                /* Push operator into operator-stack */
                else if(tok.code >= 0x03 && tok.code <= 0x06) {
                        if(output->count < 1) {
                                if(tok.code != 0x04 && tok.code != 0x05) {
                                        WUT_ALARM(WUT_ERROR, "Missing operand");
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

                        while(wut_PopList(operators, &tok_swp)) {
                                prio[0] = flx_operator_prio(&tok);
                                prio[1] = flx_operator_prio(&tok_swp);

                                if(tok.code == 1 || prio[0] > prio[1]) {
                                        wut_PushList(operators, &tok_swp);
                                        break;
                                }

                                wut_PushList(output, &tok_swp);
                        }

                        wut_PushList(operators, &tok);
                }
                /* Handle opening-bracket '(' */
                else if(tok.code == 0x01) {
                        wut_PushList(operators, &tok);
                }
                /* Handle closing-bracket ')' */
                else if(tok.code == 0x02) {
                        if(operators->count < 1) {
                                WUT_ALARM(WUT_ERROR, "Missing opening bracket");
                                goto err_destroy_operators;
                        }

                        check = 0;
                        while(wut_PopList(operators, &tok_swp)) {
                                if(tok_swp.code == 0x01) {
                                        check = 1;
                                        break;
                                }
                                wut_PushList(output, &tok_swp);
                        }
                        if(!check) {
                                WUT_ALARM(WUT_ERROR, "Missing opening bracket");
                                goto err_destroy_operators;
                        }
                }
        }

        while(wut_PopList(operators, &tok))
                wut_PushList(output, &tok);


        wut_DestroyList(operators);
        *out = output;

        return 0;

err_destroy_operators:
        wut_DestroyList(operators);

err_destroy_output:
        wut_DestroyList(output);
        return -1;
}


WUT_INTERN s8 flx_clbk_print(void *ptr, s16 idx, void *data)
{
        struct wut_FlexToken *tok = (struct wut_FlexToken *)ptr;

        WUT_IGNORE(idx);
        WUT_IGNORE(data);

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

WUT_XMOD struct wut_Flex *wut_flx_parse(struct wut_Flex *flx, char *inp)
{
        struct wut_List *tokens;
        struct wut_Flex *f = flx;

        if(!inp || !(inp)) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                return flx;
        }

        if(!(tokens = flx_tokenize(inp)))
                return flx;

        if(!f) {
                if(!(f = malloc(sizeof(struct wut_Flex)))) {
                        WUT_ALARM(WUT_ERROR, "Failed to allocate memory for flex");
                        return flx;
                }
        }
        else {
                wut_DestroyList(f->list);
        }

        if(flx_shunting_yard(tokens, &f->list) < 0) {
                goto err_destroy_tokens_raw;
        }

        wut_DestroyList(tokens);
        return f;

err_destroy_tokens_raw:
        wut_DestroyList(tokens);
        return f;
}


WUT_XMOD void wut_flx_destroy(struct wut_Flex *flx)
{
        if(!flx) return;

        wut_DestroyList(flx->list);
        free(flx);
}


WUT_XMOD s32 wut_flx_process(struct wut_Flex *flx, u16 *ref)
{
        struct wut_FlexToken *tok;
        struct wut_List *stk;
        f32 value;
        f32 opd[2];
        u16 i = 0;

        if(!flx) return 0;

        if(!(stk = wut_CreateList(F32_S, 64))) {
                WUT_ALARM(WUT_ERROR, "Failed to create operand stack");
                return 0;
        }

        while(wut_GetList(flx->list, i++, (void **)&tok)) {
                /*  Handle operands  */
                if(tok->code > 0x06) {
                        switch(tok->code) {
                                case 0x11: value = tok->value; break;
                                case 0x12: value = tok->value; break;
                                case 0x13: value = tok->value * ref[0]; break;
                                case 0x14: value = tok->value * ref[1]; break;
                                default:  value = tok->value; break;
                        }
                        wut_PushList(stk, &value);
                }
                /* Handle operators */
                else {
                        wut_PopList(stk, &opd[0]);
                        wut_PopList(stk, &opd[1]);

                        switch(tok->code) {
                                case 0x03: value = opd[1] * opd[0]; break;
                                case 0x04: value = opd[1] + opd[0]; break;
                                case 0x05: value = opd[1] - opd[0]; break;
                                case 0x06: value = opd[1] / opd[0]; break;
                        }

                        wut_PushList(stk, &value);
                }
        }

        wut_PopList(stk, &value);
        wut_DestroyList(stk);

        return (s32)value;
}


WUT_XMOD void wut_flx_print(struct wut_Flex *flx)
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

        wut_ApplyList(flx->list, &flx_clbk_print, NULL);
}
