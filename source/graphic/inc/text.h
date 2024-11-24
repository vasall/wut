#ifndef _WUT_GRAPHIC_TEXT_H
#define _WUT_GRAPHIC_TEXT_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/graphic/inc/batch.h"

#include "source/math/inc/polygon.h"

#include "source/style/inc/style.h"

#include "source/component/inc/string_object.h"


#define WUT_TEXT_WORDWRAP	WUT_KW_TEXT_WORDWRAP
#define WUT_TEXT_NOWRAP		WUT_KW_TEXT_NOWRAP
#define WUT_TEXT_LETTERWRAP	WUT_KW_TEXT_LETTERWRAP


struct wut_TextInfo {
	/*
	 * A pointer to the dedicated batch renderer.
	 */
	struct wut_Batch	*batch;

	/*
	 * A pointer to the font.
	 */
	struct wut_Font		*font;

	/*
	 * A rectangle setting the render area.
	 */
	wut_iRect		*limits;

	/*
	 * The style used for the text.
	 */
	struct wut_Style	*style;
};


struct wut_TextElement {
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


#define WUT_TEXT_ALLOC		32

struct wut_TextBuffer {
	struct wut_TextInfo 	info;

	/* The head and tail of the element list */
	s16 			head;
	s16 			tail;

	/* The size and array containing the elements */
	s16			alloc;
	s16			count;
	struct wut_TextElement	*elements;

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
WUT_XMOD struct wut_TextBuffer *wut_txt_create(struct wut_TextInfo info);


/*
 * Destroy a text buffer an free the allocated memory.
 *
 * @tbuf: Pointer to the text buffer
 */
WUT_XMOD void wut_txt_destroy(struct wut_TextBuffer *tbuf);


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
WUT_XMOD s8 wut_txt_push(struct wut_TextBuffer *tbuf, s16 off, s16 len,
		char *text);


/*
 * Remove text from the given offset with the specified length.
 *
 * @tbuf: Pointer to the text buffer
 * @off: The offset position of the text
 * @len: The length of the text in characters
 */
WUT_XMOD void wut_txt_remove(struct wut_TextBuffer *tbuf, s16 off, s16 len);


/*
 * Copy the character elements to the batch renderer.
 *
 * @tbuf: Pointer to the text buffer
 * @opt: This flag indicates if the given limits should be used or not
 * @lim: The limits are passed onto the shader and limit the space to be drawn
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_XMOD s8 wut_txt_send(struct wut_TextBuffer *tbuf, s8 opt, wut_iRect lim);


/*
 * Update the position of all elements in the text buffer.
 *
 * @tbuf: A pointer to the text buffer
 */
WUT_XMOD void wut_txt_process(struct wut_TextBuffer *tbuf);


/*
 * Dump the content of the text buffer in the terminal.
 *
 * @tbuf: Pointer to the text buffer
 */
WUT_XMOD void wut_txt_dump(struct wut_TextBuffer *tbuf);

#endif /* _WUT_GRAPHIC_TEXT_H */
