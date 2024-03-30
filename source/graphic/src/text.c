#include "graphic/inc/text.h"

#include <stdlib.h>

#include "system/inc/system.h"

#include "utility/inc/alarm.h"


FH_INTERN s8 text_resize(struct fh_text_buffer *tbuf, s16 len)
{
	s16 nalloc = (tbuf->alloc + len) * 1.5;
	s32 size = nalloc * sizeof(struct fh_text_element);
	void *pswap;
	s16 i;

	if(!(pswap = fh_realloc(tbuf->elements, size))) {
		FH_ALARM(FH_ERROR, "Failed to resize text buffer");
		return -1;
	}

	tbuf->elements = pswap;

	for(i = tbuf->alloc; i < nalloc; i++) {
		tbuf->elements[i].used = 0;
	}

	tbuf->alloc = nalloc;

	return 0;
}


FH_INTERN s16 text_get_slots(struct fh_text_buffer *tbuf, s16 num, s16 *out)
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


FH_INTERN s16 text_find(struct fh_text_buffer *tbuf, s16 idx)
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



FH_API struct fh_text_buffer *fh_text_create(struct fh_text_info info)
{
	struct fh_text_buffer *tbuf;
	s16 size;
	s16 i;

	if(!(tbuf = fh_malloc(sizeof(struct fh_text_buffer)))) {
		FH_ALARM(FH_ERROR, "Failed to create text buffer");
		goto err_return;
	}

	tbuf->info = info;
	tbuf->head = -1;
	tbuf->tail = -1;
	tbuf->alloc = FH_TEXT_ALLOC;
	tbuf->count = 0;

	tbuf->tex_spread = tbuf->info.font->data.spread_in_tex * 0.4;
	tbuf->vtx_spread = tbuf->info.font->data.spread_in_font * 0.4;

	size = tbuf->alloc * sizeof(struct fh_text_element);
	if(!(tbuf->elements = fh_malloc(size))) {
		goto err_free_tbuf;
	}

	for(i = 0; i < tbuf->alloc; i++) {
		tbuf->elements[i].used = 0;
	}

	return tbuf;

err_free_tbuf:
	fh_free(tbuf);

err_return:
	return NULL;
}


FH_API void fh_text_destroy(struct fh_text_buffer *tbuf)
{
	if(!tbuf)
		return;

	fh_free(tbuf->elements);
	fh_free(tbuf);
}


