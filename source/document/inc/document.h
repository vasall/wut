#ifndef _WUT_DOCUMENT_H
#define _WUT_DOCUMENT_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "document/inc/element.h"

#include "graphic/inc/batch.h"
#include "graphic/inc/object.h"
#include "graphic/inc/constructor.h"
#include "graphic/inc/flat.h"

#include "style/inc/class.h"

struct wut_Document {
        /* A pointer to the window this document belongs to */
        struct wut_Window 		*window;

        /* A pointer to the context */
        struct wut_Context 		*context;

        /* The body element, to which all future elements will be attached */
        struct wut_Element 		*body;

        struct wut_Element		*selected;
        struct wut_Element		*hovered;

        /* A reference to the window size */
        wut_iRect 		        *shape_ref;

        /* A list of all views */
        struct wut_ViewList 		*views;

        /* The main batch render */
        s16 				batch_id;

        /* All available style classes for this document */
        struct wut_ClassTable           *class_table;
};

struct wut_ElementSelector {
        s8 			state;
        char 			*name;
        wut_iVec2 	        *pos;
        struct wut_Element 	*element;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
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
WUT_API struct wut_Document *wut_CreateDocument(struct wut_Window *win);


/*
 * Destroy a document and free the allocated memory. This function is save to
 * call, even if doc is NULL.
 *
 * @doc: Pointer to the document
 */
WUT_API void wut_DestroyDocument(struct wut_Document *doc);


/*
 * This function will resize the document to fit the given reference shape.
 *
 * @doc: Pointer to the document
 */
WUT_API void wut_ResizeDocument(struct wut_Document *doc);


/*
 * Add a new element to the document.
 *
 * @doc: Pointer to the document
 * @parent: A pointer to the parent element
 * @name: The name of the element
 * @type: The type of the new element
 * @[data]: Optional data for the element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API struct wut_Element *wut_AddElement(struct wut_Document *doc,
                struct wut_Element *parent, char *name,
                enum wut_eTag type, void *data);


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
 * Get the most relevant element at the given position.
 *
 * @doc: Pointer to the document
 * @pos: The position to look at
 *
 * Returns: Either a pointer to the most relevant element or NULL if an error
 * 	    occurred or no element could be found
 */
WUT_API struct wut_Element *wut_GetHoveredElement(struct wut_Document *doc,
                wut_iVec2 *pos);


/*
 * Only update a branch of the element tree starting from <ele>.
 *
 * @doc: Pointer to the document
 * @ele: The starting element of the branch
 */
WUT_API void wut_UpdateDocumentBranch(struct wut_Document *doc,
                struct wut_Element *ele);


/*
 * Update the whole document.
 *
 * @doc: Pointer to the document
 */
WUT_API void wut_UpdateDocument(struct wut_Document *doc);


/*
 * Only render a branch of the element tree onto the UI texture, starting from
 * <ele>.
 *
 * @doc: Pointer to the document
 * @ele: The starting element of the branch
 */
WUT_API void wut_RenderDocumentUIBranch(struct wut_Document *doc,
                struct wut_Element *ele);


/*
 * Render the whole element tree onto the UI texture.
 *
 * @doc: Pointer to the document
 */
WUT_API void wut_RenderDocumentUI(struct wut_Document *doc);


/*
 * Render the document onto the screen.
 *
 * @doc: Pointer to the document
 */
WUT_API void wut_RenderDocument(struct wut_Document *doc);


/*
 * Display the element tree starting from <ele>. If <ele> is NULL, the whole
 * tree starting from the body-element will be shown.
 *
 * @doc: Pointer to the document
 * @ele: Pointer to the starting element or NULL
 */
WUT_API void wut_ShowDocumentTree(struct wut_Document *doc,
                struct wut_Element *ele);

/*
 * Load an element-tree from a file.
 *
 * @doc: Pointer to the document
 * @pth: Pointer to the file containing the element structure
 * @[atc]: The attachment element or NULL for body
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_LoadElements(struct wut_Document *doc, char *pth,
                struct wut_Element *ele);

/*
 * Load classes from a style file and add them to the class table.
 *
 * @doc: Pointer to the document
 * @pth: The path to the style file
 *
 * Returns: 0 on success or -1 if na error occurred
 */
WUT_API s8 wut_LoadClasses(struct wut_Document *doc, char *pth);

#endif /* _WUT_DOCUMENT_H */
