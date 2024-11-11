#ifndef _WUT_WIDGET_TEXTFIELD_H
#define _WUT_WIDGET_TEXTFIELD_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/graphic/inc/font.h"
#include "source/graphic/inc/text.h"


struct wut_Textfield {
	/* A pointer to the element */
	struct wut_Element *element;

	/* The content of the textfield */
	s16 length;
	char content[256];

	/* The underlying text buffer used for rendering */
	struct wut_TextBuffer *tbuffer;
};


/*
 * Create a new textfield-widget.
 *
 * @ele: A pointer to the element
 * @font: Pointer to the font to use
 *
 * Returns: Either a pointer to the new textfield or NULL if an error occurred
 */
WUT_XMOD struct wut_Textfield *wut_tfd_create(struct wut_Element *ele,
		struct wut_Font *font);


/*
 * Destroy a textfield and free the allocated memory.
 *
 * @txt: Pointer to the textfield
 */
WUT_XMOD void wut_tfd_destroy(struct wut_Textfield *txt);


/*
 * Update a textfield.
 *
 * @txtf: Pointer to the textfield
 */
WUT_XMOD void wut_tfd_update(struct wut_Textfield *txtf);


/*
 * Render a textfield onto the screen.
 *
 * @txtf: Pointer to the textfield
 */
WUT_XMOD void wut_tfd_render(struct wut_Textfield *txtf);

#endif /* _WUT_WIDGET_TEXTFIELD_H */
