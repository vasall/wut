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




WT_INTERN s8 wt_eletemp_load_body(struct wt_element *ele, void *data)
{
	struct wt_style *style = &ele->style;
	struct wt_stylesheet *sheet = &style->sheet;

	wt_Ignore(data);

	/*
	 * DISPLAY
	 */
	wt_sheet_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	wt_sheet_parse(sheet, "width: 100pct;");
	wt_sheet_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	wt_sheet_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	wt_sheet_parse(sheet, "spacing_top: 0;");
	wt_sheet_parse(sheet, "spacing_right: 0;");
	wt_sheet_parse(sheet, "spacing_bottom: 0;");
	wt_sheet_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	wt_sheet_parse(sheet, "padding_top: 0;");
	wt_sheet_parse(sheet, "padding_right: 0;");
	wt_sheet_parse(sheet, "padding_bottom: 0;");
	wt_sheet_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	wt_sheet_parse(sheet, "border_mode: none;");
	wt_sheet_parse(sheet, "border_width: 0;");
	wt_sheet_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	wt_sheet_parse(sheet, "radius_top_left: 0;");
	wt_sheet_parse(sheet, "radius_top_right: 0;");
	wt_sheet_parse(sheet, "radius_bottom_right: 0;");
	wt_sheet_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	wt_sheet_parse(sheet, "infill_mode: color;");
	wt_sheet_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	 */
	wt_sheet_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	wt_sheet_parse(sheet, "align_v: top;");
	wt_sheet_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	wt_sheet_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	wt_sheet_parse(sheet, "text_size: 50px;");
	wt_sheet_parse(sheet, "text_color: #000000;");
	wt_sheet_parse(sheet, "text_mass: 50;");
	wt_sheet_parse(sheet, "text_options: none;");
	wt_sheet_parse(sheet, "text_wrap_mode: wordwrap;");
	wt_sheet_parse(sheet, "text_spacing: 1.0;");
	wt_sheet_parse(sheet, "line_height: 20px;");

	return 0;
}


WT_INTERN s8 wt_eletemp_load_block(struct wt_element *ele, void *data)
{
	struct wt_style *style = &ele->style;
	struct wt_stylesheet *sheet = &style->sheet;

	wt_Ignore(data);

	/*
	 * DISPLAY
	 */
	wt_sheet_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	wt_sheet_parse(sheet, "width: 100pct;");
	wt_sheet_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	wt_sheet_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	wt_sheet_parse(sheet, "spacing_top: 0;");
	wt_sheet_parse(sheet, "spacing_right: 0;");
	wt_sheet_parse(sheet, "spacing_bottom: 0;");
	wt_sheet_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	wt_sheet_parse(sheet, "padding_top: 0;");
	wt_sheet_parse(sheet, "padding_right: 0;");
	wt_sheet_parse(sheet, "padding_bottom: 0;");
	wt_sheet_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	wt_sheet_parse(sheet, "border_mode: none;");
	wt_sheet_parse(sheet, "border_width: 0;");
	wt_sheet_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	wt_sheet_parse(sheet, "radius_top_left: 0;");
	wt_sheet_parse(sheet, "radius_top_right: 0;");
	wt_sheet_parse(sheet, "radius_bottom_right: 0;");
	wt_sheet_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	wt_sheet_parse(sheet, "infill_mode: color;");
	wt_sheet_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	 */
	wt_sheet_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	wt_sheet_parse(sheet, "align_v: top;");
	wt_sheet_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	wt_sheet_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	wt_sheet_parse(sheet, "text_size: 12px;");
	wt_sheet_parse(sheet, "text_color: #000000;");
	wt_sheet_parse(sheet, "text_mass: 50;");
	wt_sheet_parse(sheet, "text_options: none;");
	wt_sheet_parse(sheet, "text_wrap_mode: WORDWRAP;");

	return 0;
}


