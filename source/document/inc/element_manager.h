#ifndef _FH_ELEMENT_MANAGER_H
#define _FH_ELEMENT_MANAGER_H

#include "document/inc/element.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_XMOD void fh_element_adjust_shape(struct fh_element *ele);


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
FH_XMOD void fh_element_calc_off(struct fh_element *ele);


/*
 * Calculate both the input and ouput rectangles used for rendering.
 *
 * @ele: Pointer to the element
 */
FH_XMOD void fh_element_calc_render_rect(struct fh_element *ele);


/*
 * This function will set the scrollbar flag.
 *
 * @ele: Pointer to the element
 */
FH_XMOD void fh_element_hdl_scrollbar(struct fh_element *ele);


/*
 * Render an element onto a flat struct using the given renderer.
 *
 * @ele: Pointer to the element
 */
FH_API void fh_element_render(struct fh_element *ele);


/*
 * Render the scrollbar of the element. This feature is configure through the
 * scrollbar-setting in the stylesheet.
 *
 * @ele: Pointer to the element
 */
FH_XMOD void fh_element_ren_scrollbar(struct fh_element *ele);


/*
 * Check if the two element pointers point to the same element.
 *
 * @in1: First element pointer
 * @in2: Second element pointer
 *
 * Returns: 1 if they are equal, 0 if not
 */
FH_XMOD s8 fh_element_compare(struct fh_element *in1, struct fh_element *in2);


/*
 * Recursivly apply the given functions to all elements in the element tree.
 *
 * @ele: The starting element/The current element
 * @prefnc: The pre-function
 * @postfnc: The post-function
 * @data: A pointer which will be passed to every function call
 *
 * Returns: 1 to stop the recursion, 0 to continue
 */
FH_XMOD s8 fh_element_hlf(struct fh_element *ele, fh_ele_cfnc prefnc,
		fh_ele_cfnc postfnc, void *data);


/*
 * Modify an info flag of an element.
 *
 * @ele: Pointer to the element
 * @flag: The flag to modify
 * @val: The new value of the flag
 */
FH_XMOD void fh_element_mod_info(struct fh_element *ele, u8 flag, u8 val);


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
 * Recursivly apply the given function to all elements upwards starting from the
 * given one.
 *
 * @ele: Pointer to the starting element
 * @fnc: The function to apply to the elements
 * @void: A datapointer to be passed to every function pass
 */
FH_API void fh_ApplyElementRise(struct fh_element *ele, fh_ele_cfnc fnc,
		void *data);


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




#endif /* _FH_ELEMENT_MANAGER_H */
