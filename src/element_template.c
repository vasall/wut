#include "element_template.h"

#include "alarm.h"
#include "system.h"

#include <stdlib.h>




FH_INTERN s8 fh_eletemp_load_body(struct fh_element *ele)
{
	fh_Ignore(ele);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_block(struct fh_element *ele)
{
	struct fh_style *style = &ele->style;
	struct fh_stylesheet *sheet = &style->sheet;

	/* 
	 * DISPLAY
	 */
	sheet->display_mode = FH_DISPLAY_BLOCK;	

	/*
	 * SIZE
	 */
	sheet->vsize = fh_flex_set(FH_FLEX_RELATIVE, 9000);
	sheet->vsize_min = fh_flex_set(FH_FLEX_RELATIVE, 0);
	sheet->vsize_max = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	sheet->hsize = fh_flex_set(FH_FLEX_RELATIVE, 9000);
	sheet->hsize_min = fh_flex_set(FH_FLEX_RELATIVE, 0);
	sheet->hsize_max = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * POSITION
	 */
	sheet->vorientation = FH_ORIENTATION_TOP;
	sheet->vposition = fh_flex_set(FH_FLEX_AUTO, 0);

	sheet->horientation = FH_ORIENTATION_LEFT;
	sheet->hposition = fh_flex_set(FH_FLEX_AUTO, 0);

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
	sheet->infill_color =  fh_col_set(0xff, 0xff, 0, 0xff);

	/*
	 * BORDER
	 */
	sheet->border_mode = FH_BORDER_NONE;


	return 0;
}


FH_INTERN s8 fh_eletemp_load_text(struct fh_element *ele)
{
	fh_Ignore(ele);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_button(struct fh_element *ele)
{
	fh_Ignore(ele);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_input(struct fh_element *ele)
{
	fh_Ignore(ele);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_image(struct fh_element *ele)
{
	fh_Ignore(ele);

	return 0;
}



FH_API s8 fh_eletemp_load(struct fh_element *ele)
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
		case FH_BODY: fh_eletemp_load_body(ele); break;
		case FH_BLOCK: fh_eletemp_load_block(ele); break;
		case FH_TEXT: fh_eletemp_load_text(ele); break;
		case FH_BUTTON: fh_eletemp_load_button(ele); break;
		case FH_INPUT: fh_eletemp_load_input(ele); break;
		case FH_IMAGE: fh_eletemp_load_image(ele); break;
		default:break;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to load element template");
	return -1;
}
