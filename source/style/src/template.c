#include "style/inc/template.h"

#include "utility/alarm/inc/alarm.h"

#include "document/inc/document.h"
#include "document/inc/element.h"
#include "document/inc/context.h"

#include "widget/inc/widget.h"
#include "widget/inc/view.h"

#include "graphic/inc/camera.h"

#include <stdlib.h>




FH_INTERN s8 fh_eletemp_load_body(struct fh_element *ele, void *data)
{
	struct fh_style *style = &ele->style;
	struct fh_stylesheet *sheet = &style->sheet;

	fh_Ignore(data);

	/*
	 * DISPLAY
	 */
	fh_sheet_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	fh_sheet_parse(sheet, "width: 100pct;");
	fh_sheet_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	fh_sheet_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	fh_sheet_parse(sheet, "spacing_top: 0;");
	fh_sheet_parse(sheet, "spacing_right: 0;");
	fh_sheet_parse(sheet, "spacing_bottom: 0;");
	fh_sheet_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	fh_sheet_parse(sheet, "padding_top: 0;");
	fh_sheet_parse(sheet, "padding_right: 0;");
	fh_sheet_parse(sheet, "padding_bottom: 0;");
	fh_sheet_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	fh_sheet_parse(sheet, "border_mode: none;");
	fh_sheet_parse(sheet, "border_width: 0;");
	fh_sheet_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	fh_sheet_parse(sheet, "radius_top_left: 0;");
	fh_sheet_parse(sheet, "radius_top_right: 0;");
	fh_sheet_parse(sheet, "radius_bottom_right: 0;");
	fh_sheet_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	fh_sheet_parse(sheet, "infill_mode: color;");
	fh_sheet_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	*/
	fh_sheet_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	fh_sheet_parse(sheet, "align_v: top;");
	fh_sheet_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	fh_sheet_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	fh_sheet_parse(sheet, "text_size: 12px;");
	fh_sheet_parse(sheet, "text_color: #000000;");
	fh_sheet_parse(sheet, "text_mass: 50;");
	fh_sheet_parse(sheet, "text_options: none;");
	fh_sheet_parse(sheet, "text_wrap_mode: WORDWRAP;");

	return 0;
}


FH_INTERN s8 fh_eletemp_load_block(struct fh_element *ele, void *data)
{
	struct fh_style *style = &ele->style;
	struct fh_stylesheet *sheet = &style->sheet;

	fh_Ignore(data);

	/*
	 * DISPLAY
	 */
	fh_sheet_parse(sheet, "display_mode: visible;");

	/*
	 * SIZE
	 */
	fh_sheet_parse(sheet, "width: 100pct;");
	fh_sheet_parse(sheet, "height: 100pct;");

	/*
	 * REFERENCE
	 */
	fh_sheet_parse(sheet, "reference_mode: relative;");

	/*
	 * SPACING
	 */
	fh_sheet_parse(sheet, "spacing_top: 0;");
	fh_sheet_parse(sheet, "spacing_right: 0;");
	fh_sheet_parse(sheet, "spacing_bottom: 0;");
	fh_sheet_parse(sheet, "spacing_left: 0;");

	/*
	 * PADDING
	 */
	fh_sheet_parse(sheet, "padding_top: 0;");
	fh_sheet_parse(sheet, "padding_right: 0;");
	fh_sheet_parse(sheet, "padding_bottom: 0;");
	fh_sheet_parse(sheet, "padding_left: 0;");

	/*
	 * BORDER
	 */
	fh_sheet_parse(sheet, "border_mode: none;");
	fh_sheet_parse(sheet, "border_width: 0;");
	fh_sheet_parse(sheet, "border_color: #000000;");

	/*
	 * RADIUS
	 */
	fh_sheet_parse(sheet, "radius_top_left: 0;");
	fh_sheet_parse(sheet, "radius_top_right: 0;");
	fh_sheet_parse(sheet, "radius_bottom_right: 0;");
	fh_sheet_parse(sheet, "radius_bottom_left: 0;");

	/*
	 * INFILL
	 */
	fh_sheet_parse(sheet, "infill_mode: color;");
	fh_sheet_parse(sheet, "infill_color: #B00B1E;");

	/*
	 * LAYOUT
	*/
	fh_sheet_parse(sheet, "layout_mode: block;");

	/*
	 * ALIGNMENT
	 */
	fh_sheet_parse(sheet, "align_v: top;");
	fh_sheet_parse(sheet, "align_h: left;");

	/*
	 * SCROLLBAR
	 */
	fh_sheet_parse(sheet, "scrollbar_mode: auto;");

	/*
	 * TEXT
	 */
	fh_sheet_parse(sheet, "text_size: 12px;");
	fh_sheet_parse(sheet, "text_color: #000000;");
	fh_sheet_parse(sheet, "text_mass: 50;");
	fh_sheet_parse(sheet, "text_options: none;");
	fh_sheet_parse(sheet, "text_wrap_mode: WORDWRAP;");

	return 0;
}


FH_INTERN s8 fh_eletemp_load_text(struct fh_element *ele, void *data)
{
	fh_Ignore(ele);
	fh_Ignore(data);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_button(struct fh_element *ele, void *data)
{
	fh_Ignore(ele);
	fh_Ignore(data);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_input(struct fh_element *ele, void *data)
{
	fh_Ignore(ele);
	fh_Ignore(data);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_image(struct fh_element *ele, void *data)
{
	fh_Ignore(ele);
	fh_Ignore(data);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_view(struct fh_element *ele, void *data)
{
	struct fh_style *style = &ele->style;

	fh_Ignore(data);

	fh_Ignore(style);

	/*
	 * Create and initialize the View-Widget.
	 */
	if(!(ele->widget = fh_CreateWidget(ele, FH_WIDGET_VIEW, NULL)))
		return -1;


	return 0;
}


FH_API s8 fh_eletemp_load(struct fh_element *ele, void *data)
{
	if(!ele) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}


	/* 
	 * Just call the corresponding template loading function for the
	 * different element types.
	 */
	switch(ele->type) {
		case FH_BODY: fh_eletemp_load_body(ele, data); break;
		case FH_BLOCK: fh_eletemp_load_block(ele, data); break;
		case FH_TEXT: fh_eletemp_load_text(ele, data); break;
		case FH_BUTTON: fh_eletemp_load_button(ele, data); break;
		case FH_INPUT: fh_eletemp_load_input(ele, data); break;
		case FH_IMAGE: fh_eletemp_load_image(ele, data); break;
		case FH_VIEW: fh_eletemp_load_view(ele, data); break;
		default:break;
	}

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to load element template");
	return -1;
}
