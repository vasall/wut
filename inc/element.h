#ifndef _FH_ELEMENT_H
#define _FH_ELEMENT_H


#include "define.h"
#include "datatype.h"
#include "import.h"
#include "element_style.h"


#define FH_ELEMENT_NAME_LIM	126
#define FH_ELEMENT_CHILDREN_LIM	126


enum fh_element_type {
	FH_BODY		= 0,
	FH_BLOCK	= 1,
	FH_TEXT		= 2,
	FH_BUTTON	= 3,
	FH_INPUT	= 4,
	FH_IMAGE	= 5,
	FH_CANVAS	= 6,
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

	/* The children elements that are attached to this element */
	u8 children_num;
	struct fh_element *children[FH_ELEMENT_CHILDREN_LIM];

	/* The type of element */
	enum fh_element_type type;

	/* The style structure for this element */
	struct fh_style style;

	/* The component used by the element */
	struct fh_component *component;
};



/*
 * Allocate and create a new element. Then depending on the given type, load
 * template styles.
 *
 * @doc: Pointer to the document
 * @name: The name of the element
 * @type: The type of element
 *
 * Returns: Either a pointer to the newly created element or NULL if an error
 * 	    occurred
 */
FH_API struct fh_element *fh_CreateElement(struct fh_document *doc, char *name,
		enum fh_element_type type);


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
 * @data: A datapointer to be passed to every function pass
 */
FH_API void fh_ApplyElementsDown(struct fh_element *ele,
		s8 (*fnc)(struct fh_element *w, void *data), void *data);


/*
 * Apply a function recursivly upwards starting from <ele>.
 *
 * @ele: The starting element
 * @fnc: The function to apply to all elements
 * @data: A datapointer to tbe passed to every function pass
 */
FH_API void fh_ApplyElementsUp(struct fh_element *ele,
		s8 (*fnc)(struct fh_element *w, void *data), void *data);


/*
 * Update the element, update the shape and then finally reapply the style.
 *
 * @ele: Pointer to the element
 */
FH_API void fh_UpdateElement(struct fh_element *ele);


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
 * Get the shape of the element in the window.
 *
 * @ele: Pointer to the element
 *
 * Returns: The shape of the element
 */
FH_API struct fh_rect fh_GetElementShape(struct fh_element *ele);


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
