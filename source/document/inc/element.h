#ifndef _FH_ELEMENT_H
#define _FH_ELEMENT_H

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
	 * The relative offset of the upper-left corner of the parents bounding
	 * rectangle to upper-left corner of the reference-area this elements
	 * bounding box will be located in.
	 */
	struct fh_sin2	relative_offset;

	/*
	 * The absolute offset from the upper-left corner of the window to the
	 * upper-left corner of the reference-area this elements bounding box
	 * will be located in.
	 */
	struct fh_sin2	absolute_offset;

	/*
	 * The relative position in this reference-area dictated by the
	 * requested layout mode.
	 */
	struct fh_sin2	layout_offset;

	/*
	 * This is the input rectangle which will be used when rendering for
	 * example images. It defines the offset and size for the input resource.
	 */
	struct fh_rect	input_rect;

	/*
	 * General info flags for this element.
	 */
	u8 info_flags;

	/*
	 * And this is the output rectangle. It defines the absolute position
	 * and size of the element in the window and specifies how the element
	 * will be drawn on the screen.
	 */
	struct fh_rect	output_rect;


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

#endif /* _FH_ELEMENT_H */