FH_API void fh_text_process(struct fh_text_buffer *tbuf)
{
	struct fh_restyle_text *tstyle;
	struct fh_rect *limits;
	struct fh_font *font;
	s16 tmp;
		
	/* The curbatcht offset for placement */
	s16 line[2] = {0, 0};

	s16 run;
	struct fh_text_element *tele;
	struct fh_text_element *tele_last = NULL;

	struct fh_font_glyph *glyph;
	struct fh_font_glyph *glyph_last = NULL;

	/*
	 * This var remembers the index of the last space char or the start of
	 * the line which is needed for wrapping the text.
	 */
	s16 lbreak = -1;
	s8 fsol = 1;	/* This flag indicates if brake is at start of line */
	s16 lwidth;
	s8 ladv;	/* This flag indicates if the line should advance */

	printf("Process text:\n");

	tstyle = tbuf->info.style;
	limits = tbuf->info.limits;
	font = tbuf->info.font;

	/* Calculate the initial position of the line */
	line[0] = limits->x;
	line[1] = 40; 

	printf("Limits: %d, %d\n", line[0], line[1]);
	printf("Text-Size: %d\n", tstyle->size);

	lbreak = tbuf->head;
	run = tbuf->head;
	
	printf("Head: %d\n", run);

	while(run >= 0) {
		printf("A\n");	
	
		/*
		 * First gather all refebatchces for the character.
		 */
		tele = &tbuf->elements[run];
		
		printf("B %d\n", tele->glyph);

		glyph = fh_GetGlyphByIndex(font, tele->glyph);

		printf("C\n");


		/*
		 * Then calculate the instance origin.
		 */
		printf("Calc instance origin\n");
		if(glyph_last == NULL) {
			tele->inst_origin_y = line[1];
			tele->inst_origin_x = line[0] + tbuf->vtx_spread * tstyle->size;
		}
		else {
			tele->inst_origin_y = line[1];
			tmp = glyph_last->hadvance * tstyle->size * tstyle->spacing;
			tele->inst_origin_x = tele_last->inst_origin_x + tmp; 
		}

		/*
		 * After that calculate the bottom left corner for the char.
		 */
		printf("Calculate bottom left corner\n");
		tmp = glyph->hbearing_x * tstyle->size;
		tele->position_x = tele->inst_origin_x + tmp;

		tmp = (glyph->height - glyph->hbearing_y) * tstyle->size;
		tele->position_y = tele->inst_origin_y + tmp;

		/*
		 * Calculate the width either from the start of the line or the
		 * last space character.
		 */
		printf("Calculate the width\n");
		tmp = tele->position_x + glyph->width * tstyle->size;
		lwidth = tmp - tbuf->elements[lbreak].position_x; 

		/* If the word extends over the limit try to wrap it */	
		printf("Apply wrapping\n");
		if(lwidth > limits->w) {
			switch(tstyle->wrap_mode) {
				case FH_TEXT_WORDWRAP:
					/* Advance to the next line */
					ladv = 1;
					
					if(fsol) {
						break;
					}

					run = tbuf->elements[lbreak].next;
					break;

				case FH_TEXT_NOWRAP:
					/* Do absolutelly nothing! */
					break;
				
				case FH_TEXT_LETTERWRAP:
					ladv = 1;

					run = lbreak;
					break;
			}
		}

		/*
		 * Finally update the last-pointers for the next calculation and
		 * advance to the next line if requested.
		 */
		printf("Depending on wrapping, update offset\n");
		if(ladv) {	
			line[0] = limits->x;
			line[1] += tstyle->line_height;

			tele_last = NULL;
			glyph_last = NULL;

			ladv = 0;
			fsol = 1;
		}
		else {
			tele_last = tele;
			glyph_last = glyph;
			run = tbuf->elements[run].next;

			if(tele->character == 0x20) {
				lbreak = run;
				fsol = 0;
			}
		}
	}
	
}


FH_API s8 fh_text_push(struct fh_text_buffer *tbuf, s16 off, s16 len,
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
	struct fh_text_element *ele;

	if(!tbuf || off < 0 || len < 0 || !text) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return -1;
	}

	/* Resize element array if necessary */
	if(tbuf->count + len > tbuf->alloc) {
		if(text_resize(tbuf, len) < 0) {
			FH_ALARM(FH_ERROR, "Failed to resize text buffer");
			goto err_return;
		}
	}

	/* Get the elements before and after the cut */	
	cut_next = text_find(tbuf, off);
	cut_prev = cut_next < 0 ? -1 : tbuf->elements[cut_next].prev;

	printf("Push(len %d, off %d): \"%s\"\n", len, off, text);
	printf("Before %d, After %d\n", cut_prev, cut_next);

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
			ele->glyph = fh_GetGlyphIndex(tbuf->info.font, c);

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

	fh_text_dump(tbuf);

	/* TODO */
	fh_text_process(tbuf);

	return 0;

err_return:
	return -1;
}


FH_API void fh_text_remove(struct fh_text_buffer *tbuf, s16 off, s16 len)
{
	s16 cut_prev;
	s16 left;
	s16 run;
	s16 i;

	if(!tbuf || off < 0 || off > tbuf->count || len < 1) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
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

	/* TODO */
	fh_text_process(tbuf);
}