WT_INTERN s8 wt_eletemp_load_text(struct wt_element *ele, void *data)
{
	/*
	 * Create and initialize the Image-Widget.
	 */
	if(!(ele->widget = wt_CreateWidget(ele, WT_WIDGET_TEXT, data))) {
		return -1;
	}


	return 0;
}


WT_INTERN s8 wt_eletemp_load_button(struct wt_element *ele, void *data)
{
	wt_Ignore(ele);
	wt_Ignore(data);

	return 0;
}


WT_INTERN s8 wt_eletemp_load_input(struct wt_element *ele, void *data)
{
	wt_Ignore(ele);
	wt_Ignore(data);

	return 0;
}


WT_INTERN s8 wt_eletemp_load_image(struct wt_element *ele, void *data)
{
	wt_Ignore(ele);
	wt_Ignore(data);

	struct wt_style *style = &ele->style;
	struct wt_stylesheet *sheet = &style->sheet;

	wt_Ignore(data);

	/*
	 * DISPLAY
	 */
	wt_sheet_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	wt_sheet_parse(sheet, "width: 100pct;");
	wt_sheet_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	wt_sheet_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	wt_sheet_parse(sheet, "spacing_top: 0;");
	wt_sheet_parse(sheet, "spacing_right: 0;");
	wt_sheet_parse(sheet, "spacing_bottom: 0;");
	wt_sheet_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	wt_sheet_parse(sheet, "padding_top: 0;");
	wt_sheet_parse(sheet, "padding_right: 0;");
	wt_sheet_parse(sheet, "padding_bottom: 0;");
	wt_sheet_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	wt_sheet_parse(sheet, "border_mode: none;");
	wt_sheet_parse(sheet, "border_width: 0;");
	wt_sheet_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	wt_sheet_parse(sheet, "radius_top_left: 0;");
	wt_sheet_parse(sheet, "radius_top_right: 0;");
	wt_sheet_parse(sheet, "radius_bottom_right: 0;");
	wt_sheet_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	wt_sheet_parse(sheet, "infill_mode: color;");
	wt_sheet_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	 */
	wt_sheet_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	wt_sheet_parse(sheet, "align_v: top;");
	wt_sheet_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	wt_sheet_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	wt_sheet_parse(sheet, "text_size: 12px;");
	wt_sheet_parse(sheet, "text_color: #000000;");
	wt_sheet_parse(sheet, "text_mass: 50;");
	wt_sheet_parse(sheet, "text_options: none;");
	wt_sheet_parse(sheet, "text_wrap_mode: WORDWRAP;");

	/*
	 * Create and initialize the Image-Widget.
	 */
	if(!(ele->widget = wt_CreateWidget(ele, WT_WIDGET_IMAGE, data))) {
		return -1;
	}

	return 0;
}


WT_INTERN s8 wt_eletemp_load_view(struct wt_element *ele, void *data)
{
	struct wt_style *style = &ele->style;

	wt_Ignore(style);

	/*
	 * Create and initialize the View-Widget.
	 */
	if(!(ele->widget = wt_CreateWidget(ele, WT_WIDGET_VIEW, data)))
		return -1;


	return 0;
}


WT_API s8 wt_eletemp_load(struct wt_element *ele, void *data)
{
	if(!ele) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	/* 
	 * Just call the corresponding template loading function for the
	 * different element types.
	 */
	switch(ele->type) {
		case WT_BODY: wt_eletemp_load_body(ele, data); break;
		case WT_BLOCK: wt_eletemp_load_block(ele, data); break;
		case WT_TEXT: wt_eletemp_load_text(ele, data); break;
		case WT_BUTTON: wt_eletemp_load_button(ele, data); break;
		case WT_INPUT: wt_eletemp_load_input(ele, data); break;
		case WT_IMAGE: wt_eletemp_load_image(ele, data); break;
		case WT_VIEW: wt_eletemp_load_view(ele, data); break;
		default:break;
	}

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to load element template");
	return -1;
}
