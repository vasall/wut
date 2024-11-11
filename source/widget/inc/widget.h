#ifndef _WUT_WIDGET_H
#define _WUT_WIDGET_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/document/inc/element.h"

enum wut_eWidgetType {
	WUT_WIDGET_TEXT,
	WUT_WIDGET_IMAGE,
	WUT_WIDGET_VIEW
};


struct wut_Widget {
	/* A pointer to the element */
	struct wut_Element *element;

	enum wut_eWidgetType type;

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
WUT_API struct wut_Widget *wut_CreateWidget(struct wut_Element *ele,
		enum wut_eWidgetType type, void *data);


/*
 * Destroy a widget and free the allocated memory.
 *
 * @comp: Pointer to the widget
 */
WUT_API void wut_DestroyWidget(struct wut_Widget *w);


/*
 * Update a widget.
 *
 * @comp: Pointer to the widget
 * @[data]: Pointer to data used for updating
 */
WUT_API void wut_UpdateWidget(struct wut_Widget *w, void *data);


/*
 * Render a widget.
 *
 * @comp: Pointer to the widget
 */
WUT_API void wut_RenderWidget(struct wut_Widget *w);

#endif /* _WUT_WIDGET_H */
