#include "source/document/inc/document.h"

#include "source/utility/inc/alarm.h"
#include "source/utility/inc/utility.h"
#include "source/utility/inc/text_formatting.h"

#include "source/core/inc/predefined.h"

#include "source/system/inc/system.h"


#include <stdlib.h>



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CALLBACK-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
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

        if(sel->pos[0] < rect[0] || sel->pos[0] > (rect[0] + rect[2]))
                return 1;

        if(sel->pos[1] < rect[1] || sel->pos[1] > (rect[1] + rect[3]))
                return 1;

        sel->element = ele;
        sel->state = 1;

        return 0;
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

        if(strlen(ele->name) > 0) {
                printf("%s ", ele->name);
                lim -= strlen(ele->name);
        }
        else {
                printf("<no-name> ");
                lim -= 9;
        }

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


WUT_INTERN s8 doc_cfnc_check_scrollbar(struct wut_Element *ele, void *data)
{
        struct wut_DocumentTrackPass *pass =
                (struct wut_DocumentTrackPass *)data;

        /* Both scrollbars have been discovered */
        if(pass->mask == ((1<<0)|(1<<1)))
                return 1;

        /*
         * Vertical scrollbar
         */
        if(!(pass->mask & (1<<0))) {
                if(ele->scrollbar_flags & (1<<0)) {
                        pass->ele_v = ele;
                        pass->mask |= (1<<0);
                }
        }

        /*
         * Horizontal scrollbar
         */
        if(!(pass->mask & (1<<1))) {
                if(ele->scrollbar_flags & (1<<1)) {
                        pass->ele_h = ele;
                        pass->mask |= (1<<1);
                }
        }

        return 0;
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				INTERNAL-FUNCTIONS
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_INTERN void doc_reset_track_table(struct wut_Document *doc)
{
        doc->track_table.has_changed = 0;
        doc->track_table.update_element = NULL;

        doc->track_table.selected = NULL;
        doc->track_table.hovered = NULL;

        doc->track_table.scroll_v = doc->body;
        doc->track_table.scroll_h = doc->body;
}


WUT_INTERN s8 doc_create_batch(struct wut_Document *doc)
{
        struct wut_Shader *shd;
        struct wut_Batch *ren;

        struct wut_VertexAttrib v_block_attributes[] = {
                {3, GL_FLOAT},		/* position */
                {3, GL_INT},		/* 0: shape, 1: limits, 2: everything else */
                {1, GL_INT}		/* type */
        };

        struct wut_UniformTemp block_uniforms[] = {
                {"u_frame", WUT_UNI_2IV, 1, WUT_UNI_F_DEFAULT},     /* 0 */
                {"u_rect", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT},    /* 1 */
                {"u_color", WUT_UNI_4FV, 200, WUT_UNI_F_DEFAULT},   /* 2 */
                {"u_radius", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT},  /* 3 */
                {"u_bwidth", WUT_UNI_1IV, 200, WUT_UNI_F_DEFAULT},  /* 4 */
                {"u_bcolor", WUT_UNI_4FV, 200, WUT_UNI_F_DEFAULT},  /* 5 */
                {"u_scroll", WUT_UNI_2IV, 200, WUT_UNI_F_DEFAULT},  /* 6 */
                {"u_limit", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT}    /* 7 */
        };

        struct wut_VertexAttrib v_scroll_attributes[] = {
                {3, GL_FLOAT},		/* position */
                {3, GL_INT},		/* 0: shape, 1: limits, 2: everything else */
                {1, GL_INT}		/* type */
        };

        struct wut_UniformTemp scroll_uniforms[] = {
                {"u_frame", WUT_UNI_2IV, 1, WUT_UNI_F_DEFAULT},     /* 0 */
                {"u_rect", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT},    /* 1 */
                {"u_color", WUT_UNI_4FV, 200, WUT_UNI_F_DEFAULT},   /* 2 */
                {"u_width", WUT_UNI_1IV, 200, WUT_UNI_F_DEFAULT},   /* 3 */
                {"u_scroll", WUT_UNI_2IV, 200, WUT_UNI_F_DEFAULT},  /* 4 */
                {"u_limit", WUT_UNI_4IV, 200, WUT_UNI_F_DEFAULT}    /* 5 */
        };

        shd = wut_GetShader(doc->context, "__def_block_shader");

        ren = wut_bat_create(
                        shd,		        /* Pointer to the shader to use */
                        NULL,		        /* Pointer to the texture to use */
                        3,		        /* Number of vertex attributes */
                        v_block_attributes,	/* List of all vertex attributes */
                        6000,		        /* Vertex capacity */
                        6000,		        /* Index capacity */
                        8,		        /* Number of uniform buffers */
                        block_uniforms,	        /* List of all uniforms */
                        &doc_batch_cfnc_push,
                        doc->shape_ref,
                        1
                        );

        if(!ren) {
                printf("Failed to create batch renderer for block\n");
                return -1;
        }

        if((doc->batch_id = wut_ContextAddBatch(doc->context, &ren)) < 0) {
                wut_bat_destroy(ren);
                return -1;
        }


        /*  */

        shd = wut_GetShader(doc->context, "__def_scrollbar_shader");

        ren = wut_bat_create(
                        shd,		        /* Pointer to the shader to use */
                        NULL,		        /* Pointer to the texture to use */
                        3,		        /* Number of vertex attributes */
                        v_scroll_attributes,	/* List of all vertex attributes */
                        6000,		        /* Vertex capacity */
                        6000,		        /* Index capacity */
                        6,		        /* Number of uniform buffers */
                        scroll_uniforms,	/* List of all uniforms */
                        &doc_batch_cfnc_push,
                        doc->shape_ref,
                        0
                        );

        if(!ren) {
                printf("Failed to create batch renderer for scroll\n");
                return -1;
        }

        if((doc->scroll_id = wut_ContextAddBatch(doc->context, &ren)) < 0) {
                wut_bat_destroy(ren);
                return -1;
        }


        return 0;
}

WUT_INTERN void doc_destroy_batch(struct wut_Document *doc)
{
        wut_ContextRemoveBatch(doc->context, doc->batch_id);
}


WUT_INTERN struct wut_Element *doc_common_parent(struct wut_Element *e1,
                struct wut_Element *e2)
{
        struct wut_Element *run1 = e1;
        struct wut_Element *run2 = e2;

        while(run1 && run2) {
                if(wut_ele_compare(run1, run2)) {
                        return run1;
                }

                run1 = run1->parent;
                run2 = run2->parent;
        }

        return e1;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_XMOD struct wut_Document *wut_doc_create(struct wut_Window *win)
{
        struct wut_Document *doc;
        struct wut_ElementInfo body_info;

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
        body_info.tag = WUT_BODY;
        wut_SetDictionary(&body_info.attrib, "name", "body");

        if(!(doc->body = wut_CreateElement(doc, &body_info))) {
                WUT_ALARM(WUT_ERROR, "Failed to create body for document");
                goto err_free_doc;
        }

        /* Set the attributes for the body element */
        doc->body->document = doc;
        doc->body->body = doc->body;
        doc->body->parent = NULL;
        doc->body->layer = 0;

        /* Reset the tracking table */
        doc_reset_track_table(doc);

        /* Create the view list */
        if(!(doc->views = wut_vie_create_list(doc->context)))
                goto err_destroy_body;

        /* Create the default batch renderer */
        if(doc_create_batch(doc) < 0)
                goto err_destroy_views;

        /* Create the style class table */
        if(!(doc->class_table = wut_cls_create_table()))
                goto err_destroy_batch;

        /* Mark the document for updating */
        wut_doc_has_changed(doc, NULL, 0, 0);

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


WUT_XMOD void wut_doc_destroy(struct wut_Document *doc)
{
        if(!doc)
                return;

        /* Destroy the style class table */
        wut_cls_destroy_table(doc->class_table);

        /* Detach and destroy the batch renderer */
        doc_destroy_batch(doc);

        /* If the document contains a body, recursivly remove it */
        wut_RemoveElement(doc, doc->body);	

        /* Then destroy all left over views */
        wut_vie_destroy_list(doc->views);

        wut_free(doc);
}


WUT_XMOD void wut_doc_resize(struct wut_Document *doc)
{
        if(!doc) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }

        /* Mark the document for updating */
        wut_doc_has_changed(doc, NULL, 0, 0);
}


WUT_XMOD void wut_doc_update(struct wut_Document *doc)
{
        struct wut_Element *ele;

        if(!doc) return;
        if(!doc->track_table.has_changed) return;

        ele = doc->track_table.update_element;

        /* TODO: Home update flag is set, but not the element
         * Probably fixed.*/
        if(!ele) {
                doc->track_table.has_changed = 0;
                doc->track_table.update_element = NULL;
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

        doc->track_table.update_element = NULL;
        doc->track_table.has_changed = 0;
}


WUT_API void wut_doc_render(struct wut_Document *doc)
{
        struct wut_Batch *ren;

        if(!doc) {
                WUT_ALARM(WUT_WARNING, "Input parameters invalid");
                return;
        }


        /*
         * Then render the scrollbar.
         */
        ren = wut_ContextGetBatch(doc->context, doc->scroll_id);

        wut_ele_hlf(doc->body,
                        &doc_cfnc_render_ui_post,
                        NULL,
                        ren);

        /*
         * Render the elements.
         */
        ren = wut_ContextGetBatch(doc->context, doc->batch_id);

        wut_ele_hlf(doc->body,
                        &doc_cfnc_render_ui,
                        NULL,
                        ren);


}


WUT_XMOD void wut_doc_has_changed(struct wut_Document *doc,
                struct wut_Element *ele, s8 opt, s8 prio)
{
        struct wut_Element *uele;

        if(!doc) return;

        uele = doc->track_table.update_element;

        /* If an element is given... */
        if(ele) {
                /* and an element is already set, then get the common parent */
                if(uele) {
                        uele = doc_common_parent(uele, ele); 
                }
                /* and no element is set, just use the given one */
                else {
                        uele = ele;
                }
        }
        /* otherwise just update the entire document */
        else {
                uele = doc->body;
        }

        doc->track_table.update_element = uele;
        doc->track_table.has_changed = 1;
}


WUT_XMOD s8 wut_doc_track_move(struct wut_Document *doc,
                struct wut_Element *ele, wut_iVec2 pos)
{
        struct wut_Element *cele_hover = doc->track_table.hovered;
        struct wut_Window *win = doc->window;

        wut_iVec2 cpos;

        wut_ivec2_cpy(cpos, pos);

        /*
         * Check if the hovered element has changed.
         */
        if(!wut_ele_compare(cele_hover, ele)) {
                /* If that is the case, first modify the element flags */
                wut_ele_mod_info(cele_hover, WUT_ELE_F_HOVERED, 0);
                wut_ele_mod_info(ele, WUT_ELE_F_HOVERED, 1);

                /* Then link the new element */
                doc->track_table.hovered = ele;

                /* Lastly trigger the events */
                if(cele_hover) {
                        wut_evt_trigger_raw(
                                        WUT_EVT_ELEMENTLEAVE,
                                        win,
                                        cele_hover
                                        );
                }

                /* 
                 * Check if we even move into another element as this function
                 * also also called if the user leaves the window.
                 */
                if(ele) {
                        wut_evt_trigger_raw(
                                        WUT_EVT_ELEMENTENTER,
                                        win,
                                        ele
                                        );
                }

                wut_doc_track_scroll(doc, ele);
        }

        return 1;
}


WUT_XMOD s8 wut_doc_track_scroll(struct wut_Document *doc,
                struct wut_Element *ele)
{
        struct wut_DocumentTrackPass pass;

        struct wut_Element *track_v;
        struct wut_Element *track_h;

        pass.mask = 0;
        pass.ele_v = NULL;
        pass.ele_h = NULL;

        wut_ele_rise(ele, &doc_cfnc_check_scrollbar, &pass);

        track_v = doc->track_table.scroll_v;
        track_h = doc->track_table.scroll_h;

        printf("Check v: \"%s\", h: \"%s\"\n",
                        track_v->name,
                        track_h->name);

        /*
         * Vertical scrollbar
         */
        if(!wut_ele_compare(track_v, pass.ele_v)) {
                if(track_v) {
                        track_v->scrollbar_flags &= ~(1<<2);
                }
                pass.ele_v->scrollbar_flags |= (1<<2);
                doc->track_table.scroll_v = pass.ele_v;
                printf("Updated v-scrollbar to \"%s\"\n", 
                                doc->track_table.scroll_v->name);
                
                wut_doc_has_changed(doc, doc->track_table.scroll_v, 0, 0);
        }

        /*
         * Horizontal scrollbar
         */
        if(!wut_ele_compare(track_h, pass.ele_h)) {
                if(track_h) {
                        track_h->scrollbar_flags &= ~(1<<3);
                }
                pass.ele_h->scrollbar_flags |= (1<<3);
                doc->track_table.scroll_h = pass.ele_h;
                printf("Updated h-scrollbar to \"%s\"\n", 
                                doc->track_table.scroll_h->name);

                wut_doc_has_changed(doc, doc->track_table.scroll_h, 0, 0);
        }

        return 0;
}


WUT_XMOD s8 wut_doc_track_click(struct wut_Document *doc,
                struct wut_Element *ele, wut_iVec2 pos)
{
        struct wut_Element *cele_selected = doc->track_table.selected;
        struct wut_Window *win = doc->window;

        /*
         * Check if the selected element has changed.
         */
        if(!wut_ele_compare(cele_selected, ele)) {
                /* If that is the case, first modify the element flags */
                wut_ele_mod_info(cele_selected, WUT_ELE_F_SELECTED, 0);
                wut_ele_mod_info(ele, WUT_ELE_F_SELECTED, 1);

                /* Then link the new element */
                doc->track_table.selected = ele;

                /* Lastly trigger the events */
                if(cele_selected) {
                        wut_evt_trigger_raw(
                                        WUT_EVT_ELEMENTUNSELECT,
                                        win,
                                        cele_selected
                                        );
                }

                if(ele) {
                        wut_evt_trigger_raw(
                                        WUT_EVT_ELEMENTSELECT,
                                        win,
                                        ele
                                        );
                }

                return 1;
        }

        return 0;
}


WUT_XMOD struct wut_Element *wut_doc_hovered(struct wut_Document *doc,
                wut_iVec2 pos)
{
        struct wut_ElementSelector sel;

        if(!doc) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                goto err_return;
        }

        sel.state = 0;
        wut_ivec2_cpy(sel.pos, pos);
        sel.element = NULL;

        wut_ele_hlf(doc->body, &doc_cfnc_findpos, NULL, &sel);

        if(sel.state == 1) {
                return sel.element;
        }

        return NULL;

err_return:
        WUT_ALARM(WUT_ERROR, "Failed to get hovered element");
        return NULL;
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_API struct wut_Element *wut_AddElement(struct wut_Document *doc,
                struct wut_Element *parent, struct wut_ElementInfo *info)
{
        struct wut_Element *ele;

        if(!doc || !info) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                goto err_return;
        }

        if(!parent) {
                parent = doc->body;
        }

        /* Create new element */
        if(!(ele = wut_CreateElement(doc, info))) {
                WUT_ALARM(WUT_ERROR, "Failed to create new element for document");
                goto err_return;
        }

        /* Attach element to parent */
        if(wut_AttachElement(parent, ele) < 0) {
                WUT_ALARM(WUT_ERROR, "Failed to attach element to parent");
                goto err_destroy_ele;
        }

        /* Mark the document for updating */
        wut_doc_has_changed(doc, ele, 0, 0);

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
