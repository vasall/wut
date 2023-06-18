#ifndef _FH_ELEMENT_WIDGET_H
#define _FH_ELEMENT_WIDGET_H

#include "stdinc.h"
#include "element.h"

enum fh_widget_type {
	FH_WIDGET_TEXT,
	FH_WIDGET_IMAGE,
	FH_WIDGET_VIEW
};


struct fh_widget {
	/* A pointer to the element */
	struct fh_element *element;

	enum fh_widget_type type;

	void *ref;
};


/*
 * Create and initialize a new widget.
 *
 * @ele: Pointer to the element
 * @type: The type of widget to create
 * @[data]: Data used for the creation of some widgets
 *
 * Returns: Either a pointer to the widget or NULL if an error occurred
 */
FH_API struct fh_widget *fh_CreateWidget(struct fh_element *ele,
		enum fh_widget_type type, void *data);


/*
 * Destroy a widget and free the allocated memory.
 *
 * @comp: Pointer to the widget
 */
FH_API void fh_DestroyWidget(struct fh_widget *w);


/*
 * Update a widget.
 *
 * @comp: Pointer to the widget
 * @[data]: Pointer to data used for updating
 */
FH_API void fh_UpdateWidget(struct fh_widget *w, void *data);


/*
 * Render a widget.
 *
 * @comp: Pointer to the widget
 */
FH_API void fh_RenderWidget(struct fh_widget *w);

#endif /* _FH_ELEMENT_WIDGET_H */
