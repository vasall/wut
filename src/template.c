#include "template.h"

#include "document.h"
#include "element.h"
#include "context.h"
#include "widget.h"
#include "view.h"
#include "camera.h"

#include <stdlib.h>




FH_INTERN s8 fh_eletemp_load_body(struct fh_element *ele, void *data)
{
	struct fh_style *style = &ele->style;
	struct fh_stylesheet *sheet = &style->sheet;

	fh_Ignore(data);

	/* 
	 * DISPLAY
	 */
	sheet->display_mode = FH_DISPLAY_VISIBLE;	

	/*
	 * REFERENCE
	 */
	sheet->reference_mode = FH_REFERENCE_RELATIVE;

	/*
	 * SIZE
	 */
	sheet->vsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->hsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * SPACING
	 */
	sheet->spacing_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * PADDING 
	 */
	sheet->padding_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * INFILL
	 */
	sheet->infill_mode = FH_INFILL_COLOR;
	sheet->infill_color =  fh_col_set(0, 0, 0xFF, 0xff);

	/*
	 * LAYOUT
	 */
	sheet->layout_mode = FH_LAYOUT_BLOCKS;

	/*
	 * SCROLLBAR
	 */
	sheet->scrollbar_mode = FH_SCROLLBAR_VERTICAL;

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
	sheet->display_mode = FH_DISPLAY_VISIBLE;	

	/*
	 * REFERENCE
	 */
	sheet->reference_mode = FH_REFERENCE_RELATIVE;

	/*
	 * SIZE
	 */
	sheet->vsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->hsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * SPACING
	 */
	sheet->spacing_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * PADDING 
	 */
	sheet->padding_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * INFILL
	 */
	sheet->infill_mode = FH_INFILL_COLOR;
	sheet->infill_color =  fh_col_set(0xFF, 0, 0, 0xff);

	/*
	 * LAYOUT
	 */
	sheet->layout_mode = FH_LAYOUT_BLOCKS;

	/*
	 * SCROLLBAR
	 */
	sheet->scrollbar_mode = FH_SCROLLBAR_VERTICAL;

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
	struct fh_stylesheet *sheet = &style->sheet;

	fh_Ignore(data);

	/* 
	 * DISPLAY
	 */
	sheet->display_mode = FH_DISPLAY_VISIBLE;	

	/*
	 * REFERENCE
	 */
	sheet->reference_mode = FH_REFERENCE_RELATIVE;

	/*
	 * SIZE
	 */
	sheet->vsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->hsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * SPACING
	 */
	sheet->spacing_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * PADDING 
	 */
	sheet->padding_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * INFILL
	 */
	sheet->infill_mode = FH_INFILL_COLOR;
	sheet->infill_color =  fh_col_set(0, 0, 0xFF, 0xff);

	/*
	 * LAYOUT
	 */
	sheet->layout_mode = FH_LAYOUT_BLOCKS;

	/*
	 * SCROLLBAR
	 */
	sheet->scrollbar_mode = FH_SCROLLBAR_VERTICAL;


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
		ALARM(ALARM_ERR, "Input parameters invalid");
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
	ALARM(ALARM_ERR, "Failed to load element template");
	return -1;
}
