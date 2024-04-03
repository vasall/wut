#ifndef _FH_WIDGET_TEXTFIELD_H
#define _FH_WIDGET_TEXTFIELD_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/font.h"
#include "graphic/inc/text.h"

struct fh_textfield {
	/* A pointer to the element */
	struct fh_element *element;

	/* The content of the textfield */
	s16 length;
	char content[256];

	/* The underlying text buffer used for rendering */
	struct fh_text_buffer *tbuffer;
};


/*
 * Create a new textfield-widget.
 *
 * @ele: A pointer to the element
 * @font: Pointer to the font to use
 *
 * Returns: Either a pointer to the new textfield or NULL if an error occurred
 */
FH_API struct fh_textfield *fh_textfield_create(struct fh_element *ele,
		struct fh_font *font);


/*
 * Destroy a textfield and free the allocated memory.
 *
 * @txt: Pointer to the textfield
 */
FH_API void fh_textfield_destroy(struct fh_textfield *txt);


/*
 * Update a textfield.
 *
 * @txtf: Pointer to the textfield
 */
FH_API void fh_textfield_update(struct fh_textfield *txtf);


/*
 * Render a textfield onto the screen.
 *
 * @txtf: Pointer to the textfield
 */
FH_API void fh_textfield_render(struct fh_textfield *txtf);

#endif /* _FH_WIDGET_TEXTFIELD_H */
