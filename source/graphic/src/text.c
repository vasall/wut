#include "graphic/inc/text.h"

#include <stdlib.h>

#include "system/inc/system.h"

#include "utility/inc/alarm.h"


WUT_INTERN s8 text_resize(struct wut_TextBuffer *tbuf, s16 len)
{
	s16 nalloc = (tbuf->alloc + len) * 1.5;
	s32 size = nalloc * sizeof(struct wut_TextElement);
	void *pswap;
	s16 i;

	if(!(pswap = wut_realloc(tbuf->elements, size))) {
		WUT_ALARM(WUT_ERROR, "Failed to resize text buffer");
		return -1;
	}

	tbuf->elements = pswap;

	for(i = tbuf->alloc; i < nalloc; i++) {
		tbuf->elements[i].used = 0;
	}

	tbuf->alloc = nalloc;

	return 0;
}


WUT_INTERN s16 text_get_slots(struct wut_TextBuffer *tbuf, s16 num, s16 *out)
{
	s16 i;
	s16 j = 0;
	s16 left = num;

	for(i = 0; i < tbuf->alloc && left > 0; i++) {
		if(tbuf->elements[i].used == 0) {
			out[j++] = i;
			left--;
		}
	}

	return -1;
}


WUT_INTERN s16 text_find(struct wut_TextBuffer *tbuf, s16 idx)
{
	s16 i = 0;
	s16 run;

	if(idx < 0 || tbuf->head < 0 || tbuf->count < idx) {
		return -1;
	}

	/* Search from the top if index is on higher end */
	if(idx > tbuf->count>>1) {
		i = tbuf->count - 1;
		run = tbuf->tail;
		for(; i > idx; run = tbuf->elements[run].prev) {
			i++;
		}
	}
	/* Otherwise search from bottom */
	else {
		i = 0;
		run = tbuf->head;
		for(; i < idx; run = tbuf->elements[run].next) {
			i++;
		}

	}

	return run;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */



WUT_XMOD struct wut_TextBuffer *wut_txt_create(struct wut_TextInfo info)
{
	struct wut_TextBuffer *tbuf;
	s16 size;
	s16 i;

	if(!(tbuf = wut_malloc(sizeof(struct wut_TextBuffer)))) {
		WUT_ALARM(WUT_ERROR, "Failed to create text buffer");
		goto err_return;
	}

	tbuf->info = info;
	tbuf->head = -1;
	tbuf->tail = -1;
	tbuf->alloc = WUT_TEXT_ALLOC;
	tbuf->count = 0;

	tbuf->tex_spread = tbuf->info.font->data.spread_in_tex * 0.4;
	tbuf->vtx_spread = tbuf->info.font->data.spread_in_font * 0.4;

	size = tbuf->alloc * sizeof(struct wut_TextElement);
	if(!(tbuf->elements = wut_malloc(size))) {
		goto err_free_tbuf;
	}

	for(i = 0; i < tbuf->alloc; i++) {
		tbuf->elements[i].used = 0;
	}

	return tbuf;

err_free_tbuf:
	wut_free(tbuf);

err_return:
	return NULL;
}


WUT_XMOD void wut_txt_destroy(struct wut_TextBuffer *tbuf)
{
	if(!tbuf)
		return;

	wut_free(tbuf->elements);
	wut_free(tbuf);
}


WUT_XMOD s8 wut_txt_push(struct wut_TextBuffer *tbuf, s16 off, s16 len,
		char *text)
{
	s16 cut_prev;
	s16 cut_next;
	s16 rlast;
	s16 i;
	s16 tmp;
	char c;
	s16 slots[128];
	s16 left;
	struct wut_TextElement *ele;

	if(!tbuf || off < 0 || len < 0 || !text) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return -1;
	}

	/* Resize element array if necessary */
	if(tbuf->count + len > tbuf->alloc) {
		if(text_resize(tbuf, len) < 0) {
			WUT_ALARM(WUT_ERROR, "Failed to resize text buffer");
			goto err_return;
		}
	}

	/* Get the elements before and after the cut */	
	cut_next = text_find(tbuf, off);
	cut_prev = cut_next < 0 ? -1 : tbuf->elements[cut_next].prev;

	rlast = cut_prev;
	left = len;

	while(left > 0) {
		/* Aquire free slots in bulk */
		tmp = left > 128 ? 128 : left;
		text_get_slots(tbuf, tmp, slots);
			
		for(i = 0; i < tmp; i++) {
			ele = &tbuf->elements[slots[i]];
			c = text[len - left + i];

			/* Aquire the character and glyph */
			ele->character = c;
			ele->glyph = wut_GetGlyphIndex(tbuf->info.font, c);

			/* If no previous elements, this will become the head */
			if(rlast < 0) {
				tbuf->head = slots[i];
				ele->prev = -1;
			}
			else {
				tbuf->elements[rlast].next = slots[i];
				ele->prev = rlast;
			}

			ele->next = -1;
			ele->used = 1;

			rlast = slots[i];
		}

		left -= tmp;
	}

	/* If no elements following cut, last one will be tail */
	if(cut_next < 0) {
		tbuf->tail = rlast;
	}

	/* Update size of buffer and mark as changed */
	tbuf->count += len;

	wut_txt_dump(tbuf);

	return 0;

err_return:
	return -1;
}


