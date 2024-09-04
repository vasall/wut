#include "document/inc/document.h"

#include "utility/inc/alarm.h"
#include "utility/inc/utility.h"

#include "core/inc/predefined.h"

#include "system/inc/system.h"


#include <stdlib.h>

/*
 * 
 *     CALLBACK-FUNCTIONS
 *
 */

WUT_INTERN s8 doc_cfnc_remove(struct wut_Element *ele, void *data)
{
        WUT_IGNORE(data);

        /* First detach the element */
        wut_DetachElement(ele);

        /* Then destroy it */
        wut_DestroyElement(ele);

        return 0;
}

WUT_INTERN s8 doc_cfnc_find(struct wut_Element *ele, void *data)
{
        struct wut_ElementSelector *sel = (struct wut_ElementSelector *)data;

        if(sel->state == 1)
                return 1;

        if(strcmp(ele->name, sel->name) == 0) {
                sel->state = 1;
                sel->element = ele;
                return 1;
        }

        return 0;
}

WUT_INTERN s8 doc_cfnc_findpos(struct wut_Element *ele, void *data)
{
        struct wut_ElementSelector *sel = (struct wut_ElementSelector *)data;
        wut_iRect rect;

        wut_GetElementBox(ele, rect);

        if(sel->state == 1)
                return 1;

        if((*sel->pos)[0] < rect[0] || (*sel->pos)[0] > (rect[0] + rect[2]))
                return 0;

        if((*sel->pos)[1] < rect[1] || (*sel->pos)[1] > (rect[1] + rect[3]))
                return 0;

        sel->element = ele;
        sel->state = 1;

        return 1;
}


WUT_INTERN s8 doc_cfnc_update_style(struct wut_Element *ele, void *data)
{
        WUT_IGNORE(data);

        wut_UpdateElementStyle(ele);

        return 0;
}


WUT_INTERN s8 doc_cfnc_update_shape(struct wut_Element *ele, void *data)
{
        WUT_IGNORE(data);

        wut_UpdateElementChildrenShape(ele);

        return 0;
}


WUT_INTERN s8 doc_cfnc_render_ui(struct wut_Element *ele, void *data)
{
        struct wut_Batch *batch = (struct wut_Batch *)data;

        wut_ele_render(batch, ele);

        return 0;
}


WUT_INTERN s8 doc_cfnc_render_ui_post(struct wut_Element *ele, void *data)
{
        struct wut_Batch *batch = (struct wut_Batch *)data;

        wut_ele_ren_scrollbar(batch, ele);

        return 0;
}


WUT_INTERN s8 doc_cfnc_show(struct wut_Element  *ele, void *data)
{
        u8 i;

        s32 lim = 16;

        wut_iRect r;

        WUT_IGNORE(data);

        for(i = 0; i < ele->layer; i++) {
                printf("  ");
                lim -= 2;
        }

        printf("%s ", ele->name);
        lim -= strlen(ele->name);

        for(i = 0; i < lim; i++)
                printf(" ");


        wut_GetBoundingBox(ele, r);
        wut_irect_dump(r);
        printf("  --  ");
        wut_GetElementBox(ele, r);
        wut_irect_dump(r);
        printf("  --  ");
        wut_GetContentBox(ele, r);
        wut_irect_dump(r);

        printf("\n");

        return 0;

}

WUT_INTERN void doc_batch_cfnc_push(struct wut_Batch *ren, void *data)
{
        s32 frame[2];
        wut_iRect *ref = (wut_iRect *)data;

        frame[0] = (*ref)[2];
        frame[1] = (*ref)[3];
        wut_bat_push_uniform(ren, 0, frame);

}


