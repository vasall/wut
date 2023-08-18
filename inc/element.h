#ifndef _FH_ELEMENT_H
#define _FH_ELEMENT_H


#include "define.h"
#include "datatype.h"
#include "import.h"
#include "style.h"


#define FH_ELEMENT_NAME_LIM	126
#define FH_ELEMENT_CHILDREN_LIM	126


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
	 * This flag indicated if the element is visible or not.
	 * Will be used during rendering.
	 */
	s8 is_visible;

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
};

/*
 * The default callback function for all higher-level-function in relation with
 * elements.
 */
typedef s8 (*fh_ele_cfnc)(struct fh_element *, void *);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_CROSS void fh_ele_adjust_shape(struct fh_element *ele);


/*
 * Calculate the relative offset of this elements bounding box to its parents
 * bounding box. Or if there is no parent the offset from the upper-left corner
 * of the window to the bounding box of this element.
 * Then extend it so that it contains the absolute offset from the windows
 * upper-left corner to the upper-left corner of the element. This will
 * also include the scrolling of all above elements plus layouting.
 *
 * @ele: Pointer to the element
 */
FH_CROSS void fh_ele_calc_off(struct fh_element *ele);


/*
 * Calculate both the input and ouput rectangles used for rendering.
 *
 * @ele: Pointer to the element
 */
FH_CROSS void fh_ele_calc_render_rect(struct fh_element *ele);


/*
 * This function will set the scrollbar flag.
 *
 * @ele: Pointer to the element
 */
FH_CROSS void fh_ele_hdl_scrollbar(struct fh_element *ele);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Allocate and create a new element. Then depending on the given type, load
 * template styles.
 *
 * @doc: Pointer to the document
 * @name: The name of the element
 * @type: The type of element
 * @[data]: Optional data
 *  
 * Returns: Either a pointer to the newly created element or NULL if an error
 * 	    occurred
 */
FH_API struct fh_element *fh_CreateElement(struct fh_document *doc, char *name,
		enum fh_element_type type, void *data);


/*
 * Destroy a element and free the allocated memory.
 *
 * @ele: Pointer to the element to destroy
 */
FH_API void fh_DestroyElement(struct fh_element *ele);


/*
 * Attach a given node to the specified parent element.
 *
 * @parent: Pointer to the parent element
 * @ele: The element to attach to the parent
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_AttachElement(struct fh_element *parent, struct fh_element *ele);


/*
 * Detach the given element from its parent. This function is save to call even
 * if the element doesn't have a parent.
 */
FH_API void fh_DetachElement(struct fh_element *ele);


/*
 * Apply a function recursivly downwards starting from <ele>.
 *
 * Note that if the callback function returns 1, the recursion will stop.
 *
 * @ele: The starting element
 * @fnc: The function to apply to all elements
 * @post: The post function to call after the children have been handled
 * @data: A datapointer to be passed to every function pass
 */
FH_API void fh_ApplyElementsDown(struct fh_element *ele,
		fh_ele_cfnc fnc, fh_ele_cfnc post, void *data);


/*
 * Apply a function recursivly upwards starting from <ele>.
 *
 * @ele: The starting element
 * @fnc: The function to apply to all elements
 * @data: A datapointer to tbe passed to every function pass
 */
FH_API void fh_ApplyElementsUp(struct fh_element *ele,
		fh_ele_cfnc fnc, void *data);


/*
 * Update the element, update the shape and then finally reapply the style.
 *
 * @ele: Pointer to the element
 */
FH_API void fh_UpdateElementStyle(struct fh_element *ele);


/*
 * This function will adjust the position of the children elements relative to
 * the parent element and copy the size defined through the stylesheet.
 *
 * @ele: Pointer to the element
 */
FH_API void fh_UpdateElementChildrenShape(struct fh_element *ele);


/*
 * Render an element onto a canvas using the given renderer.
 *
 * @ele: Pointer to the element
 */
FH_API void fh_RenderElement(struct fh_element *ele);


/*
 * Get the context used for rendering the element.
 *
 * @ele: Pointer to the element
 *
 * Returns: Pointer to the context
 */
FH_API struct fh_context *fh_GetElementContext(struct fh_element *ele);


/*
 * Get the size and position of the bounding box in the window in pixels.
 *
 * @ele: Pointer to the element
 *
 * Returns: The absolute shape of the bounding box
 */
FH_API struct fh_rect fh_GetBoundingBox(struct fh_element *ele);


/*
 * Get the size and position of the rectangle containing the element in
 * pixels.
 *
 * @ele: Pointer to the element
 *
 * Returns: The absolute shape of the element box
 */
FH_API struct fh_rect fh_GetElementBox(struct fh_element *ele);


/*
 * Get the size and position of the content rectangle inside the element in
 * pixels.
 *
 * @ele: Pointer to the element
 *
 * Returns: The absolute shape of the content box
 */
FH_API struct fh_rect fh_GetContentBox(struct fh_element *ele);


/*
 * Modify the stylesheet of an element.
 *
 * For example: "vposition: 10px;" or "infill-color: #ff00ff00;"
 * This function can even take multiple changes at once, as long as they are
 * seperated by a semicolon.
 *
 * @ele: Pointer to the element
 * @str: A string containing the instructions
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ModifyElementStyle(struct fh_element *ele, char *str);


/*
 * Get the view from a canvas element, so it can be used for rendering.
 *
 * @ele: Pointer to the element
 *
 * Returns: Either a pointer to the view struct or NULL if an error occurred
 */
FH_API struct fh_view *fh_GetView(struct fh_element *ele);

#endif /* _FH_ELEMENT_H */
