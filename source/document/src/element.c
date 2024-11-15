#include "source/document/inc/element.h"

#include "source/document/inc/document.h"

#include "source/utility/inc/alarm.h"

#include "source/math/inc/polygon.h"

#include "source/widget/inc/widget.h"

#include "source/system/inc/system.h"

#include "source/style/inc/layout.h"


#include <stdlib.h>


WUT_INTERN void ele_update_offset(struct wut_Element *ele)
{
	struct wut_Element *par = ele->parent;

        wut_ivec2_clr(ele->relative_offset);

	/*
	 * If the element has a parent, calculate the offset from the upper-left
	 * corner of the parents shape to the upper-left corner of this elements
	 * shape.
	 */
	if(par) {
		ele->relative_offset[0] =
			par->style.shape_content_delta[0] -	/* Border+Padding */
			par->content_offset[0] +		/* Scrolling */
			ele->layout_offset[0];			/* Layout */
		ele->relative_offset[1] =
			par->style.shape_content_delta[1] -	/* Border+Padding */
			par->content_offset[1] +		/* Scrolling */
			ele->layout_offset[1];			/* Layout */
	}

	ele->absolute_offset[0] = ele->relative_offset[0];
	ele->absolute_offset[1] = ele->relative_offset[1];

	/*
	 * If this element has a parent, move the absolute offset by the
	 * absolute offset of the parent.
	 */
	if(par) {
		ele->absolute_offset[0] += par->absolute_offset[0];
		ele->absolute_offset[1] += par->absolute_offset[1];
	}
}



WUT_INTERN void ele_calc_bounding_rect(struct wut_Element *ele)
{
	wut_irect_mov(ele->bounding_rect, ele->style.shape_bounding_box,
                        ele->absolute_offset);
}


WUT_INTERN void ele_calc_element_rect(struct wut_Element *ele)
{
	wut_irect_add(ele->element_rect, ele->bounding_rect,
                        ele->style.shape_element_delta);
}


WUT_INTERN void ele_calc_inner_rect(struct wut_Element *ele)
{
	wut_irect_add(ele->inner_rect, ele->bounding_rect, 
                        ele->style.shape_inner_delta);
}


WUT_INTERN void ele_calc_content_rect(struct wut_Element *ele)
{
	wut_irect_add(ele->content_rect, ele->bounding_rect, 
                        ele->style.shape_content_delta);
}


WUT_INTERN void ele_calc_visible(struct wut_Element *ele)
{
	wut_iRect out;
	wut_iRect dif;

	/*
	 * First calculate the absolute position of the reference-area of the 
	 * bounding box in the window.
	 */
	wut_irect_mov(out, ele->style.shape_bounding_box, ele->absolute_offset);

	/*
	 * Finally convert from the bounding box to the element box.
	 */
	wut_irect_add(out, out, ele->style.shape_element_delta);

	wut_irect_cpy(dif, out);	/* Will be used later */

	/*
	 * Check intersecting area with the parent.
	 */
	if(ele->parent) {
		if(!wut_irect_intersect(dif, out, ele->parent->output_rect)) {
			/* If the element is not inside the parent */
			ele->info_flags &= ~WUT_ELE_F_VISIBLE;
			return;
		}
	}

	/*
	 * Now we have to validate the intersecting area with the window.
	 * This will also check if the element even is inside the window.
	 */
	if(!wut_irect_intersect(dif, dif, *ele->document->shape_ref)) {
		/* Element is not inside the window */
		ele->info_flags &= ~WUT_ELE_F_VISIBLE;
		return;
	}

	/*
	 * Copy the visible area into the according rectangle.
	 */
	wut_irect_cpy(ele->visible_out_rect, dif);

	/*
	 * Now we can copy the resulting rectangle to the element and mark the
         * element as visible.
	 */
	wut_irect_cpy(ele->output_rect, out);
	ele->info_flags |= WUT_ELE_F_VISIBLE;
}


