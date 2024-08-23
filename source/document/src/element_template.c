#include "document/inc/element_template.h"

#include "utility/inc/alarm.h"

#include "document/inc/document.h"
#include "document/inc/element.h"

#include "style/inc/style.h"

#include "widget/inc/widget.h"
#include "widget/inc/view.h"

#include "graphic/inc/context.h"
#include "graphic/inc/camera.h"

#include <stdio.h>
#include <stdlib.h>




WUT_INTERN s8 wut_etm_load_body(struct wut_Element *ele, void *data)
{
	struct wut_Style *style = &ele->style;
	struct wut_Stylesheet *sheet = &style->sheet;

	WUT_IGNORE(data);

	/*
	 * DISPLAY
	 */
	wut_sht_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	wut_sht_parse(sheet, "width: 100pct;");
	wut_sht_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	wut_sht_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	wut_sht_parse(sheet, "spacing_top: 0;");
	wut_sht_parse(sheet, "spacing_right: 0;");
	wut_sht_parse(sheet, "spacing_bottom: 0;");
	wut_sht_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	wut_sht_parse(sheet, "padding_top: 0;");
	wut_sht_parse(sheet, "padding_right: 0;");
	wut_sht_parse(sheet, "padding_bottom: 0;");
	wut_sht_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	wut_sht_parse(sheet, "border_mode: none;");
	wut_sht_parse(sheet, "border_width: 0;");
	wut_sht_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	wut_sht_parse(sheet, "radius_top_left: 0;");
	wut_sht_parse(sheet, "radius_top_right: 0;");
	wut_sht_parse(sheet, "radius_bottom_right: 0;");
	wut_sht_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	wut_sht_parse(sheet, "infill_mode: color;");
	wut_sht_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	 */
	wut_sht_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	wut_sht_parse(sheet, "align_v: top;");
	wut_sht_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	wut_sht_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	wut_sht_parse(sheet, "text_size: 50px;");
	wut_sht_parse(sheet, "text_color: #000000;");
	wut_sht_parse(sheet, "text_mass: 50;");
	wut_sht_parse(sheet, "text_options: none;");
	wut_sht_parse(sheet, "text_wrap_mode: wordwrap;");
	wut_sht_parse(sheet, "text_spacing: 1.0;");
	wut_sht_parse(sheet, "line_height: 20px;");

	return 0;
}


WUT_INTERN s8 wut_etm_load_block(struct wut_Element *ele, void *data)
{
	struct wut_Style *style = &ele->style;
	struct wut_Stylesheet *sheet = &style->sheet;

	WUT_IGNORE(data);

	/*
	 * DISPLAY
	 */
	wut_sht_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	wut_sht_parse(sheet, "width: 100pct;");
	wut_sht_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	wut_sht_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	wut_sht_parse(sheet, "spacing_top: 0;");
	wut_sht_parse(sheet, "spacing_right: 0;");
	wut_sht_parse(sheet, "spacing_bottom: 0;");
	wut_sht_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	wut_sht_parse(sheet, "padding_top: 0;");
	wut_sht_parse(sheet, "padding_right: 0;");
	wut_sht_parse(sheet, "padding_bottom: 0;");
	wut_sht_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	wut_sht_parse(sheet, "border_mode: none;");
	wut_sht_parse(sheet, "border_width: 0;");
	wut_sht_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	wut_sht_parse(sheet, "radius_top_left: 0;");
	wut_sht_parse(sheet, "radius_top_right: 0;");
	wut_sht_parse(sheet, "radius_bottom_right: 0;");
	wut_sht_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	wut_sht_parse(sheet, "infill_mode: color;");
	wut_sht_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	 */
	wut_sht_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	wut_sht_parse(sheet, "align_v: top;");
	wut_sht_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	wut_sht_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	wut_sht_parse(sheet, "text_size: 12px;");
	wut_sht_parse(sheet, "text_color: #000000;");
	wut_sht_parse(sheet, "text_mass: 50;");
	wut_sht_parse(sheet, "text_options: none;");
	wut_sht_parse(sheet, "text_wrap_mode: WORDWRAP;");

	return 0;
}


