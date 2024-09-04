#ifndef _WUT_DOCUMENT_ELEMENT_H
#define _WUT_DOCUMENT_ELEMENT_H

struct wut_Element;

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "document/inc/identity.h"
#include "document/inc/tag.h"
#include "document/inc/element_template.h"

#include "style/inc/style.h"

#include "event/inc/event_listener.h"


#define WUT_ELE_NAME_LIM	        126
#define WUT_ELE_CHILDREN_LIM	        126

#define WUT_ELE_F_VISIBLE	        (1<<0)
#define WUT_ELE_F_HOVERED	        (1<<1)
#define WUT_ELE_F_SELECTED	        (1<<2)




struct wut_Element {
        u8                      identity;

        /* The name of the element */
        char                    name[WUT_ELE_NAME_LIM];

        /* The unique identifier for this element */
        u32                     id;

        /* The layer this element is on */
        u32                     layer;

        /* A pointer to the document this element is part of */
        struct wut_Document     *document;

        /* A pointer to the body element */
        struct wut_Element      *body;

        /* The parent element, this element is attached to */
        struct wut_Element      *parent;

        /* A pointer to the next child in the elements children list */
        struct wut_Element      *left;
        struct wut_Element      *right;

        /* The slot of the element in the parent children list */
        s8                      slot;

        /* The children that are attached to this element */
        u8                      children_num;
        struct wut_Element      *firstborn;

        /* The type of element */
        enum wut_eTag   type;

        /* The style structure for this element */
        struct wut_Style        style;

        /* The widget used by the element */
        struct wut_Widget       *widget;

        /*
         * General info flags for this element, like visiblity and scrollbars.
         */
        u8                      info_flags;

        /*
         * The relative position in the reference-area dictated by the
         * layout mode of the parent.
         */
        wut_iVec2	        layout_offset;

        /* 
         * The relative offset of the upper-left corner of the parents bounding
         * rectangle to upper-left corner of the reference-area this elements
         * bounding box will be located in. This includes the parents padding,
         * parent scrolling and this elements layout offset. 
         */
        wut_iVec2               relative_offset;

        /*
         * The absolute offset from the upper-left corner of the window to the
         * upper-left corner of the reference-area this elements bounding box
         * will be located in. This is calculated by the absolute offset of the
         * parent element plus the relative offset of this element.
         */
        wut_iVec2	        absolute_offset;


        /*
         * The absolute rectangles of the element in the window.
         */
        wut_iRect	        bounding_rect;
        wut_iRect	        element_rect;	/* Spacing */
        wut_iRect               inner_rect;	/* Spacing, Border */
        wut_iRect	        content_rect;	/* Spacing, Border, Padding */



        /*
         * And this is the output rectangle. It defines the absolute position
         * and size of the element in the window and specifies how the element
         * will be drawn on the screen.
         */
        wut_iRect	        output_rect;

        /*
         * This rectangle defines the size and offset of the visible part of
         * this element for input. This will be used to calculate the UV-coords
         * for input images.
         */
        wut_iRect               visible_in_rect;

        /*
         * This rectangle defines the absolute position and size of the visible
         * part of this element in the window.
         */
        wut_iRect	        visible_out_rect;


        /* These properties are used for scrolling */
        wut_iVec2 	        content_size;
        wut_iVec2               content_offset;

        /* This flag indicates which scrollbars to render */
        s32                     scrollbar_flags;


        struct wut_EventHandler *event_handler;
};

/*
 * The default callback function for all higher-level-function in relation with
 * elements.
 */
typedef s8 (*wut_ElementFunc)(struct wut_Element *, void *);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_XMOD void wut_ele_adjust_shape(struct wut_Element *ele);


/*
 * This function will set the scrollbar flag.
 *
 * @ele: Pointer to the element
 */
WUT_XMOD void wut_ele_hdl_scrollbar(struct wut_Element *ele);


/*
 * Check if the two element pointers point to the same element.
 *
 * @in1: First element pointer
 * @in2: Second element pointer
 *
 * Returns: 1 if they are equal, 0 if not
 */
WUT_XMOD s8 wut_ele_compare(struct wut_Element *in1, struct wut_Element *in2);


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
WUT_XMOD s8 wut_ele_hlf(struct wut_Element *ele, wut_ElementFunc prefnc,
		wut_ElementFunc postfnc, void *data);


