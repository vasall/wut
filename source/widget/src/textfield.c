#include "widget/inc/textfield.h"

#include "system/inc/system.h"

#include "document/inc/document.h"
#include "document/inc/element.h"

#include "utility/inc/alarm.h"

#include <stdlib.h>

#define TEXTFIELD_DEBUG 1

FH_INTERN struct fh_rect *textfield_get_limit(struct fh_textfield *txtf)
{	
	return &txtf->element->content_rect;
}


FH_API struct fh_textfield *fh_textfield_create(struct fh_element *ele,
		struct fh_font *font)
{
	struct fh_textfield *txtf;
	struct fh_text_info info;
	char *teststring = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut l";

	if(!(txtf = fh_malloc(sizeof(struct fh_textfield)))) {
		return NULL;
	}

	/* Configure the textfield */
	txtf->element = ele;
	strcpy(txtf->content, teststring);
	txtf->length = strlen(txtf->content);

	/* Configure the text information */
	info.batch = fh_ContextGetBatch(ele->document->context, font->batch_id);
	info.font = font;
	info.limits = textfield_get_limit(txtf);
	info.style = &ele->style;

	if(!(txtf->tbuffer = fh_text_create(info))) {
		FH_ALARM(FH_ERROR, "Failed to create text buffer");
		goto err_free_txtf;
	}

	fh_text_push(txtf->tbuffer, 0, txtf->length, txtf->content);

	return txtf;

err_free_txtf:
	fh_free(txtf);
	return NULL;
}


FH_API void fh_textfield_destroy(struct fh_textfield *txtf)
{
	fh_text_destroy(txtf->tbuffer);
	fh_free(txtf);
}


FH_API void fh_textfield_update(struct fh_textfield *txtf)
{
	fh_text_process(txtf->tbuffer);
}


FH_API void fh_textfield_render(struct fh_textfield *txtf)
{
	fh_text_send(txtf->tbuffer);
}
