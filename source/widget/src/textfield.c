#include "widget/inc/textfield.h"

#include "system/inc/system.h"

#include "document/inc/document.h"
#include "document/inc/element.h"

#include "utility/inc/alarm.h"

#include <stdlib.h>

#define TEXTFIELD_DEBUG 1

WUT_INTERN wut_iRect *tfd_get_limit(struct wut_Textfield *txtf)
{	
	return &txtf->element->content_rect;
}


WUT_XMOD struct wut_Textfield *wut_tfd_create(struct wut_Element *ele,
		struct wut_Font *font)
{
	struct wut_Textfield *txtf;
	struct wut_TextInfo info;
	char *teststring = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut l";

	if(!(txtf = wut_malloc(sizeof(struct wut_Textfield)))) {
		return NULL;
	}

	/* Configure the textfield */
	txtf->element = ele;
	strcpy(txtf->content, teststring);
	txtf->length = strlen(txtf->content);

	/* Configure the text information */
	info.batch = wut_ContextGetBatch(ele->document->context, font->batch_id);
	info.font = font;
	info.limits = tfd_get_limit(txtf);
	info.style = &ele->style;

	if(!(txtf->tbuffer = wut_txt_create(info))) {
		WUT_ALARM(WUT_ERROR, "Failed to create text buffer");
		goto err_free_txtf;
	}

	wut_txt_push(txtf->tbuffer, 0, txtf->length, txtf->content);

	return txtf;

err_free_txtf:
	wut_free(txtf);
	return NULL;
}


WUT_XMOD void wut_tfd_destroy(struct wut_Textfield *txtf)
{
	wut_txt_destroy(txtf->tbuffer);
	wut_free(txtf);
}


WUT_XMOD void wut_tfd_update(struct wut_Textfield *txtf)
{
	wut_txt_process(txtf->tbuffer);
}


WUT_XMOD void wut_tfd_render(struct wut_Textfield *txtf)
{
	wut_txt_send(txtf->tbuffer);
}
