#include "model_constructor.h"

#include "system.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

#include <stdlib.h>
#include <stdint.h>

#define FH_MDLC_DEBUG 0


FH_INTERN u32 mdlc_type_size(GLenum type) {
	switch(type) {
		case GL_FLOAT: return sizeof(f32);
	}

	return 0;
}


FH_INTERN u32 mdlc_calc_stride(struct fh_model_c *c)
{
	u32 stride = 0;
	u8 i;
	struct fh_model_c_attrib *attr;

	for(i = 0; i < c->attrib_num; i++) {
		attr = &c->attribs[i];
		stride += (attr->elements * attr->element_size);
	}

	return stride;
}


FH_INTERN void mdlc_concate_data(struct fh_model *mdl, struct fh_model_c *c)
{
	u8 *ptr;
	u32 i;
	u32 j;
	struct fh_model_c_attrib *attr;
	u32 tmp = 0;

	ptr = mdl->vertex_buffer;
	
	/* Iterate through all vertices... */
	for(i = 0; i < mdl->vertex_number; i++) {
		/* ...and for every vertex write all attributes */
		for(j = 0; j < c->attrib_num; j++) {
			attr = &c->attribs[j];

			tmp = (attr->elements * attr->element_size);
			memcpy(ptr, attr->data + (tmp * i), tmp);

			ptr += tmp;
		}
	}
}


FH_INTERN void mdlc_create_buffers(struct fh_model *mdl)
{
	u32 size;

	/* Create the vertex-array-object */
	glGenVertexArrays(1, &mdl->vao);
	glBindVertexArray(mdl->vao);

	/* Create the buffer-array-object */
	glGenBuffers(1, &mdl->bao);
	glBindBuffer(GL_ARRAY_BUFFER, mdl->bao);
	size = mdl->vertex_number * mdl->vertex_stride;
	glBufferData(GL_ARRAY_BUFFER, size, mdl->vertex_buffer,
			GL_STATIC_DRAW);

	/* Create the element-buffer-object */
	glGenBuffers(1, &mdl->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mdl->ebo);
	size = mdl->index_number * U32_S;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, mdl->index_buffer,
			GL_STATIC_DRAW);


	glBindVertexArray(0);
}


FH_INTERN void mdlc_enable_attr(struct fh_model *mdl, struct fh_model_c *c)
{
	u32 i;
	struct fh_model_c_attrib *attr;
	u16 slot_lst[FH_MODEL_ATTRIB_LIM];
	s8 slot;
	void *p;
	u32 count;

	fh_zeros(slot_lst, FH_MODEL_ATTRIB_LIM * U16_S);

	for(i = 0; i < c->attrib_num; i++) {
		attr = &c->attribs[i];

		if((slot = fh_ShaderGetInputLoc(mdl->shader, attr->name)) < 0) {
			ALARM(ALARM_ERR, "Input variable not found");
			return;
		}

		slot_lst[slot] = i;
	}


	/* Bind the vertex-array-object */
	glBindVertexArray(mdl->vao);

	/* Bind the buffer-array-object */
	glBindBuffer(GL_ARRAY_BUFFER, mdl->bao);


	/*
	 * Enable the attributes.
	 */
	count = 0;
	for(i = 0; i < c->attrib_num; i++) {
		slot = slot_lst[i];
		attr = &c->attribs[i];

		p = (void *)((uintptr_t)(count * U8_S));

		glVertexAttribPointer(
				i,
				attr->elements,
				attr->type,
				GL_FALSE,
				mdl->vertex_stride,
				p
				);

		count += (attr->elements * attr->element_size);
	}
}

FH_INTERN s8 mdlc_init_uniforms(struct fh_model *mdl, struct fh_model_c *c)
{
	u32 i;
	s32 j;
	struct fh_model_c_unibuf *unibuf;
	struct fh_model_uniform *uniform;

	s32 slot;

	printf("For model %s\n", mdl->name);

	/* Bind the vertex-array-object */
	glBindVertexArray(mdl->vao);

	for(i = 0; i < c->unibuf_num; i++) {
		unibuf = &c->unibufs[i];
		uniform = &mdl->uniforms[i];

		/* First write everything to the uniform buffer */
		strcpy(uniform->name, unibuf->name);
		uniform->size = unibuf->size;
		if(!(uniform->data = fh_malloc(uniform->size))) {
			ALARM(ALARM_ERR, "Failed to allocate memory");
			goto err_free;
		}

		/* Then register it with OpenGL */
		glGenBuffers(1, &uniform->bao);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, NULL,
				GL_STATIC_DRAW);

		printf("Uniform %d: %s\n", i, uniform->name);

		/* Retrieve the binding location in the shader */
		if((slot = fh_ShaderGetUniformLoc(mdl->shader, uniform->name)) < 0) {
			ALARM(ALARM_ERR, "Uniform not found");
			goto err_free;
		}

		printf("Location: %d\n", slot);

		uniform->location = slot;
	}

	mdl->uniform_number = c->unibuf_num;


	return 0;