FH_API s8 fh_text_send(struct fh_text_buffer *tbuf)
{
	struct fh_batch *batch;
	struct fh_text_element *ele;
	struct fh_font_glyph *glyph;
	struct fh_restyle_text *style;

	s16 i;

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
	i = 0;

	while(run >= 0) {
		ele = &tbuf->elements[run];
		glyph = fh_GetGlyphByIndex(tbuf->info.font, ele->glyph);
		style = tbuf->info.style;

		/* bottom left */
#if TEXTFIELD_DEBUG
		vtx[0].vertex_x = ele->position_x - tbuf->vtx_spread * style->size;
#else
		vtx[0].vertex_x = ele->position_x;
#endif
		vtx[0].vertex_y = ele->position_y - tbuf->vtx_spread * style->size;
		vtx[0].vertex_z = 0.0f;
		vtx[0].texture_u = glyph->tex_coord_x - tbuf->tex_spread;
		vtx[0].texture_v = glyph->tex_coord_y + glyph->tex_height + tbuf->tex_spread;

		/* bottom right */
#if TEXTFIELD_DEBUG
		vtx[1].vertex_x = ele->position_x +
			(glyph->width + tbuf->vtx_spread) * style->size;
#else
		vtx[1].vertex_x = ele->position_x + glyph->width * style->size;
#endif
		vtx[1].vertex_y = ele->position_y - tbuf->vtx_spread * style->size;
		vtx[1].vertex_z = 0.0f;
		vtx[1].texture_u = glyph->tex_coord_x + glyph->tex_width + tbuf->tex_spread;
		vtx[1].texture_v = glyph->tex_coord_y + glyph->tex_height + tbuf->tex_spread;

		/* top right */
#if TEXTFIELD_DEBUG
		vtx[2].vertex_x = ele->position_x + 
			(glyph->width + tbuf->vtx_spread) * style->size;
#else
		vtx[2].vertex_x = ele->position_x + glyph->width * style->size;
#endif
		vtx[2].vertex_y = ele->position_y - 
			(glyph->height + tbuf->vtx_spread) * style->size;
		vtx[2].vertex_z = 0.0f;
		vtx[2].texture_u = glyph->tex_coord_x + glyph->tex_width + tbuf->tex_spread;
		vtx[2].texture_v = glyph->tex_coord_y - tbuf->tex_spread;

		/* top left */
#if TEXTFIELD_DEBUG
		vtx[3].vertex_x = ele->position_x - tbuf->vtx_spread * style->size;
#else
		vtx[3].vertex_x = ele->position_x;
#endif
		vtx[3].vertex_y = ele->position_y -
			(glyph->height + tbuf->vtx_spread) * style->size;
		vtx[3].vertex_z = 0.0f;
		vtx[3].texture_u = glyph->tex_coord_x - tbuf->tex_spread;
		vtx[3].texture_v = glyph->tex_coord_y - tbuf->tex_spread;


		idx[0] = fh_batch_push_vertex(batch, (void *)&vtx[0]);
		idx[1] = fh_batch_push_vertex(batch, (void *)&vtx[1]);
		idx[2] = fh_batch_push_vertex(batch, (void *)&vtx[2]);
		idx[3] = fh_batch_push_vertex(batch, (void *)&vtx[3]);

		fh_batch_push_index(batch, idx[0]);
		fh_batch_push_index(batch, idx[2]);
		fh_batch_push_index(batch, idx[3]);

		fh_batch_push_index(batch, idx[0]);
		fh_batch_push_index(batch, idx[1]);
		fh_batch_push_index(batch, idx[2]);

		for(i = 0; i < 4; i++) {
			printf("[%d] x: %f, y: %f\n",
					i,
					vtx[i].vertex_x,
					vtx[i].vertex_y);
		}

		run = ele->next;
	}

	printf("Finished!\n");

	return 0;
}


FH_API void fh_text_dump(struct fh_text_buffer *tbuf)
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