/*
 * Modify an info flag of an element.
 *
 * @ele: Pointer to the element
 * @flag: The flag to modify
 * @val: The new value of the flag
 */
WUT_XMOD void wut_ele_mod_info(struct wut_Element *ele, u8 flag, u8 val);


/*
 * Scroll the element on a given axis by a certain value. This function will
 * respect the limits of the scrollable area.
 *
 * @ele: Pointer to the element
 * @val: The value to scroll by
 *
 * Returns: 1 if scrolling was successfull or 0 if not
 */
WUT_XMOD s8 wut_ele_scroll(struct wut_Element *ele, s32 *val);


/*
 * Render an element onto a flat struct using the given renderer.
 *
 * @ren: Pointer to the batch renderer
 * @ele: Pointer to the element
 */
WUT_XMOD void wut_ele_render(struct wut_Batch *ren, struct wut_Element *ele);


/*
 * Render the scrollbar of the element. This feature is configure through the
 * scrollbar-setting in the stylesheet.
 *
 * @ren: Pointer to the batch renderer
 * @ele: Pointer to the element
 */
WUT_XMOD void wut_ele_ren_scrollbar(struct wut_Batch *ren,
                struct wut_Element *ele);




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
WUT_API struct wut_Element *wut_CreateElement(struct wut_Document *doc, char *name,
		enum wut_eTag type, void *data);


/*
 * Destroy a element and free the allocated memory.
 *
 * @ele: Pointer to the element to destroy
 */
WUT_API void wut_DestroyElement(struct wut_Element *ele);


/*
 * Attach a given node to the specified parent element.
 *
 * @parent: Pointer to the parent element
 * @ele: The element to attach to the parent
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_AttachElement(struct wut_Element *parent, struct wut_Element *ele);


/*
 * Detach the given element from its parent. This function is save to call even
 * if the element doesn't have a parent.
 */
WUT_API void wut_DetachElement(struct wut_Element *ele);


/*
 * Recursivly apply the given function to all elements upwards starting from the
 * given one.
 *
 * @ele: Pointer to the starting element
 * @fnc: The function to apply to the elements
 * @void: A datapointer to be passed to every function pass
 */
WUT_API void wut_ApplyElementRise(struct wut_Element *ele, wut_ElementFunc fnc,
		void *data);


/*
 * Update the element, update the shape and then finally reapply the style.
 *
 * @ele: Pointer to the element
 */
WUT_API void wut_UpdateElementStyle(struct wut_Element *ele);


/*
 * This function will adjust the position of the children elements relative to
 * the parent element and copy the size defined through the stylesheet.
 *
 * @ele: Pointer to the element
 */
WUT_API void wut_UpdateElementChildrenShape(struct wut_Element *ele);


/*
 * Get the context used for rendering the element.
 *
 * @ele: Pointer to the element
 *
 * Returns: Pointer to the context
 */
WUT_API struct wut_Context *wut_GetElementContext(struct wut_Element *ele);


/*
 * Get the size and position of the bounding box in the window in pixels.
 *
 * @ele: Pointer to the element
 * @out: The rectangle to write the absolute shape of the bounding box to
 */
WUT_API void wut_GetBoundingBox(struct wut_Element *ele, wut_iRect out);


/*
 * Get the size and position of the rectangle containing the element in
 * pixels.
 *
 * @ele: Pointer to the element
 * @out: The rectangle to write the absolute shape of the element box to
 */
WUT_API void wut_GetElementBox(struct wut_Element *ele, wut_iRect out);


/*
 * Get the size and position of the content rectangle inside the element in
 * pixels.
 *
 * @ele: Pointer to the element
 * @out: The rectangle to write the absolute shape of the content box to
 */
WUT_API void wut_GetContentBox(struct wut_Element *ele, wut_iRect out);


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
WUT_API s8 wut_ModifyElementStyle(struct wut_Element *ele, char *str);


/*
 * Get the view from a canvas element, so it can be used for rendering.
 *
 * @ele: Pointer to the element
 *
 * Returns: Either a pointer to the view struct or NULL if an error occurred
 */
WUT_API struct wut_View *wut_GetView(struct wut_Element *ele);


#endif /* _WUT_DOCUMENT_ELEMENT_H */
