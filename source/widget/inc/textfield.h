#ifndef _FH_WIDGET_TEXTFIELD_H
#define _FH_WIDGET_TEXTFIELD_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/font.h"

struct fh_textfield {
	/* The font used for rendering */
	struct fh_font *font;

	/* A pointer to the element */
	struct fh_element *element;

	/* The content of the textfield */
	char content[256];
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
 * Render a textfield onto the screen.
 */
FH_API void fh_textfield_render(struct fh_textfield *txt);

#endif /* _FH_WIDGET_TEXTFIELD_H */
