#ifndef _WT_DOCUMENT_ELEMENT_H
#define _WT_DOCUMENT_ELEMENT_H

struct wt_element;

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/identity.h"
#include "document/inc/element_template.h"

#include "style/inc/style.h"

#include "event/inc/event_listener.h"


#define WT_ELEMENT_NAME_LIM	126
#define WT_ELEMENT_CHILDREN_LIM	126

#define WT_ELEMENT_F_VISIBLE	(1<<0)
#define WT_ELEMENT_F_HOVERED	(1<<1)
#define WT_ELEMENT_F_SELECTED	(1<<2)



enum wt_element_type {
	WT_BODY		= 0,
	WT_BLOCK	= 1,
	WT_TEXT		= 2,
	WT_BUTTON	= 3,
	WT_INPUT	= 4,
	WT_IMAGE	= 5,
	WT_VIEW		= 6,
	WT_CUSTOM	= 7
};


struct wt_element {
	wt_identity_t identity;

	/* The name of the element */
	char name[WT_ELEMENT_NAME_LIM];

	/* The unique identifier for this element */
	u32 id;

	/* The layer this element is on */
	u32 layer;

	/* A pointer to the document this element is part of */
	struct wt_document *document;

	/* A pointer to the body element */
	struct wt_element *body;

	/* The parent element, this element is attached to */
	struct wt_element *parent;

	/* A pointer to the next child in the elements children list */
	struct wt_element *older_sibling;
	struct wt_element *younger_sibling;

	/* The slot of the element in the parent children list */
	s8 slot;

	/* The children that are attached to this element */
	u8 children_num;
	struct wt_element *firstborn;

	/* The type of element */
	enum wt_element_type type;

	/* The style structure for this element */
	struct wt_style style;

	/* The widget used by the element */
	struct wt_widget *widget;

	/*
	 * General info flags for this element, like visiblity and scrollbars.
	 */
	u8 info_flags;

	/*
	 * The relative position in the reference-area dictated by the
	 * layout mode of the parent.
	 */
	struct wt_sin2	layout_offset;

	/* 
	 * The relative offset of the upper-left corner of the parents bounding
	 * rectangle to upper-left corner of the reference-area this elements
	 * bounding box will be located in. This includes the parents padding,
	 * parent scrolling and this elements layout offset. 
	 */
	struct wt_sin2	relative_offset;

	/*
	 * The absolute offset from the upper-left corner of the window to the
	 * upper-left corner of the reference-area this elements bounding box
	 * will be located in. This is calculated by the absolute offset of the
	 * parent element plus the relative offset of this element.
	 */
	struct wt_sin2	absolute_offset;


	/*
	 * The absolute rectangles of the element in the window.
	 */
	struct wt_rect	bounding_rect;
	struct wt_rect	element_rect;	/* -Spacing */
	struct wt_rect  inner_rect;	/* -Spacing, -Border */
	struct wt_rect	content_rect;	/* -Spacing, -Border, -Padding */

	

	/*
	 * And this is the output rectangle. It defines the absolute position
	 * and size of the element in the window and specifies how the element
	 * will be drawn on the screen.
	 */
	struct wt_rect	output_rect;

	/*
	 * This rectangle defines the size and offset of the visible part of
	 * this element for input. This will be used to calculate the UV-coords
	 * for input images.
	 */
	struct wt_rect visible_in_rect;

	/*
	 * This rectangle defines the absolute position and size of the visible
	 * part of this element in the window.
	 */
	struct wt_rect	visible_out_rect;


	/* These properties are used for scrolling */
	struct wt_sin2 	content_size;
	struct wt_sin2	content_offset;

	/* This flag indicates which scrollbars to render */
	s32 scrollbar_flags;


	struct wt_event_handler *event_handler;
};

/*
 * The default callback function for all higher-level-function in relation with
 * elements.
 */
typedef s8 (*wt_ele_cfnc)(struct wt_element *, void *);

#endif /* _WT_DOCUMENT_ELEMENT_H */