err_free:
	for(j = i - 1;j >= 0; j--) {
		uniform = &mdl->uniforms[j];
		glDeleteBuffers(1, &uniform->bao);
		fh_free(uniform->data);
	}


	ALARM(ALARM_ERR, "Failed to initialize uniform buffers");
	return -1;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_model_c *fh_BeginModelConstr(char *name,
		u32 vnum, u32 inum, u32 *idx)
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

	/* Reset configuration attributes */
	c->shader = NULL;
	c->texture = NULL;

	return c;

err_free_c:
	fh_free(c);

err_return:
	ALARM(ALARM_ERR, "Failed to begin creating new model");
	return NULL;


}


FH_API struct fh_model *fh_EndModelConstr(struct fh_model_c *c,
		struct fh_context *ctx, vec3_t pos, vec3_t rot)
{
	u32 i;

	struct fh_model *mdl;
	u32 tmp;

	u32 size;
	void **p;

	if(!c || !ctx) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(mdl = fh_malloc(sizeof(struct fh_model)))) {
		ALARM(ALARM_ERR, "Failed to alloctae memory for model");
		goto err_return;
	}

	
	/* Copy basic attributes */
	strcpy(mdl->name, c->name);
	
	/* Copy the resource references over */
	mdl->shader = c->shader;
	mdl->texture = c->texture;
	
	/*
	 * Copy the indices.
	 */

	mdl->index_number = c->idx_num;

	tmp = mdl->index_number * U32_S;
	if(!(mdl->index_buffer = fh_malloc(tmp))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for index buffer");
		goto err_free_mdl;
	}

	memcpy(mdl->index_buffer, c->idx, tmp);


	/*
	 * Set the basic vertex data attributes and allocate the buffer.
	 */

	mdl->vertex_number = c->vtx_num;
	mdl->vertex_stride = mdlc_calc_stride(c);

	tmp = mdl->vertex_stride * mdl->vertex_number;
	if(!(mdl->vertex_buffer = fh_malloc(tmp))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for vertex buffer");
		goto err_free_index_buffer;
	}

	/*
	 * Concate the vertex data in the following pattern:
	 *
	 *        vtx 0              vtx 1              vtx 2
	 *  < pos  tex  nrm >  < pos  tex  nrm >  < pos  tex  nrm >
	 */

	mdlc_concate_data(mdl, c);

	/*
	 * Create and initialize the data objects in OpenGL.
	 */
	mdlc_create_buffers(mdl);

	/*
	 * Enable the vertex attributes.
	 */
	mdlc_enable_attr(mdl, c);

	/*
	 * Create and initialize the uniform buffers.
	 */
	if(mdlc_init_uniforms(mdl, c) < 0)
		goto err_destroy_buffers;


	vec3_cpy(mdl->position, pos);
	vec3_cpy(mdl->rotation, rot);

	/* Reset view pointer */
	mdl->view = NULL;

	/* Insert the new model into the context model table */
	mdl->context = ctx;

	size = sizeof(struct fh_model);
	p = (void **)&mdl;

	if(fh_tbl_add(ctx->models, mdl->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to insert entry into fh_table");
		goto err_destroy_uniforms;
	}

	return mdl;

err_destroy_uniforms:
	for(i = 0; i < mdl->uniform_number; i++) {
		glDeleteBuffers(1, &mdl->uniforms[i].bao);
		fh_free(mdl->uniforms[i].data);	
	}

err_destroy_buffers:

	glDeleteBuffers(1, &mdl->ebo);
	glDeleteBuffers(1, &mdl->bao);
	glDeleteVertexArrays(1, &mdl->vao);

err_free_index_buffer:
	fh_free(mdl->index_buffer);

err_free_mdl:
	fh_free(mdl);

err_return:
	ALARM(ALARM_ERR, "Failed to convert constructor");
	return NULL;
}


FH_API void fh_ModelConstrCleanup(struct fh_model_c *c)
{
	u8 i;

	if(!c)
		return;

	for(i = 0; i < c->attrib_num; i++) {
		fh_free(c->attribs[i].data);
	}

	fh_free(c->idx);

	fh_free(c);
}


/*
 * GENERAL
 */

FH_API void fh_ModelConstrTexture(struct fh_model_c *c, struct fh_texture *tex)
{
	if(!c || !tex) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	c->texture = tex;
}



/*
 * CUSTOM MODE
 */

FH_API void fh_ModelConstrShader(struct fh_model_c *c, struct fh_shader *shd)
{
	if(!c || !shd) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	c->shader = shd;
}


FH_API void fh_ModelConstrAttrib(struct fh_model_c *c, char *name, u8 size,
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
	a->element_size = mdlc_type_size(type);
	a->type = type;

	s = c->vtx_num * a->elements * a->element_size;
	if(!(a->data = fh_malloc(s))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for attribute");
		return;
	}

	memcpy(a->data, data, s);

	c->attrib_num++;
}


FH_API void fh_ModelConstrUniform(struct fh_model_c *c, char *name, u32 size)
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
}