WUT_XMOD s8 wut_ele_scroll(struct wut_Element *ele, s32 *val)
{
	s32 limits[2];
	u8 ret = 0;

        /* TODO */
        if(!ele) {
                return -1;
        }

	if(val[0] != 0 && (ele->scrollbar_flags & WUT_RESTYLE_SCROLL_H)) {
		limits[0] = 0;
		limits[1] = ele->content_size[0] - ele->content_rect[2]; 

		ele->content_offset[0] -= val[0];

		if(ele->content_offset[0] < limits[0]) ele->content_offset[0] = limits[0];
		if(ele->content_offset[0] > limits[1]) ele->content_offset[0] = limits[1];

		ret = 1;
	}
	if(val[1] != 0 && (ele->scrollbar_flags & WUT_RESTYLE_SCROLL_V)) {
		limits[0] = 0;
		limits[1] = ele->content_size[1] - ele->content_rect[3]; 

		ele->content_offset[1] -= val[1];

		if(ele->content_offset[1] < limits[0]) ele->content_offset[1] = limits[0];
		if(ele->content_offset[1] > limits[1]) ele->content_offset[1] = limits[1];

		ret = 1;
	}

	
        /* Mark the document to be updated */
        wut_doc_has_changed(ele->document, ele->document->body, 0, WUT_HIGH);

	return ret;
}


WUT_INTERN void ele_link_classes(struct wut_Element *ele)
{
        wut_stl_link_classes(&ele->style, ele->document->class_table);
}


