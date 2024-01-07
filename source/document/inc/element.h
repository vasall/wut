#ifndef _FH_DOCUMENT_ELEMENT_H
#define _FH_DOCUMENT_ELEMENT_H

struct fh_element;

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/identity.h"

#include "style/inc/style.h"
#include "style/inc/template.h"

#include "event/inc/event_listener.h"


#define FH_ELEMENT_NAME_LIM	126
#define FH_ELEMENT_CHILDREN_LIM	126

#define FH_ELEMENT_F_VISIBLE	(1<<0)
#define FH_ELEMENT_F_HOVERED	(1<<1)
#define FH_ELEMENT_F_SELECTED	(1<<2)



enum fh_element_type {
	FH_BODY		= 0,
	FH_BLOCK	= 1,
	FH_TEXT		= 2,
	FH_BUTTON	= 3,
	FH_INPUT	= 4,
	FH_IMAGE	= 5,
	FH_VIEW		= 6,
	FH_CUSTOM	= 7
};


struct fh_element {
	u8 identifier;

	/* The name of the element */
	char name[FH_ELEMENT_NAME_LIM];

	/* The unique identifier for this element */
	u32 id;

	/* The layer this element is on */
	u32 layer;

	/* A pointer to the document this element is part of */
	struct fh_document *document;

	/* A pointer to the body element */
	struct fh_element *body;

	/* The parent element, this element is attached to */
	struct fh_element *parent;

	/* A pointer to the next child in the elements children list */
	struct fh_element *older_sibling;
	struct fh_element *younger_sibling;

	/* The slot of the element in the parent children list */
	s8 slot;

	/* The children that are attached to this element */
	u8 children_num;
	struct fh_element *firstborn;

	/* The type of element */
	enum fh_element_type type;

	/* The style structure for this element */
	struct fh_style style;

	/* The widget used by the element */
	struct fh_widget *widget;

	/*
	 * General info flags for this element, like visiblity and scrollbars.
	 */
	u8 info_flags;

	/*
	 * The relative position in the reference-area dictated by the
	 * layout mode of the parent.
	 */
	struct fh_sin2	layout_offset;

	/* 
	 * The relative offset of the upper-left corner of the parents bounding
	 * rectangle to upper-left corner of the reference-area this elements
	 * bounding box will be located in. This includes the parents padding,
	 * parent scrolling and this elements layout offset. 
	 */
	struct fh_sin2	relative_offset;

	/*
	 * The absolute offset from the upper-left corner of the window to the
	 * upper-left corner of the reference-area this elements bounding box
	 * will be located in. This is calculated by the absolute offset of the
	 * parent element plus the relative offset of this element.
	 */
	struct fh_sin2	absolute_offset;


	/*
	 * The absolute rectangles of the element in the window.
	 */
	struct fh_rect	bounding_box;
	struct fh_rect	element_box;	/* -Spacing */
	struct fh_rect	content_box;	/* -Spacing, -Border, -Padding */

	

	/*
	 * And this is the output rectangle. It defines the absolute position
	 * and size of the element in the window and specifies how the element
	 * will be drawn on the screen.
	 */
	struct fh_rect	output_rect;

	/*
	 * This rectangle defines the size and offset of the visible part of
	 * this element for input. This will be used to calculate the UV-coords
	 * for input images.
	 */
	struct fh_rect visible_in_rect;

	/*
	 * This rectangle defines the absolute position and size of the visible
	 * part of this element in the window.
	 */
	struct fh_rect	visible_out_rect;


	/* These properties are used for scrolling */
	struct fh_sin2 	content_size;
	struct fh_sin2	content_offset;

	/* This flag indicates which scrollbars to render */
	s32 scrollbar_flags;


	struct fh_event_handler *event_handler;
};

/*
 * The default callback function for all higher-level-function in relation with
 * elements.
 */
typedef s8 (*fh_ele_cfnc)(struct fh_element *, void *);

#endif /* _FH_DOCUMENT_ELEMENT_H */
