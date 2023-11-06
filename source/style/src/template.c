#include "style/inc/template.h"

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

	fh_Ignore(data);

	/* Reset the stylesheet */
	fh_ResetStyle(style);

	return 0;
}


FH_INTERN s8 fh_eletemp_load_block(struct fh_element *ele, void *data)
{
	struct fh_style *style = &ele->style;

	fh_Ignore(data);

	/* Reset the stylesheet */
	fh_ResetStyle(style);

	/*
	 * INFILL
	 */
	style->sheet.infill_mode = FH_INFILL_COLOR;
	style->sheet.infill_color =  fh_col_set_u32(0xB0, 0x0B, 0x1E, 0xff);

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

	/* Reset the stylesheet */
	fh_ResetStyle(style);

	/*
	 * INFILL
	 */
	style->sheet.infill_mode = FH_INFILL_COLOR;
	style->sheet.infill_color =  fh_col_set_u32(0, 0, 0xFF, 0xFF);

	/*
	 * SCROLLBAR
	 */
	style->sheet.scrollbar_mode = FH_SCROLLBAR_NONE;

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
