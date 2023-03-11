#include "element_template.h"

#include <stdlib.h>




FH_INTERN s8 fh_eletemp_load_body(struct fh_element *ele)
{
	/* SILENCIO! */
	if(ele) {}

	return 0;
}


FH_INTERN s8 fh_eletemp_load_block(struct fh_element *ele)
{
	/* SILENCIO! */
	if(ele) {}

	return 0;
}


FH_INTERN s8 fh_eletemp_load_text(struct fh_element *ele)
{
	/* SILENCIO! */
	if(ele) {}

	return 0;
}


FH_INTERN s8 fh_eletemp_load_button(struct fh_element *ele)
{
	/* SILENCIO! */
	if(ele) {}

	return 0;
}


FH_INTERN s8 fh_eletemp_load_input(struct fh_element *ele)
{
	/* SILENCIO! */
	if(ele) {}

	return 0;
}


FH_INTERN s8 fh_eletemp_load_image(struct fh_element *ele)
{
	/* SILENCIO! */
	if(ele) {}

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