WUT_INTERN s8 ele_link_to_par(struct wut_Element *ele, void *data)
{
        struct wut_Element *parent = ele->parent;

        WUT_IGNORE(data);

        /* Copy over the different pointers */
	ele->layer = parent->layer + 1;

	/* Link the stylesheet */
	wut_stl_link(&ele->style, &parent->style);

        return 0;
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_XMOD void wut_ele_adjust_shape(struct wut_Element *ele)
{
        /* First calculate the relative and absolute offset */
	ele_update_offset(ele);

	/* Then calculate the different shape rectangles */
	ele_calc_bounding_rect(ele);
	ele_calc_element_rect(ele);
	ele_calc_inner_rect(ele);
	ele_calc_content_rect(ele);

	/* Finally check visibility */
	ele_calc_visible(ele);
}


WUT_XMOD void wut_ele_hdl_scrollbar(struct wut_Element *ele)
{
	u8 flag = 0;
	wut_iRect inner_rect;

	wut_GetContentBox(ele, inner_rect); 

        /*
         * Check if scrollbars are enabled.
         */
	if(inner_rect[3] < ele->content_size[1])
		flag |= WUT_RESTYLE_SCROLL_V;

	if(inner_rect[2] < ele->content_size[0])
		flag |= WUT_RESTYLE_SCROLL_H;

	ele->scrollbar_flags = flag & ele->style.scrollbar_flags;
}


WUT_XMOD void wut_ele_set_scrollbar_vis(struct wut_Element *ele, s8 opt)
{
        if(!ele)
                return;

        if(opt) {
                if((ele->scrollbar_flags & (1<<0)))
                        ele->scrollbar_flags |= (1<<2);
                if((ele->scrollbar_flags & (1<<1)))
                        ele->scrollbar_flags |= (1<<3);
        }
        else {
                ele->scrollbar_flags &= ~((1<<2)|(1<<3));
        }
}


WUT_XMOD void wut_ele_get_scrollbar_vert(struct wut_Element *ele,
               wut_iRect rect)
{
        s32 bw = ele->style.border_width;

        rect[0] = ele->element_rect[0] + ele->element_rect[2] - bw - 10;
	rect[1] = ele->element_rect[1] + bw;
	rect[2] = 10;
	rect[3] = ele->element_rect[3];
}


WUT_XMOD void wut_ele_get_scrollbar_hori(struct wut_Element *ele,
               wut_iRect rect)
{
        s32 bw = ele->style.border_width;

        rect[0] = ele->element_rect[0] + bw;
        rect[1] = ele->element_rect[1] + ele->element_rect[3] - bw - 10;
	rect[2] = ele->element_rect[2];
	rect[3] = 10;
}


WUT_XMOD s8 wut_ele_compare(struct wut_Element *in1, struct wut_Element *in2)
{
        /* If at least one of the elements is NULL... */
        if(!in1 || !in2) {
                /* If both elements are NULL, then they're equal */
                if(in1 == in2) {
                        return 1;
                }

                return 0;
        }
        
	if(in1->id == in2->id) {
		return 1;
	}

	return 0;
}



WUT_XMOD s8 wut_ele_hlf(struct wut_Element *ele, wut_ElementFunc prefnc,
		wut_ElementFunc postfnc, void *data)
{
	struct wut_Element *run;
	struct wut_Element *next;

	if(!ele)
		return 0;

	if(prefnc) {
		if(prefnc(ele, data)) {
			return 1;
                }
	}


	run = ele->children;
	while(run) {
		next = run->right;

		if(wut_ele_hlf(run, prefnc, postfnc, data))
			return 1;

		run = next;
	}

	if(postfnc) {
		return postfnc(ele, data);
	}

	return 0;
}


WUT_XMOD void wut_ele_mod_info(struct wut_Element *ele, u8 flag, u8 val)
{
	if(!ele)
		return;

	if(val)
		ele->info_flags &= ~flag;
	else 
		ele->info_flags |= flag;
}


WUT_XMOD void wut_ele_update(struct wut_Element *ele)
{
        /* TODO */
       WUT_IGNORE(ele); 
}


WUT_XMOD void wut_ele_render(struct wut_Batch *ren, struct wut_Element *ele)
{
	s32 indices[4];
	s32 v_index[3];
	f32 color[4];

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		s32 index[3];
		s32 type;
	} vdata;

	s32 p0x;
	s32 p0y;
	s32 p1x;
	s32 p1y;


	/*
	 * Return if the element is not visible.
	 */
	if(!(ele->info_flags & WUT_ELE_F_VISIBLE))
		return;

	p0x = ele->element_rect[0];
	p0y = ele->element_rect[1];
	p1x = ele->element_rect[0] + ele->element_rect[2];
	p1y = ele->element_rect[1] + ele->element_rect[3];

	/* Unioform: u_rect */
	v_index[0] = wut_bat_push_uniform(ren, 1, ele->element_rect);

	/* Uniform: u_color */
	wut_GetColorFV(ele->style.infill_color, color);
	v_index[2] = wut_bat_push_uniform(ren, 2, color);

	/* Uniform: u_radius */
	wut_bat_push_uniform(ren, 3, ele->style.radius_corner);

	/* Uniform: u_bwidth */
	wut_bat_push_uniform(ren, 4, &ele->style.border_width);

	/* Uniform: u_bcolor */
	wut_GetColorFV(ele->style.border_color, color);
	wut_bat_push_uniform(ren, 5, color);

	/* Uniform: u_limit */
	if(ele->parent) {
		v_index[1] = wut_bat_push_uniform(ren, 7, ele->parent->visible_out_rect);
	}
	else {
		v_index[1] = -1;
	}

	vdata.z = (f32)ele->layer / 100.0;
	vdata.index[0] = v_index[0];	/* The element rectangle */
	vdata.index[1] = v_index[1];	/* The rendering zone */
	vdata.index[2] = v_index[2];	/* The color to use */
	vdata.type = WUT_RENTYPE_DEFAULT;

	vdata.x = (f32)p0x;
	vdata.y = (f32)p0y;
	indices[0] = wut_bat_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p1x;
	vdata.y = (f32)p0y;
	indices[1] = wut_bat_push_vertex(ren, (void *)&vdata);

	vdata.x = (f32)p1x;
	vdata.y = (f32)p1y;
	indices[2] = wut_bat_push_vertex(ren, (void *)&vdata);


	vdata.x = (f32)p0x;
	vdata.y = (f32)p1y;
	indices[3] = wut_bat_push_vertex(ren, (void *)&vdata);

	wut_bat_push_index(ren, indices[0]);
	wut_bat_push_index(ren, indices[2]);
	wut_bat_push_index(ren, indices[3]);

	wut_bat_push_index(ren, indices[0]);
	wut_bat_push_index(ren, indices[1]);
	wut_bat_push_index(ren, indices[2]);

#if 0
	printf("Render \"%s\": [%d, %d, %d, %d]\n", 
			ele->name,
			p0x, p0y,
			p1x, p1y);
#endif

	/* If the element has a widget, render that aswell */
	if(ele->widget) {
		wut_RenderWidget(ele->widget);
	}
}


