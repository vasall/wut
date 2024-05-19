#ifndef _WT_WIDGET_TEXTFIELD_H
#define _WT_WIDGET_TEXTFIELD_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/font.h"
#include "graphic/inc/text.h"

struct wt_textfield {
	/* A pointer to the element */
	struct wt_element *element;

	/* The content of the textfield */
	s16 length;
	char content[256];

	/* The underlying text buffer used for rendering */
	struct wt_text_buffer *tbuffer;
};


/*
 * Create a new textfield-widget.
 *
 * @ele: A pointer to the element
 * @font: Pointer to the font to use
 *
 * Returns: Either a pointer to the new textfield or NULL if an error occurred
 */
WT_API struct wt_textfield *wt_textfield_create(struct wt_element *ele,
		struct wt_font *font);


/*
 * Destroy a textfield and free the allocated memory.
 *
 * @txt: Pointer to the textfield
 */
WT_API void wt_textfield_destroy(struct wt_textfield *txt);


/*
 * Update a textfield.
 *
 * @txtf: Pointer to the textfield
 */
WT_API void wt_textfield_update(struct wt_textfield *txtf);


/*
 * Render a textfield onto the screen.
 *
 * @txtf: Pointer to the textfield
 */
WT_API void wt_textfield_render(struct wt_textfield *txtf);

#endif /* _WT_WIDGET_TEXTFIELD_H */
