#ifndef _WT_WIDGET_H
#define _WT_WIDGET_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/element.h"

enum wt_widget_type {
	WT_WIDGET_TEXT,
	WT_WIDGET_IMAGE,
	WT_WIDGET_VIEW
};


struct wt_widget {
	/* A pointer to the element */
	struct wt_element *element;

	enum wt_widget_type type;

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
WT_API struct wt_widget *wt_CreateWidget(struct wt_element *ele,
		enum wt_widget_type type, void *data);


/*
 * Destroy a widget and free the allocated memory.
 *
 * @comp: Pointer to the widget
 */
WT_API void wt_DestroyWidget(struct wt_widget *w);


/*
 * Update a widget.
 *
 * @comp: Pointer to the widget
 * @[data]: Pointer to data used for updating
 */
WT_API void wt_UpdateWidget(struct wt_widget *w, void *data);


/*
 * Render a widget.
 *
 * @comp: Pointer to the widget
 */
WT_API void wt_RenderWidget(struct wt_widget *w);

#endif /* _WT_WIDGET_H */
