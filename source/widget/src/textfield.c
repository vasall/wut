#include "widget/inc/textfield.h"

#include "system/inc/system.h"

#include "document/inc/document.h"
#include "document/inc/element.h"

#include "utility/inc/alarm.h"

#include <stdlib.h>

#define TEXTFIELD_DEBUG 1

WT_INTERN struct wt_rect *textfield_get_limit(struct wt_textfield *txtf)
{	
	return &txtf->element->content_rect;
}


WT_API struct wt_textfield *wt_textfield_create(struct wt_element *ele,
		struct wt_font *font)
{
	struct wt_textfield *txtf;
	struct wt_text_info info;
	char *teststring = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut l";

	if(!(txtf = wt_malloc(sizeof(struct wt_textfield)))) {
		return NULL;
	}

	/* Configure the textfield */
	txtf->element = ele;
	strcpy(txtf->content, teststring);
	txtf->length = strlen(txtf->content);

	/* Configure the text information */
	info.batch = wt_ContextGetBatch(ele->document->context, font->batch_id);
	info.font = font;
	info.limits = textfield_get_limit(txtf);
	info.style = &ele->style;

	if(!(txtf->tbuffer = wt_text_create(info))) {
		WT_ALARM(WT_ERROR, "Failed to create text buffer");
		goto err_free_txtf;
	}

	wt_text_push(txtf->tbuffer, 0, txtf->length, txtf->content);

	return txtf;

err_free_txtf:
	wt_free(txtf);
	return NULL;
}


WT_API void wt_textfield_destroy(struct wt_textfield *txtf)
{
	wt_text_destroy(txtf->tbuffer);
	wt_free(txtf);
}


WT_API void wt_textfield_update(struct wt_textfield *txtf)
{
	wt_text_process(txtf->tbuffer);
}


WT_API void wt_textfield_render(struct wt_textfield *txtf)
{
	wt_text_send(txtf->tbuffer);
}
