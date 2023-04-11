#include "model_constructor.h"

#include "alarm.h"
#include "system.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

#include <stdlib.h>
#include <stdint.h>

#define FH_MDLC_DEBUG 1

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


FH_INTERN void fh_mdlc_link_res(struct fh_model *mdl, struct fh_model_c *c)
{
	if(!(mdl->shader = fh_shd_get(c->shader))) {
		ALARM(ALARM_ERR, "Requested shader not found");
		return;
	}

	if(!(mdl->texture = fh_tex_get(c->texture))) {
		ALARM(ALARM_ERR, "Requested texture not found");
		return;
	}
	
#if FH_MDLC_DEBUG
	printf("Resources linked\n");
#endif
}

FH_INTERN u32 fh_mdlc_calc_stride(struct fh_model_c *c)
{
	u32 stride = 0;
	u8 i;
	struct fh_model_c_attrib *attr;

	for(i = 0; i < c->attrib_num; i++) {
		attr = &c->attribs[i];
		stride += (attr->elements * attr->element_size);
	}

#if FH_MDLC_DEBUG
	printf("Stride is %d\n", stride);
#endif

	return stride;

}

FH_INTERN void fh_mdlc_concate(struct fh_model *mdl, struct fh_model_c *c)
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


	printf("\n\n");
	printf("vertices (num %d, stride %d, size %d):\n",
			mdl->vertex_number, mdl->vertex_stride,
			mdl->vertex_number * mdl->vertex_stride);


	ptr = mdl->vertex_buffer;
	for(i = 0; i < mdl->vertex_number * mdl->vertex_stride; i++) {
		if(i % 16 == 0 && i != 0)
			printf("\n");

		printf("%02x ", (u8)*ptr);
		ptr++;
	}

	printf("\n\n");


#if FH_MDLC_DEBUG
	printf("Successfully concated vertex data\n");
#endif
}

FH_INTERN void fh_mdlc_create_objs(struct fh_model *mdl, struct fh_model_c *c)
{
	u32 size;

	/* SILENCIO */
	if(c) {}

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


#if FH_MDLC_DEBUG
	printf("Created VAO %d, BAO %d and EBO %d\n", mdl->vao, mdl->bao, mdl->ebo);
#endif

}

FH_INTERN void fh_mdlc_enable_attr(struct fh_model *mdl, struct fh_model_c *c)
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

		if((slot = fh_shd_loc(mdl->shader, attr->name)) < 0) {
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

#if FH_MDLC_DEBUG
		printf("Enable attribute %s(%d) at location %d\n", attr->name,
				i, slot);

		printf("The offset is %d\n", count * U8_S);
#endif

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

FH_INTERN void fh_mdlc_init_uniforms(struct fh_model *mdl, struct fh_model_c *c)
{
	u32 i;
	s32 j;
	struct fh_model_c_unibuf *unibuf;
	struct fh_model_uniform *uniform;	

	/* Bind the vertex-array-object */
	glBindVertexArray(mdl->vao);

	for(i = 0; i < c->unibuf_num; i++) {
		unibuf = &c->unibufs[i];
		uniform = &mdl->uniforms[i];

#if FH_MDLC_DEBUG
		printf("Initialize uniform %s\n", unibuf->name);
#endif


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

		printf("UNIFORM: %d\n", uniform->bao);
	}

	mdl->uniform_number = c->unibuf_num;


	return;

err_free:
	for(j = i - 1;j >= 0; j--) {
		uniform = &mdl->uniforms[j];
		fh_free(uniform->data);
	}
}


FH_API struct fh_model *fh_mdlc_finish(struct fh_model_c *c)
{
	struct fh_model *mdl;
	u32 tmp;
	u32 i;

	if(!c) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(mdl = fh_malloc(sizeof(struct fh_model)))) {
		ALARM(ALARM_ERR, "Failed to alloctae memory for model");
		goto err_return;
	}

	
	/* Copy basic attributes */
	strcpy(mdl->name, c->name);
	
	/*
	 * Link the shader and texture for this model.
	 */
	fh_mdlc_link_res(mdl, c);
	

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
	mdl->vertex_stride = fh_mdlc_calc_stride(c);

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

	fh_mdlc_concate(mdl, c);

	/*
	 * Create and initialize the data objects in OpenGL.
	 */
	fh_mdlc_create_objs(mdl, c);

	/*
	 * Enable the vertex attributes.
	 */
	fh_mdlc_enable_attr(mdl, c);

	/*
	 * Create and initialize the uniform buffers.
	 */
	fh_mdlc_init_uniforms(mdl, c);

	mdl->cam = fh_cam_create("main", 60, 800.0/600.0, 0.24, 1000.0);
	
	mat4_idt(mdl->unibuf.mpos);
	mat4_idt(mdl->unibuf.mrot);
	mat4_idt(mdl->unibuf.view);
	mat4_idt(mdl->unibuf.proj);

	mdl->x = 0;

	printf("\n\n");
	printf(">>> Indices (num %d):\n", mdl->index_number);

	for(i = 0; i < mdl->index_number; i++) {
		if(i % 16 == 0 && i != 0)
			printf("\n");

		printf("%02x ", mdl->index_buffer[i]);
	}
	printf("\n\n");


#if FH_MDLC_DEBUG
	printf("Successfully constructed model\n");
#endif

	return mdl;

err_free_index_buffer:
	fh_free(mdl->index_buffer);

err_free_mdl:
	fh_free(mdl);

err_return:
	ALARM(ALARM_ERR, "Failed to convert constructor");
	return NULL;
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
