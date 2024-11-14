#ifndef _WUT_DOCUMENT_H
#define _WUT_DOCUMENT_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/window/inc/window.h"

#include "source/document/inc/element.h"
#include "source/document/inc/loader.h"

#include "source/graphic/inc/batch.h"
#include "source/graphic/inc/object.h"
#include "source/graphic/inc/constructor.h"
#include "source/graphic/inc/flat.h"

#include "source/style/inc/class.h"

#define WUT_CHANGE_STYLE        (1<<0)
#define WUT_CHANGE_ELEMENT      (1<<1)

#define WUT_REGULAR_LENGTH      16

#define WUT_LOW                 1
#define WUT_MIDDLE              2
#define WUT_HIGH                3
#define WUT_VERYHIGH            4

struct wut_DocumentTrackTable {
        /* 
         * This flag indicates if the document has changed, and the element to
         * update from. This will reduce the times the document will be updated.
         */
        s8                              has_changed;
        struct wut_Element              *update_element;

        /*
         * Pointer to the element currently selected and the element the cursor
         * is currently hovering over.
         */
        struct wut_Element		*selected;
        struct wut_Element		*hovered;
};

struct wut_Document {
        /* A pointer to the window this document belongs to */
        struct wut_Window 		*window;

        /* A pointer to the context */
        struct wut_Context 		*context;

        /* The body element, to which all future elements will be attached */
        struct wut_Element 		*body;

        /* All available style classes for this document */
        struct wut_ClassTable           *class_table;

        /* The activity table containing all active elements */
        struct wut_DocumentTrackTable   track_table;

        /* A reference to the window size */
        wut_iRect 		        *shape_ref;

        /* A list of all views */
        struct wut_ViewList 		*views;

        /* The main batch render */
        s16 				batch_id;
        s16                             scroll_id;
};

struct wut_ElementSelector {
        s8 			state;
        char 			*name;
        wut_iVec2 	        pos;
        struct wut_Element 	*element;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Create a new document.
 *
 * @win: Pointer to the window
 *
 * Returns: Either a pointer to a new document or NULL if an error occurred
 */
WUT_XMOD struct wut_Document *wut_doc_create(struct wut_Window *win);


/*
 * Destroy a document and free the allocated memory. This function is save to
 * call, even if doc is NULL.
 *
 * @doc: Pointer to the document
 */
WUT_XMOD void wut_doc_destroy(struct wut_Document *doc);


/*
 * This function will resize the document to fit the given reference shape.
 *
 * @doc: Pointer to the document
 */
WUT_XMOD void wut_doc_resize(struct wut_Document *doc);


/*
 * Update the document, update the document body and the elements contained that
 * have changed.
 *
 * @doc: Pointer to the document
 */
WUT_XMOD void wut_doc_update(struct wut_Document *doc);


/*
 * Only update a branch of the element tree starting from <ele>.
 *
 * @doc: Pointer to the document
 * @ele: The starting element of the branch
 */
WUT_API void wut_doc_refresh_branch(struct wut_Document *doc,
                struct wut_Element *ele);

/*
 * Render the document batch onto the window.
 *
 * @doc: Pointer to the document
 */
WUT_API void wut_doc_render(struct wut_Document *doc);


/*
 * Mark the document as changed, so it will be updated.
 *
 * @doc: Pointer to the document
 * @ele: The element to update from or NULL for the entire document
 * @opt: What has changed
 * @prio: How fast do the changes need to be processed
 */
WUT_XMOD void wut_doc_has_changed(struct wut_Document *doc,
                struct wut_Element *ele, s8 opt, s8 prio);


/*
 * TODO: Update comment
 * Update the element with the closes scrollbar in the tracking table.
 *
 * @doc: Pointer to the document
 * @ele: The element the cursor is currently hovering
 *
 * Returns: 1 if something happened, 0 if not
 */
WUT_XMOD s8 wut_doc_track_move(struct wut_Document *doc,
                struct wut_Element *ele, wut_iVec2 pos);


/*
 * TODO: Update comment
 * Update the element with the closes scrollbar in the tracking table.
 *
 * @doc: Pointer to the document
 * @ele: The element the cursor is currently hovering
 *
 * Returns: 1 if something happened, 0 if not
 */
WUT_XMOD s8 wut_doc_track_scroll(struct wut_Document *doc,
                struct wut_Element *ele, wut_iVec2 pos);


/*
 * TODO: Update comment
 * Update the element with the closes scrollbar in the tracking table.
 *
 * @doc: Pointer to the document
 * @ele: The element the cursor is currently hovering
 *
 * Returns: 1 if something happened, 0 if not
 */
WUT_XMOD s8 wut_doc_track_click(struct wut_Document *doc,
                struct wut_Element *ele, wut_iVec2 pos);


/*
 * Get the most relevant element at the given position.
 *
 * @doc: Pointer to the document
 * @pos: The position to look at
 *
 * Returns: Either a pointer to the most relevant element or NULL if an error
 * 	    occurred or no element could be found
 */
WUT_XMOD struct wut_Element *wut_doc_hovered(struct wut_Document *doc,
                wut_iVec2 pos);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Add a new element to the document.
 *
 * @doc: Pointer to the document
 * @parent: A pointer to the parent element
 * @info: A info-construct to create the element from
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API struct wut_Element *wut_AddElement(struct wut_Document *doc,
                struct wut_Element *parent, struct wut_ElementInfo *info);


/*
 * Remove an element from the document.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the element
 */
WUT_API void wut_RemoveElement(struct wut_Document *doc, struct wut_Element *ele);


/*
 * Get an element from a document by searching for the given name.
 *
 * @doc: Pointer to the document
 * @name: The name of the element to look for
 *
 * Returns: Either a pointer to the element or NULL if an error occurred or no
 * 	    element could be found
 */
WUT_API struct wut_Element *wut_GetElement(struct wut_Document *doc, char *name);


/*
 * Display the element tree starting from <ele>. If <ele> is NULL, the whole
 * tree starting from the body-element will be shown.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the starting element or NULL
 */
WUT_API void wut_ShowDocumentTree(struct wut_Document *doc,
                struct wut_Element *ele);

#endif /* _WUT_DOCUMENT_H */
