#ifndef _FH_ELEMENT_H
#define _FH_ELEMENT_H


#include "define.h"
#include "import.h"
#include "element_style.h"
#include "renderer.h"


#define FH_ELEMENT_NAME_LIM	126
#define FH_ELEMENT_CHILDREN_LIM	126


enum fh_element_type {
	FH_BODY,
	FH_BLOCK,
	FH_TEXT,
	FH_BUTTON,
	FH_INPUT,
	FH_IMAGE,
	FH_OPENGL,
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
FH_API struct fh_element *fh_ele_create(char *name, enum fh_element_type type);


/*
 * Destroy a element and free the allocated memory.
 *
 * @ele: Pointer to the element to destroy
 */
FH_API void fh_ele_destroy(struct fh_element *ele);


/*
 * Attach a given node to the specified parent element.
 *
 * @parent: Pointer to the parent element
 * @ele: The element to attach to the parent
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ele_attach(struct fh_element *parent, struct fh_element *ele);


/*
 * Detach the given element from its parent. This function is save to call even
 * if the element doesn't have a parent.
 */
FH_API void fh_ele_detach(struct fh_element *ele);


/*
 * This function will call both fh_ele_detach() and fh_ele_destroy().
 *
 * @ele: Pointer to the element to remove
 */
FH_API void fh_ele_remove(struct fh_element *ele);


/*
 * This function is a higher-level-function which will apply the cfnc-function to
 * all elements starting from the str element downwards.
 *
 * Note that if the callback function returns 1, the recursion will stop.
 *
 * @str: The active element struct, also the one at the top
 * @cfnc: The callback-function to execute to the elements
 * @data: A data pointer which will be passed to every function call
 */
FH_API void fh_ele_hlf_down(struct fh_element *str,
		s8 (*cfnc)(struct fh_element *w, void *data), void *data);


/*
 * This function is a higher-level-function which will apply the cfnc-function to
 * all window structs from the lowest element below start, up to the
 * the str element struct itself.
 *
 * Note that if the callback function returns 1, the recursion will stop.
 *
 * @str: The active element struct, also the one at the top
 * @cfnc: The callback-function to execute to all elements
 * @data: A data pointer which will be passed to every function call
 */
FH_API void fh_ele_hlf_up(struct fh_element *str,
		s8 (*cfnc)(struct fh_element *w, void *data), void *data);



/*
 * Update the element, update the shape and then finally reapply the style.
 *
 * @ele: Pointer to the element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ele_update(struct fh_element *ele);


/*
 * Render an element onto a canvas using the given renderer.
 *
 * @ele: Pointer to the element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_ele_render(struct fh_element *ele);



#endif /* _FH_ELEMENT_H */
