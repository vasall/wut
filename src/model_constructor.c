#include "model_constructor.h"

#include "alarm.h"
#include "system.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

#include <stdlib.h>
#include <stdint.h>

#define FH_MDLC_DEBUG 0


FH_INTERN u32 fh_mdlc_type_size(GLenum type) {
	switch(type) {
		case GL_FLOAT: return sizeof(f32);
	}

	return 0;
}


FH_API struct fh_model_c *fh_mdlc_begin(char *name, u32 vnum,
		u32 inum, u32 *idx)
{
	struct fh_model_c *c;

	if(!name || vnum < 1 || inum < 1 || !idx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(c = fh_malloc(sizeof(struct fh_model_c)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for constructor");
		goto err_return;
	}

#if FH_MDLC_DEBUG
	printf("Start creating new model %s with %d vertices and %d indices\n",
			name, vnum, inum);
#endif


	/* Set the basic attributes */
	strcpy(c->name, name);
	c->vtx_num = vnum;
	c->idx_num = inum;
	c->attrib_num = 0;
	c->unibuf_num = 0;

	/* Copy the indices */
	if(!(c->idx = fh_malloc(inum * sizeof(u32)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for index buffer");
		goto err_free_c;
	}
	memcpy(c->idx, idx, inum * sizeof(u32));


	return c;

err_free_c:
	fh_free(c);

err_return:
	ALARM(ALARM_ERR, "Failed to begin creating new model");
	return NULL;
}


FH_API void fh_mdlc_shader(struct fh_model_c *c, char *shader)
{
	if(!c || !shader) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

#if FH_MDLC_DEBUG
	printf("Use shader %s\n", shader);
#endif

	strcpy(c->shader, shader);
}


FH_API void fh_mdlc_texture(struct fh_model_c *c, char *texture)
{
	if(!c || !texture) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

#if FH_MDLC_DEBUG
	printf("Use texture %s\n", texture);
#endif

	strcpy(c->texture, texture);
}


FH_API void fh_mdlc_attrib(struct fh_model_c *c, char *name, u8 size,
		GLenum type, void *data)
{
	struct fh_model_c_attrib *a;
	u32 s;

	if(!c || !name || size < 1 || !data) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	if(c->attrib_num + 1 > FH_MODEL_ATTRIB_LIM)
		return;

	a = &c->attribs[c->attrib_num];

	strcpy(a->name, name);
	a->elements = size;
	a->element_size = fh_mdlc_type_size(type);
	a->type = type;

	s = c->vtx_num * a->elements * a->element_size;
	if(!(a->data = fh_malloc(s))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for attribute");
		return;
	}

	memcpy(a->data, data, s);

	c->attrib_num++;
	
#if FH_MDLC_DEBUG

	printf("Set attribute %s with stride %d\n", 
			name, a->elements * a->element_size);
#endif
}


FH_API void fh_mdlc_uniform(struct fh_model_c *c, char *name, u32 size)
{
	struct fh_model_c_unibuf *u;

	if(!c || !name || size < 1) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	if(c->unibuf_num + 1 > FH_MODEL_UNIFORM_LIM)
		return;

	u = &c->unibufs[c->unibuf_num];

	strcpy(u->name, name);
	u->size = size;

	c->unibuf_num++;

#if FH_MDLC_DEBUG
	printf("Set uniform %s with size %d\n", name, size);
#endif
}


FH_API void fh_mdlc_destroy(struct fh_model_c *c)
{
	u32 i;

	if(!c)
		return;

	for(i = 0; i < c->attrib_num; i++) {
		fh_free(c->attribs[i].data);
	}

	fh_free(c->idx);

	fh_free(c);
}