WUT_XMOD void wut_txt_remove(struct wut_TextBuffer *tbuf, s16 off, s16 len)
{
	s16 cut_prev;
	s16 left;
	s16 run;
	s16 i;

	if(!tbuf || off < 0 || off > tbuf->count || len < 1) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	/* Sanitize input */
	left = len;
	if(off + len > tbuf->count) {
		left = tbuf->count - off;
	}

	run = text_find(tbuf, off);
	cut_prev = tbuf->elements[run].prev;
	
	for(i = left; i > 0; i--) {
		tbuf->elements[run].used = 0;
		
		if(tbuf->elements[run].next < 0) {
			tbuf->tail = run;
			break;
		}

		run = tbuf->elements[run].next;
	}


	/*
	 * Stitch everything back together.
	 */
	if(run < 0) {
		tbuf->tail = cut_prev;
	}
	else {
		tbuf->elements[run].prev = cut_prev;
	}

	if(cut_prev < 0) {
		tbuf->head = run;
	}
	else {
		tbuf->elements[cut_prev].next = run;
	}

	/* Update number of entries and mark as changed */
	tbuf->count -= len;
}


WUT_XMOD void wut_txt_process(struct wut_TextBuffer *tbuf)
{
	struct wut_Style *tstyle;
	wut_iRect *limits;
	struct wut_Font *font;
	s16 tmp;
		
	/* The curbatcht offset for placement */
	s16 line[2] = {0, 0};

	s16 run;
	struct wut_TextElement *tele;
	struct wut_TextElement *tele_last = NULL;

	struct wut_FontGlyph *glyph;
	struct wut_FontGlyph *glyph_last = NULL;

	/*
	 * This var remembers the index of the last space char or the start of
	 * the line which is needed for wrapping the text.
	 */
	s16 lbreak = -1;	/* The index of the element at the linestart */
	s16 lspace = -1;	/* The index of the last space */

	s8 fsol = 1;	/* This flag indicates if brake is at start of line */
	s16 lwidth;
	s8 ladv = 0;	/* This flag indicates if the line should advance */

	printf("Process text:\n");

	tstyle = tbuf->info.style;
	limits = tbuf->info.limits;
	font = tbuf->info.font;

	/* Calculate the initial position of the line */
	line[0] = (*limits)[0];
	line[1] = (*limits)[1] + tstyle->text_line_height;

	printf(">> Line: %d, %d\n", line[0], line[1]);
	printf(">> Text-Size: %d\n", tstyle->text_size);

	lbreak = tbuf->head;
	run = tbuf->head;

	while(run >= 0) {
		/*
		 * First gather all refebatchces for the character.
		 */
		tele = &tbuf->elements[run];
		glyph = wut_GetGlyphByIndex(font, tele->glyph);	


		/*
		 * Then calculate the instance origin.
		 */
		if(glyph_last == NULL) {
			tele->inst_origin_y = line[1];
			tele->inst_origin_x = line[0] + tbuf->vtx_spread * tstyle->text_size;
		}
		else {
			tele->inst_origin_y = line[1];
			tmp = glyph_last->hadvance * tstyle->text_size * tstyle->text_spacing;
			tele->inst_origin_x = tele_last->inst_origin_x + tmp; 
		}

		/*
		 * After that calculate the bottom left corner for the char.
		 */
		tmp = glyph->hbearing_x * tstyle->text_size;
		tele->position_x = tele->inst_origin_x + tmp;

		tmp = (glyph->height - glyph->hbearing_y) * tstyle->text_size;
		tele->position_y = tele->inst_origin_y + tmp;

		/*
		 * Calculate the width either from the start of the line or the
		 * last space character.
		 */
		tmp = tele->position_x + glyph->width * tstyle->text_size;
		lwidth = tmp - tbuf->elements[lbreak].position_x; 


		/* If the word extends over the limit try to wrap it */
		if(lwidth >= (*limits)[2]) {
			printf("Reached end of line(%d)\n", run);
			printf("tmp: %d, lwidth: %d\n", tmp, lwidth);
			printf("Wrap_mode: %d\n", tstyle->text_wrap_mode);
			printf("Width: %d\n", (*limits)[2]);

			switch(tstyle->text_wrap_mode) {
				case WUT_TEXT_WORDWRAP:
					printf("Wordwrap\n");

					if(fsol) {
						printf("SOL, just go to next!\n");
						break;
					}
					
					if(lspace >= 0) {
						/* Advance to the next line */
						ladv = 1;

						printf("Space, go to last space\n");
						run = lspace;
						break;
					}

					/* Advance to the next line */
					ladv = 1;

					printf("Otherwise\n");
					run = tbuf->elements[lbreak].next;
					break;

				case WUT_TEXT_NOWRAP:
					/* Do absolutelly nothing! */
					break;
				
				case WUT_TEXT_LETTERWRAP:
					printf("Letterwrap!\n");

					ladv = 1;

					if(fsol && lbreak == run) {
						run = tele->next;
					}

					printf("Reset to %d\n", run);
					break;
			}
		}

		/*
		 * Finally update the last-pointers for the next calculation and
		 * advance to the next line if requested.
		 */
		if(ladv) {
			printf("Go to next line\n");

			line[0] = (*limits)[0];
			line[1] += tstyle->text_line_height;

			tele_last = NULL;
			glyph_last = NULL;

			lbreak = run;
			lspace = -1;

			ladv = 0;
			fsol = 1;
		}
		else {
			tele_last = tele;
			glyph_last = glyph;
			run = tbuf->elements[run].next;

			if(tele->character == 0x20) {
				printf(">>>>>> FOUND A SPACE!!!\n");
				lspace = run;
				fsol = 0;
			}
		}
	}
	
}