WUT_INTERN s8 wut_etm_load_text(struct wut_Element *ele, void *data)
{
	/*
	 * Create and initialize the Image-Widget.
	 */
	if(!(ele->widget = wut_CreateWidget(ele, WUT_WIDGET_TEXT, data))) {
		return -1;
	}


	return 0;
}


WUT_INTERN s8 wut_etm_load_button(struct wut_Element *ele, void *data)
{
	WUT_IGNORE(ele);
	WUT_IGNORE(data);

	return 0;
}


WUT_INTERN s8 wut_etm_load_input(struct wut_Element *ele, void *data)
{
	WUT_IGNORE(ele);
	WUT_IGNORE(data);

	return 0;
}


WUT_INTERN s8 wut_etm_load_image(struct wut_Element *ele, void *data)
{
	struct wut_Style *style = &ele->style;
	struct wut_Stylesheet *sheet = &style->sheet;

	WUT_IGNORE(ele);
	WUT_IGNORE(data);

	/*
	 * DISPLAY
	 */
	wut_sht_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	wut_sht_parse(sheet, "width: 100pct;");
	wut_sht_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	wut_sht_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	wut_sht_parse(sheet, "spacing_top: 0;");
	wut_sht_parse(sheet, "spacing_right: 0;");
	wut_sht_parse(sheet, "spacing_bottom: 0;");
	wut_sht_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	wut_sht_parse(sheet, "padding_top: 0;");
	wut_sht_parse(sheet, "padding_right: 0;");
	wut_sht_parse(sheet, "padding_bottom: 0;");
	wut_sht_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	wut_sht_parse(sheet, "border_mode: none;");
	wut_sht_parse(sheet, "border_width: 0;");
	wut_sht_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	wut_sht_parse(sheet, "radius_top_left: 0;");
	wut_sht_parse(sheet, "radius_top_right: 0;");
	wut_sht_parse(sheet, "radius_bottom_right: 0;");
	wut_sht_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	wut_sht_parse(sheet, "infill_mode: color;");
	wut_sht_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	 */
	wut_sht_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	wut_sht_parse(sheet, "align_v: top;");
	wut_sht_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	wut_sht_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	wut_sht_parse(sheet, "text_size: 12px;");
	wut_sht_parse(sheet, "text_color: #000000;");
	wut_sht_parse(sheet, "text_mass: 50;");
	wut_sht_parse(sheet, "text_options: none;");
	wut_sht_parse(sheet, "text_wrap_mode: WORDWRAP;");

	/*
	 * Create and initialize the Image-Widget.
	 */
	if(!(ele->widget = wut_CreateWidget(ele, WUT_WIDGET_IMAGE, data))) {
		return -1;
	}

	return 0;
}


WUT_INTERN s8 wut_etm_load_view(struct wut_Element *ele, void *data)
{
	struct wut_Style *style = &ele->style;

	WUT_IGNORE(style);

	/*
	 * Create and initialize the View-Widget.
	 */
	if(!(ele->widget = wut_CreateWidget(ele, WUT_WIDGET_VIEW, data)))
		return -1;


	return 0;
}


WUT_API s8 wut_etm_load(struct wut_Element *ele, void *data)
{
	if(!ele) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/* 
	 * Just call the corresponding template loading function for the
	 * different element types.
	 */
	switch(ele->type) {
		case WUT_BODY: wut_etm_load_body(ele, data); break;
		case WUT_BLOCK: wut_etm_load_block(ele, data); break;
		case WUT_TEXT: wut_etm_load_text(ele, data); break;
		case WUT_BUTTON: wut_etm_load_button(ele, data); break;
		case WUT_INPUT: wut_etm_load_input(ele, data); break;
		case WUT_IMAGE: wut_etm_load_image(ele, data); break;
		case WUT_VIEW: wut_etm_load_view(ele, data); break;
		default:break;
	}

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load element template");
	return -1;
}
