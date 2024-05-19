#ifndef _WT_DOCUMENT_ELEMENT_MANAGER_H
#define _WT_DOCUMENT_ELEMENT_MANAGER_H

#include "document/inc/element.h"


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_XMOD void wt_element_adjust_shape(struct wt_element *ele);


/*
 * This function will set the scrollbar flag.
 *
 * @ele: Pointer to the element
 */
WT_XMOD void wt_element_hdl_scrollbar(struct wt_element *ele);


/*
 * Check if the two element pointers point to the same element.
 *
 * @in1: First element pointer
 * @in2: Second element pointer
 *
 * Returns: 1 if they are equal, 0 if not
 */
WT_XMOD s8 wt_element_compare(struct wt_element *in1, struct wt_element *in2);


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
WT_XMOD s8 wt_element_hlf(struct wt_element *ele, wt_ele_cfnc prefnc,
		wt_ele_cfnc postfnc, void *data);


/*
 * Modify an info flag of an element.
 *
 * @ele: Pointer to the element
 * @flag: The flag to modify
 * @val: The new value of the flag
 */
WT_XMOD void wt_element_mod_info(struct wt_element *ele, u8 flag, u8 val);


/*
 * Scroll the element on a given axis by a certain value. This function will
 * respect the limits of the scrollable area.
 *
 * @ele: Pointer to the element
 * @val: The value to scroll by
 *
 * Returns: 1 if scrolling was successfull or 0 if not
 */
WT_XMOD s8 wt_element_scroll(struct wt_element *ele, s32 *val);


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
WT_API struct wt_element *wt_CreateElement(struct wt_document *doc, char *name,
		enum wt_element_type type, void *data);


/*
 * Destroy a element and free the allocated memory.
 *
 * @ele: Pointer to the element to destroy
 */
WT_API void wt_DestroyElement(struct wt_element *ele);


/*
 * Attach a given node to the specified parent element.
 *
 * @parent: Pointer to the parent element
 * @ele: The element to attach to the parent
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_AttachElement(struct wt_element *parent, struct wt_element *ele);


/*
 * Detach the given element from its parent. This function is save to call even
 * if the element doesn't have a parent.
 */
WT_API void wt_DetachElement(struct wt_element *ele);


/*
 * Recursivly apply the given function to all elements upwards starting from the
 * given one.
 *
 * @ele: Pointer to the starting element
 * @fnc: The function to apply to the elements
 * @void: A datapointer to be passed to every function pass
 */
WT_API void wt_ApplyElementRise(struct wt_element *ele, wt_ele_cfnc fnc,
		void *data);


/*
 * Update the element, update the shape and then finally reapply the style.
 *
 * @ele: Pointer to the element
 */
WT_API void wt_UpdateElementStyle(struct wt_element *ele);


/*
 * This function will adjust the position of the children elements relative to
 * the parent element and copy the size defined through the stylesheet.
 *
 * @ele: Pointer to the element
 */
WT_API void wt_UpdateElementChildrenShape(struct wt_element *ele);


/*
 * Get the context used for rendering the element.
 *
 * @ele: Pointer to the element
 *
 * Returns: Pointer to the context
 */
WT_API struct wt_context *wt_GetElementContext(struct wt_element *ele);


/*
 * Get the size and position of the bounding box in the window in pixels.
 *
 * @ele: Pointer to the element
 *
 * Returns: The absolute shape of the bounding box
 */
WT_API struct wt_rect wt_GetBoundingBox(struct wt_element *ele);


/*
 * Get the size and position of the rectangle containing the element in
 * pixels.
 *
 * @ele: Pointer to the element
 *
 * Returns: The absolute shape of the element box
 */
WT_API struct wt_rect wt_GetElementBox(struct wt_element *ele);


/*
 * Get the size and position of the content rectangle inside the element in
 * pixels.
 *
 * @ele: Pointer to the element
 *
 * Returns: The absolute shape of the content box
 */
WT_API struct wt_rect wt_GetContentBox(struct wt_element *ele);


/*
 * Get a reference pointer to the content box for the element.
 *
 * @ele: Pointer to the element
 *
 * Returns: A pointer to the content ractangle struct or NULL if an error
 * 	    occurred
 */
WT_API struct wt_rect *wt_GetContextBoxRef(struct wt_element *ele);

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
WT_API s8 wt_ModifyElementStyle(struct wt_element *ele, char *str);


/*
 * Get the view from a canvas element, so it can be used for rendering.
 *
 * @ele: Pointer to the element
 *
 * Returns: Either a pointer to the view struct or NULL if an error occurred
 */
WT_API struct wt_view *wt_GetView(struct wt_element *ele);


#endif /* _WT_DOCUMENT_ELEMENT_MANAGER_H */
