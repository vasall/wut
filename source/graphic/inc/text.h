#ifndef _WT_GRAPHIC_TEXT_H
#define _WT_GRAPHIC_TEXT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/batch.h"

#include "utility/inc/shape.h"

#include "style/inc/style.h"


#define WT_TEXT_WORDWRAP	WT_KW_TEXT_WORDWRAP
#define WT_TEXT_NOWRAP		WT_KW_TEXT_NOWRAP
#define WT_TEXT_LETTERWRAP	WT_KW_TEXT_LETTERWRAP


struct wt_text_info {
	/*
	 * A pointer to the dedicated batch renderer.
	 */
	struct wt_batch		*batch;

	/*
	 * A pointer to the font.
	 */
	struct wt_font		*font;

	/*
	 * A rectangle setting the render area.
	 */
	struct wt_rect		*limits;

	/*
	 * The style used for the text.
	 */
	struct wt_style		*style;
};

struct wt_text_element {
	s8	used;

	s16	next;
	s16 	prev;

	char	character;
	s16	glyph;

	s16	inst_origin_x;
	s16	inst_origin_y;

	/* Position of the bottom-left corner */
	s16	position_x;
	s16	position_y;
};

#define WT_TEXT_ALLOC		32

struct wt_text_buffer {
	struct wt_text_info 	info;

	/* The head and tail of the element list */
	s16 			head;
	s16 			tail;

	/* The size and array containing the elements */
	s16			alloc;
	s16			count;
	struct wt_text_element	*elements;

	/* The size of the text buffer in pixel */
	s16			width;
	s16			height;

	/* The spread to use for the text buffer */
	s16			tex_spread;
	s16			vtx_spread;
};


/*
 * Create a new text buffer.
 *
 * @info: The necessary info for the text buffer
 *
 * Returns: Either a pointer to the newly created text buffer or NULL if an
 * 	    error occurred
 */
WT_API struct wt_text_buffer *wt_text_create(struct wt_text_info info);


/*
 * Destroy a text buffer an free the allocated memory.
 *
 * @tbuf: Pointer to the text buffer
 */
WT_API void wt_text_destroy(struct wt_text_buffer *tbuf);


/*
 * Push text into the text buffer from the given offset position.
 *
 * @tbuf: Pointer to the text buffer
 * @off: The offset position of the pushed text in the buffer
 * @len: The length of the text in characters
 * @text: The string
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_text_push(struct wt_text_buffer *tbuf, s16 off, s16 len,
		char *text);


/*
 * Remove text from the given offset with the specified length.
 *
 * @tbuf: Pointer to the text buffer
 * @off: The offset position of the text
 * @len: The length of the text in characters
 */
WT_API void wt_text_remove(struct wt_text_buffer *tbuf, s16 off, s16 len);


/*
 * Copy the character elements to the batch renderer.
 *
 * @tbuf: Pointer to the text buffer
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_text_send(struct wt_text_buffer *tbuf);


/*
 * Update the position of all elements in the text buffer.
 *
 * @tbuf: A pointer to the text buffer
 */
WT_API void wt_text_process(struct wt_text_buffer *tbuf);


/*
 * Dump the content of the text buffer in the terminal.
 *
 * @tbuf: Pointer to the text buffer
 */
WT_API void wt_text_dump(struct wt_text_buffer *tbuf);

#endif /* _WT_GRAPHIC_TEXT_H */