WUT_INTERN s8 doc_create_batch(struct wut_Document *doc)
{
        struct wut_Shader *shd;
        struct wut_Batch *ren;

        struct wut_VertexAttrib v_attributes[] = {
                {3, GL_FLOAT},		/* position */
                {3, GL_INT},		/* 0: shape, 1: limits, 2: everything else */
                {1, GL_INT}		/* type */
        };

        struct wut_UniformTemp uniforms[] = {
                {"u_frame", WUT_UNI_2IV, 1, WUT_UNI_F_DEFAULT},	 /* 0 */
                {"u_rect", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT},	 /* 1 */
                {"u_color", WUT_UNI_4FV, 200, WUT_UNI_F_DEFAULT},	 /* 2 */
                {"u_radius", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT}, /* 3 */
                {"u_bwidth", WUT_UNI_1IV, 200, WUT_UNI_F_DEFAULT}, /* 4 */
                {"u_bcolor", WUT_UNI_4FV, 200, WUT_UNI_F_DEFAULT}, /* 5 */
                {"u_scroll", WUT_UNI_2IV, 200, WUT_UNI_F_DEFAULT}, /* 6 */
                {"u_limit", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT}	 /* 7 */
        };

        shd = wut_GetShader(doc->context, "__def_block_shader");

        ren = wut_bat_create(
                        shd,		/* Pointer to the shader to use */
                        NULL,		/* Pointer to the texture to use */
                        3,		/* Number of vertex attributes */
                        v_attributes,	/* List of all vertex attributes */
                        6000,		/* Vertex capacity */
                        6000,		/* Index capacity */
                        8,		/* Number of uniform buffers */
                        uniforms,	/* List of all uniforms */
                        &doc_batch_cfnc_push,
                        doc->shape_ref
                        );

        if(!ren)
                return -1;

        if((doc->batch_id = wut_ContextAddBatch(doc->context, &ren)) < 0) {
                wut_bat_destroy(ren);
                return -1;
        }

        return 0;
}

WUT_INTERN void doc_destroy_batch(struct wut_Document *doc)
{
        wut_ContextRemoveBatch(doc->context, doc->batch_id);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_API struct wut_Document *wut_CreateDocument(struct wut_Window *win)
{
        struct wut_Document *doc;

        /* Allocate memory for the document */
        if(!(doc = wut_zalloc(sizeof(struct wut_Document)))) {
                WUT_ALARM(WUT_ERROR, "Failed to allocate memory for document");
                goto err_return;
        }

        /* Set the attributes for the document */
        doc->window = win;
        doc->context = win->context;

        /* Get a reference to the shape/size of the window */
        doc->shape_ref = &win->shape;

        /* Create the body element */	
        if(!(doc->body = wut_CreateElement(doc, "body", WUT_BODY, NULL))) {
                WUT_ALARM(WUT_ERROR, "Failed to create body for document");
                goto err_free_doc;
        }

        /* Set the attributes for the body element */
        doc->body->body = doc->body;
        doc->body->parent = NULL;
        doc->body->layer = 0;

        /* Reset the eventbased elements-buffers */
        doc->selected	= NULL;
        doc->hovered	= NULL;

        /* Create the view list */
        if(!(doc->views = wut_vie_create_list(doc->context)))
                goto err_destroy_body;

        /* Create the default batch renderer */
        if(doc_create_batch(doc) < 0)
                goto err_destroy_views;

        /* Create the style class table */
        if(!(doc->classes = wut_cls_create_table()))
                goto err_destroy_batch;

        /* Update the body element */
        wut_UpdateDocument(doc);

        return doc;

err_destroy_batch:
        doc_destroy_batch(doc);

err_destroy_views:
        wut_vie_destroy_list(doc->views);

err_destroy_body:
        wut_DestroyElement(doc->body);

err_free_doc:
        wut_free(doc);

err_return:
        WUT_ALARM(WUT_ERROR, "Failed to create new document");
        return NULL;
}


WUT_API void wut_DestroyDocument(struct wut_Document *doc)
{
        if(!doc)
                return;

        /* Destroy the style class table */
        wut_cls_destroy_table(doc->classes);

        /* Detach and destroy the batch renderer */
        doc_destroy_batch(doc);

        /* If the document contains a body, recursivly remove it */
        wut_RemoveElement(doc, doc->body);	

        /* Then destroy all left over views */
        wut_vie_destroy_list(doc->views);

        wut_free(doc);
}


WUT_API void wut_ResizeDocument(struct wut_Document *doc)
{
        if(!doc) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        /* Then resize the elements */
        wut_UpdateDocument(doc);
}


WUT_API struct wut_Element *wut_AddElement(struct wut_Document *doc,
                struct wut_Element *parent, char *name,
                enum wut_eTag type, void *data)
{
        struct wut_Element *ele;

        if(!doc || !parent || !name) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                goto err_return;
        }

        /* Create new element */
        if(!(ele = wut_CreateElement(doc, name, type, data))) {
                WUT_ALARM(WUT_ERROR, "Failed to create new element for document");
                goto err_return;
        }

        /* Attach element to parent */
        if(wut_AttachElement(parent, ele) < 0) {
                WUT_ALARM(WUT_ERROR, "Failed to attach element to parent");
                goto err_destroy_ele;
        }

        /* Update the new element */
        wut_UpdateDocumentBranch(doc, ele);

        return ele;

err_destroy_ele:
        wut_DestroyElement(ele);

err_return:
        WUT_ALARM(WUT_ERROR, "Failed to add element to document");
        return NULL;
}


