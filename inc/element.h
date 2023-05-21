#ifndef _FH_ELEMENT_H
#define _FH_ELEMENT_H


#include "define.h"
#include "datatype.h"
#include "import.h"
#include "element_style.h"


#define FH_ELEMENT_NAME_LIM	126
#define FH_ELEMENT_CHILDREN_LIM	126


enum fh_element_type {
	FH_BODY,
	FH_BLOCK,
	FH_TEXT,
	FH_BUTTON,
	FH_INPUT,
	FH_IMAGE,
	FH_CANVAS,
	FH_CUSTOM
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
};



/*
 * Allocate and create a new element. Then depending on the given type, load
 * template styles.
 *
 * @name: The name of the element
 * @type: The type of element
 *
 * Returns: Either a pointer to the newly created element or NULL if an error
 * 	    occurred
 */
FH_API struct fh_element *fh_CreateElement(char *name,
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
 * Get the shape of the element in the window.
 *
 * @ele: Pointer to the element
 *
 * Returns: The shape of the element
 */
FH_API struct fh_rect fh_GetElementShape(struct fh_element *ele);


#endif /* _FH_ELEMENT_H */
