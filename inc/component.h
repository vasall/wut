#ifndef _FH_ELEMENT_COMPONENT_H
#define _FH_ELEMENT_COMPONENT_H

#include "stdinc.h"
#include "element.h"

enum fh_component_type {
	FH_COMPONENT_VIEW
};


struct fh_component {
	/* A pointer to the element */
	struct fh_element *element;

	enum fh_component_type type;

	void *ref;
};


/*
 * Create and initialize a new component.
 *
 * @ele: Pointer to the element
 * @type: The type of component to create
 *
 * Returns: Either a pointer to the component or NULL if an error occurred
 */
FH_API struct fh_component *fh_CreateComponent(struct fh_element *ele,
		enum fh_component_type type);


/*
 * Destroy a component and free the allocated memory.
 *
 * @comp: Pointer to the component
 */
FH_API void fh_DestroyComponent(struct fh_component *comp);


/*
 * Update a component.
 *
 * @comp: Pointer to the component
 * @[data]: Pointer to data used for updating
 */
FH_API void fh_UpdateComponent(struct fh_component *comp, void *data);


/*
 * Render a component.
 *
 * @comp: Pointer to the component
 */
FH_API void fh_RenderComponent(struct fh_component *comp);

#endif /* _FH_ELEMENT_COMPONENT_H */