WUT_API void wut_RemoveElement(struct wut_Document *doc, struct wut_Element *ele)
{
        if(!doc || !ele) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        wut_ele_hlf(ele, &doc_cfnc_remove, NULL, NULL);
}


WUT_API struct wut_Element *wut_GetElement(struct wut_Document *doc, char *name)
{
        struct wut_ElementSelector sel;

        if(!doc || !name) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                goto err_return;
        }

        sel.state = 0;
        sel.name = name;
        sel.element = NULL;

        /* Recursivly search for the element in the document */
        wut_ele_hlf(doc->body, NULL, &doc_cfnc_find, &sel);

        if(sel.state == 1) {
                return sel.element;
        }

err_return:
        WUT_ALARM(WUT_ERROR, "Failed to get element");
        return NULL;
}


WUT_API struct wut_Element *wut_GetHoveredElement(struct wut_Document *doc,
                wut_iVec2 *pos)
{
        struct wut_ElementSelector sel;

        if(!doc) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                goto err_return;
        }

        sel.state = 0;
        sel.pos = pos;
        sel.element = NULL;

        wut_ele_hlf(doc->body, NULL, &doc_cfnc_findpos, &sel);

        if(sel.state == 1) {
                printf("Found an element\n");
                return sel.element;
        }

        printf("Found no element\n");

        return NULL;

err_return:
        WUT_ALARM(WUT_ERROR, "Failed to get hovered element");
        return NULL;
}


WUT_API void wut_UpdateDocumentBranch(struct wut_Document *doc,
                struct wut_Element *ele)
{
        if(!doc || !ele) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        /* First update the style */
        wut_ele_hlf(ele, &doc_cfnc_update_style, NULL, NULL);

        /* 
         * Then update the shape of the element. Note that this works in a very
         * specifiy way. The function to update the shape will apply the new
         * layout to the children-elements. Because of that, we have to call the
         * function with the parent element. To update the position and shape of
         * the wanted element.
         */
        if(!ele->parent) {
                wut_ele_adjust_shape(ele);

                wut_ele_hlf(ele, &doc_cfnc_update_shape, NULL, NULL);
        }
        else {
                wut_ele_hlf(ele->parent, &doc_cfnc_update_shape, NULL, NULL);
        }
}


WUT_API void wut_UpdateDocument(struct wut_Document *doc)
{
        if(!doc) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        wut_UpdateDocumentBranch(doc, doc->body);
}


WUT_API void wut_RenderDocumentUIBranch(struct wut_Document *doc,
                struct wut_Element *ele)
{
        struct wut_Batch *ren;

        if(!doc || !ele) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        ren = wut_ContextGetBatch(doc->context, doc->batch_id);

        wut_ele_hlf(ele,
                        &doc_cfnc_render_ui,
                        &doc_cfnc_render_ui_post,
                        ren);
}


WUT_API void wut_RenderDocumentUI(struct wut_Document *doc)
{
        if(!doc) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        wut_RenderDocumentUIBranch(doc, doc->body);

}


WUT_API void wut_RenderDocument(struct wut_Document *doc)
{
        struct wut_Batch *ren;

        if(!doc) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        ren = wut_ContextGetBatch(doc->context, doc->batch_id);

        wut_RenderDocumentUI(doc);
}


WUT_API void wut_ShowDocumentTree(struct wut_Document *doc,
                struct wut_Element *ele)
{
        struct wut_Element *start;

        if(!doc) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        if(ele) start = ele;
        else start = doc->body;

        wut_ele_hlf(start, &doc_cfnc_show, NULL, NULL);
}