WUT_XMOD void wut_ele_ren_scrollbar(struct wut_Batch *ren, struct wut_Element *ele)
{
	wut_iRect rect;
	s32 indices[4];
	s32 s_index[3];

        f32 color[4] = {1.0, 0, 0, 1.0};
        static s32 width = 5;

        /* 
         * index 0: offset of the slider
         * index 1: size of the slider
         */
	s32 scroll[2];

	struct tempStruct_vtx {
		f32 x;
		f32 y;
		f32 z;
		s32 index[3];
		s32 type;
	} vdata;

        s32 p0x;
        s32 p0y;
        s32 p1x;
        s32 p1y;

	/* vertical */
	if(ele->scrollbar_flags & WUT_RESTYLE_SCROLL_V) {
                /* Scrollbar is active */
                if(ele->scrollbar_flags & (1<<4)) {
                        width = 10;
                }

		p0x = ele->element_rect[0] + ele->element_rect[2] -
                        ele->style.border_width - width;
		p0y = ele->element_rect[1] + ele->style.border_width;
		p1x = ele->element_rect[0] + ele->element_rect[2] - 
                        ele->style.border_width;
		p1y = ele->element_rect[1] + ele->element_rect[3] -
                        ele->style.border_width;

		wut_irect_set(rect,
				p0x,
				p0y,
				width,
				ele->element_rect[3] - (ele->style.border_width * 2));


		/* Unioform: u_rect */
		s_index[0] = wut_bat_push_uniform(ren, 1, rect);

	        /* Uniform: u_color */
	        s_index[2] = wut_bat_push_uniform(ren, 2, color);

        	/* Uniform: u_width */
        	wut_bat_push_uniform(ren, 3, &width);

		/* Uniform: u_scroll */	
		scroll[1] = (p1y - p0y) *
                        ((f32)ele->content_rect[3] / (f32)ele->content_size[1]);
		scroll[0] = (p1y - p0y - 2 - scroll[1]) *
                        ((f32)ele->content_offset[1] / (f32)(ele->content_size[1] -
                                ele->content_rect[3]));
		s_index[2] = wut_bat_push_uniform(ren, 4, scroll);

 
	        /* Uniform: u_limit */
	        if(ele->parent) {
		        s_index[1] = wut_bat_push_uniform(ren, 5, 
                                        ele->parent->visible_out_rect);
	        }
	        else {
		        s_index[1] = -1;
	        }               

		vdata.z = (f32)ele->layer / 100.0;
		vdata.index[0] = s_index[0];
		vdata.index[1] = s_index[1];
		vdata.index[2] = s_index[2];
		vdata.type = WUT_RENTYPE_SCROLL_V;


		vdata.x = (f32)p0x;
		vdata.y = (f32)p0y;
		indices[0] = wut_bat_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p1x;
		vdata.y = (f32)p0y;
		indices[1] = wut_bat_push_vertex(ren, (void *)&vdata);

		vdata.x = (f32)p1x;
		vdata.y = (f32)p1y;
		indices[2] = wut_bat_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p0x;
		vdata.y = (f32)p1y;
		indices[3] = wut_bat_push_vertex(ren, (void *)&vdata);

		wut_bat_push_index(ren, indices[0]);
		wut_bat_push_index(ren, indices[2]);
		wut_bat_push_index(ren, indices[3]);

		wut_bat_push_index(ren, indices[0]);
		wut_bat_push_index(ren, indices[1]);
		wut_bat_push_index(ren, indices[2]);	
	}

	/* horizontal */
	if(ele->scrollbar_flags & WUT_RESTYLE_SCROLL_H) {	
                /* Scrollbar is active */
                if(ele->scrollbar_flags & (1<<5)) {
                        width = 10;
                }

                p0x = ele->element_rect[0] + ele->style.border_width;
		p0y = ele->element_rect[1] + ele->element_rect[3] -
                        ele->style.border_width - width;

		p1x = ele->element_rect[0] + ele->element_rect[2] -
                        ele->style.border_width;
		p1y = ele->element_rect[1] + ele->element_rect[3] -
                        ele->style.border_width;

                /* 
                 * If the vertical scrollbar is also active, reduce size to
                 * prevent overlap.
                 */
	        if(ele->scrollbar_flags & WUT_RESTYLE_SCROLL_V) {
                        p1x -= width;
                }

		wut_irect_set(rect,
				p0x,
				p0y, 
				ele->element_rect[2] - (ele->style.border_width * 2),
				width);


		/* Unioform: u_rect */
		s_index[0] = wut_bat_push_uniform(ren, 1, rect);

	        /* Uniform: u_color */
	        s_index[2] = wut_bat_push_uniform(ren, 2, color);

        	/* Uniform: u_width */
        	wut_bat_push_uniform(ren, 3, &width);

		/* Uniform: u_scroll */	
		scroll[1] = (p1x - p0x) *
                        ((f32)ele->content_rect[2] / (f32)ele->content_size[0]);
		scroll[0] = (p1x - p0x - 2 - scroll[1]) *
                        ((f32)ele->content_offset[0] / (f32)(ele->content_size[0] -
                                ele->content_rect[2]));
		s_index[2] = wut_bat_push_uniform(ren, 4, scroll);

		vdata.z = (f32)ele->layer / 100.0;
		vdata.index[0] = s_index[0];
		vdata.index[1] = -1;
		vdata.index[2] = s_index[2];
		vdata.type = WUT_RENTYPE_SCROLL_H;


		vdata.x = (f32)p0x;
		vdata.y = (f32)p0y;
		indices[0] = wut_bat_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p1x;
		vdata.y = (f32)p0y;
		indices[1] = wut_bat_push_vertex(ren, (void *)&vdata);

		vdata.x = (f32)p1x;
		vdata.y = (f32)p1y;
		indices[2] = wut_bat_push_vertex(ren, (void *)&vdata);


		vdata.x = (f32)p0x;
		vdata.y = (f32)p1y;
		indices[3] = wut_bat_push_vertex(ren, (void *)&vdata);

		wut_bat_push_index(ren, indices[0]);
		wut_bat_push_index(ren, indices[2]);
		wut_bat_push_index(ren, indices[3]);

		wut_bat_push_index(ren, indices[0]);
		wut_bat_push_index(ren, indices[1]);
		wut_bat_push_index(ren, indices[2]);
	}
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API struct wut_Element *wut_CreateElement(struct wut_Document *doc,
                struct wut_ElementInfo *info)
{
	struct wut_Element *ele;
        static s32 element_counter = 0;

        if(!info) {
                WUT_ALARM(WUT_ERROR, "Input parameters invalid");
                goto err_return;
        }
	
	if(!(ele = wut_zalloc(sizeof(struct wut_Element)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for new element");
		goto err_return;
	}

	/* Set the identifier */
	ele->identity = WUT_IDT_ELEMENT;
        ele->id = element_counter++;

	/* Set the basic attributes for the element */
	strcpy(ele->name, wut_GetDictionary(&info->attrib, "name"));
	ele->type = info->tag;

	printf("Element \"%s\" has type %d\n", ele->name, ele->type);

	/* ...and reset the rest */
	ele->layer = 0;
	ele->document = doc;
	ele->body = doc->body;
	ele->parent = NULL;
	ele->left = NULL;
	ele->right = NULL;
	ele->slot = -1;
	ele->children_num = 0;
	ele->children = NULL;

	/* Create the event handler */
	if(!(ele->event_handler = wut_hdl_create())) {
		WUT_ALARM(WUT_ERROR, "Failed to create event handler");
		goto err_free_ele;
	}

	/* Initialize the style structure */
	if(wut_stl_init(&ele->style, NULL) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to initialize style for element");
		goto err_destroy_handler;
	}

        /* 
         * If classes are already given in the attribute-dictionary, pass them
         * on, directly.
         */
        wut_AddClasses(ele, wut_GetDictionary(&info->attrib, "class"));

	/* Load a template, if there is one for the given type */
	ele->widget = NULL;
	if(wut_etm_load(ele, &info->attrib) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to load the template for the element");
		goto err_destroy_handler;
	}

	return ele;

err_destroy_handler:
	wut_hdl_destroy(ele->event_handler);

err_free_ele:
	wut_free(ele);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new element");
	return NULL;
}


WUT_API void wut_DestroyElement(struct wut_Element *ele)
{
	if(!ele) {
		return;
	}

	wut_hdl_destroy(ele->event_handler);

	/* If the element has a widget attached to it, destroy that aswell */
	if(ele->widget) {
		wut_DestroyWidget(ele->widget);
	}

	wut_free(ele);
}


WUT_API s8 wut_AttachElement(struct wut_Element *parent,
                struct wut_Element *ele)
{
	struct wut_Element *run;

	if(!parent || !ele) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!parent->children) {
		parent->children = ele;
	}
	else {	
		run = parent->children;	
		while(run->right) {
			run = run->right;
		}

		run->right = ele;
		ele->left = run;
	}

	parent->children_num++;

        /*
         * Finally link the element to the parent by copying over the references
         * to the document and body. Also link the stylesheets together.
         */
	ele->parent = parent;
        wut_ele_hlf(ele, &ele_link_to_par, NULL, NULL);

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to attach element to parent");
	return -1;
}


WUT_API void wut_DetachElement(struct wut_Element *ele)
{
	struct wut_Element *par;

	if(!ele) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	par = ele->parent;

	if(!par) {
		return;
	}

	if(!ele->left)
		par->children = ele->right;
	else
		(ele->left)->right = ele->right;

	if(ele->right)
		(ele->right)->left = ele->left;

	par->children_num--;

	ele->left = NULL;
	ele->right = NULL;
	ele->parent = NULL;
}


WUT_API void wut_AddClasses(struct wut_Element *ele, char *names)
{
        /* Add class names to style struct */
        wut_stl_add_classes(&ele->style, names);

        /*
         * Link the classes in the style struct. This will only work if the
         * element has been atttached to the document. Otherwise, the classes
         * will remain unresolved until the element is attached to a document.
         */
        ele_link_classes(ele);
}


WUT_API void wut_ApplyElementRise(struct wut_Element *ele, wut_ElementFunc fnc,
		void *data)
{
	struct wut_Element *run;
	struct wut_Element *next;

	if(!ele) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	run = ele;
	while(run) {
		next = run->parent;

		if(fnc(ele, data) == 1)
			return;

		run = next;
	}
}


WUT_API void wut_UpdateElementStyle(struct wut_Element *ele)
{
	struct wut_StylePass pass;

	if(!ele) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

        printf("Process element style for \"%s\":\n", ele->name);

        /* Then process the style */
	pass.document_shape = ele->document->shape_ref;
	wut_stl_process(&ele->style, &pass); 

	/* Then if the element has a widget, update that aswell */
	if(ele->widget) {
		wut_UpdateWidget(ele->widget, NULL);
	}
}


WUT_API void wut_UpdateElementChildrenShape(struct wut_Element *ele)
{
	if(!ele) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	switch(ele->style.layout_mode) {
		case WUT_KW_LAYOUT_BLOCK:	wut_lay_block(ele);	break;
		case WUT_KW_LAYOUT_ROW: 	wut_lay_row(ele); 	break;
		case WUT_KW_LAYOUT_COLUMN: 	wut_lay_column(ele);  break;
		default: break;
	}
}


WUT_API struct wut_Context *wut_GetElementContext(struct wut_Element *ele)
{
	if(!ele) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return NULL;
	}

	return ele->document->context;
}


WUT_API void wut_GetBoundingBox(struct wut_Element *ele, wut_iRect out)
{
	if(!ele) {
		wut_irect_clr(out);
		return;
	}

	wut_irect_mov(out, ele->style.shape_bounding_box, ele->absolute_offset);
}


WUT_API void wut_GetElementBox(struct wut_Element *ele, wut_iRect out)
{
	if(!ele) {
		wut_irect_clr(out);
                return;
	}

#if 0
	wut_irect_mov(out, ele->style.shape_bounding_box, ele->absolute_offset);
	wut_irect_add(out, out, ele->style.shape_element_delta);
#else
	wut_irect_cpy(out, ele->element_rect);
#endif
}


WUT_API void wut_GetContentBox(struct wut_Element *ele, wut_iRect out)
{
	if(!ele) {
                wut_irect_clr(out);
		return;
	}

	wut_irect_mov(out, ele->style.shape_bounding_box, ele->absolute_offset);
	wut_irect_add(out, out, ele->style.shape_content_delta);
}


WUT_API s8 wut_ModifyElementStyle(struct wut_Element *ele, char *str)
{
	if(!ele || !str) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

        /* Parse the incoming instructions and update the stylesheet */
	wut_ModifyStyle(&ele->style, str);

        /* Mark the document for updating */
        wut_doc_has_changed(ele->document, ele, 0, 0);

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to set attribute");
	return -1;
}


WUT_API struct wut_View *wut_GetView(struct wut_Element *ele)
{
	if(!ele) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(ele->type != WUT_VIEW || !ele->widget) {
		WUT_ALARM(WUT_ERROR, "Wrong element type");
		goto err_return;
	}

	return (struct wut_View *)ele->widget->ref;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to get view");
	return NULL;
}