WUT_XMOD s8 wut_txt_send(struct wut_TextBuffer *tbuf)
{
	struct wut_Batch *batch;
	struct wut_TextElement *ele;
	struct wut_FontGlyph *glyph;
	struct wut_Style *tstyle;


	s16 run;

	s32 idx[4];
	struct vertex {
		f32 vertex_x;
		f32 vertex_y;
		f32 vertex_z;
		f32 texture_u;
		f32 texture_v;
	} vtx[4];


	if(!tbuf || !tbuf->info.batch) {
		printf("tbuf undefined or no batch renderer\n");
		return -1;
	}

	batch = tbuf->info.batch;
	run = tbuf->head;

	while(run >= 0) {
		ele = &tbuf->elements[run];
		glyph = wut_GetGlyphByIndex(tbuf->info.font, ele->glyph);
		tstyle = tbuf->info.style;

		/* bottom left */
#if TEXTFIELD_DEBUG
		vtx[0].vertex_x = ele->position_x - tbuf->vtx_spread * tstyle->text_size;
#else
		vtx[0].vertex_x = ele->position_x;
#endif
		vtx[0].vertex_y = ele->position_y - tbuf->vtx_spread * tstyle->text_size;
		vtx[0].vertex_z = 0.0f;
		vtx[0].texture_u = glyph->tex_coord_x - tbuf->tex_spread;
		vtx[0].texture_v = glyph->tex_coord_y + glyph->tex_height + tbuf->tex_spread;

		/* bottom right */
#if TEXTFIELD_DEBUG
		vtx[1].vertex_x = ele->position_x +
			(glyph->width + tbuf->vtx_spread) * tstyle->text_size;
#else
		vtx[1].vertex_x = ele->position_x + glyph->width * tstyle->text_size;
#endif
		vtx[1].vertex_y = ele->position_y - tbuf->vtx_spread * tstyle->text_size;
		vtx[1].vertex_z = 0.0f;
		vtx[1].texture_u = glyph->tex_coord_x + glyph->tex_width + tbuf->tex_spread;
		vtx[1].texture_v = glyph->tex_coord_y + glyph->tex_height + tbuf->tex_spread;

		/* top right */
#if TEXTFIELD_DEBUG
		vtx[2].vertex_x = ele->position_x + 
			(glyph->width + tbuf->vtx_spread) * tstyle->text_size;
#else
		vtx[2].vertex_x = ele->position_x + glyph->width * tstyle->text_size;
#endif
		vtx[2].vertex_y = ele->position_y - 
			(glyph->height + tbuf->vtx_spread) * tstyle->text_size;
		vtx[2].vertex_z = 0.0f;
		vtx[2].texture_u = glyph->tex_coord_x + glyph->tex_width + tbuf->tex_spread;
		vtx[2].texture_v = glyph->tex_coord_y - tbuf->tex_spread;

		/* top left */
#if TEXTFIELD_DEBUG
		vtx[3].vertex_x = ele->position_x - tbuf->vtx_spread * tstyle->text_size;
#else
		vtx[3].vertex_x = ele->position_x;
#endif
		vtx[3].vertex_y = ele->position_y -
			(glyph->height + tbuf->vtx_spread) * tstyle->text_size;
		vtx[3].vertex_z = 0.0f;
		vtx[3].texture_u = glyph->tex_coord_x - tbuf->tex_spread;
		vtx[3].texture_v = glyph->tex_coord_y - tbuf->tex_spread;


		idx[0] = wut_bat_push_vertex(batch, (void *)&vtx[0]);
		idx[1] = wut_bat_push_vertex(batch, (void *)&vtx[1]);
		idx[2] = wut_bat_push_vertex(batch, (void *)&vtx[2]);
		idx[3] = wut_bat_push_vertex(batch, (void *)&vtx[3]);

		wut_bat_push_index(batch, idx[0]);
		wut_bat_push_index(batch, idx[2]);
		wut_bat_push_index(batch, idx[3]);

		wut_bat_push_index(batch, idx[0]);
		wut_bat_push_index(batch, idx[1]);
		wut_bat_push_index(batch, idx[2]);

		run = ele->next;
	}

	return 0;
}


WUT_XMOD void wut_txt_dump(struct wut_TextBuffer *tbuf)
{
	s16 run;
	s16 i;

	printf("Dump content of text buffer:\n");
	printf("Count: %d, Head: %d, Tail: %d\n",
			tbuf->count, tbuf->head, tbuf->tail);

	if(tbuf->count < 1) {
		printf("Empty!\n");
		return;
	}

	i = 0;
	run = tbuf->head;
	while(run >= 0) {
		printf("[%2d]  %3d  %c\n", i, run, tbuf->elements[run].character);
		run = tbuf->elements[run].next;
		i++;
	}
}
