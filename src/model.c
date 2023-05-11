#include "model.h"

#include "alarm.h"
#include "system.h"
#include "core.h"
#include "table.h"

#include <stdlib.h>

#define FH_MDL_DEBUG 1


FH_API s8 fh_mdl_init(struct fh_window *win)
{
	struct fh_table *tbl;

	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tbl = fh_tbl_create(&fh_mdl_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create fh_table for models");
		goto err_return;
	}

	/* Attach the model table to the window */
	win->models = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to create new model table.");
	return -1;
}


FH_API void fh_mdl_close(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_WARN, "Input paramters invalid");
		return;
	}

	fh_tbl_destroy(win->models);
	win->models = NULL;
}


FH_INTERN s8 fh_mdl_link_resources(struct fh_window *win, 
		struct fh_model *mdl, struct fh_model_c *c)
{
	if(!(mdl->shader = fh_shd_get(win, c->shader))) {
		ALARM(ALARM_ERR, "Requested shader not found");
		goto err_return;
	}

	if(!(mdl->texture = fh_tex_get(win, c->texture))) {
		ALARM(ALARM_ERR, "Requested texture not found");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to link model resources");
	return -1;
}

FH_INTERN u32 fh_mdl_calc_stride(struct fh_model_c *c)
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

FH_INTERN void fh_mdl_concate_data(struct fh_model *mdl, struct fh_model_c *c)
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

FH_INTERN void fh_mdl_create_buffers(struct fh_model *mdl)
{
	u32 size;

	/* Create the vertex-array-object */
	glGenVertexArrays(1, &mdl->vao);
	glBindVertexArray(mdl->vao);

	printf("mdl vao: %d\n", mdl->vao);

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

#if FH_MDLC_DEBUG
	printf("Created VAO %d, BAO %d and EBO %d\n", mdl->vao, mdl->bao, mdl->ebo);
#endif

}

FH_INTERN void fh_mdl_enable_attr(struct fh_model *mdl, struct fh_model_c *c)
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

FH_INTERN s8 fh_mdl_init_uniforms(struct fh_model *mdl, struct fh_model_c *c)
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


FH_API struct fh_model *fh_mdl_create(struct fh_window *win,
		struct fh_model_c *c)
{
	struct fh_model *mdl;
	u32 tmp;

	if(!win || !c) {
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
	if(fh_mdl_link_resources(win, mdl, c) < 0)
		goto err_free_mdl;
	
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
	mdl->vertex_stride = fh_mdl_calc_stride(c);

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

	fh_mdl_concate_data(mdl, c);

	/*
	 * Create and initialize the data objects in OpenGL.
	 */
	fh_mdl_create_buffers(mdl);

	/*
	 * Enable the vertex attributes.
	 */
	fh_mdl_enable_attr(mdl, c);

	/*
	 * Create and initialize the uniform buffers.
	 */
	if(fh_mdl_init_uniforms(mdl, c) < 0)
		goto err_destroy_buffers;

#if FH_MDLC_DEBUG
	printf("Successfully constructed model\n");
#endif

	return mdl;

err_destroy_buffers:
	glBindVertexArray(mdl->vao);

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




FH_API void fh_mdl_destroy(struct fh_model *mdl)
{
	u32 i;
	struct fh_model_uniform *uniform;

	if(!mdl)
		return;

	glBindVertexArray(mdl->vao);

	for(i = 0; i < mdl->uniform_number; i++) {
		uniform =  &mdl->uniforms[i];
		glDeleteBuffers(1, &uniform->bao);
		fh_free(uniform->data);	
	}

	glDeleteBuffers(1, &mdl->ebo);
	glDeleteBuffers(1, &mdl->bao);
	glDeleteVertexArrays(1, &mdl->vao);

	fh_free(mdl);

}


FH_API s8 fh_mdl_insert(struct fh_window *win, struct fh_model *mdl)
{
	u32 size;
	void **p;

	if(!win || !mdl) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct fh_model);
	p = (void **)&mdl;

	if(fh_tbl_add(win->models, mdl->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to insert entry into fh_table");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert new model into model table");
	return -1;

}


FH_API void fh_mdl_remove(struct fh_window *win, struct fh_model *mdl)
{
	if(!win || !mdl) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_tbl_rmv(win->models, mdl->name);

}


FH_API struct fh_model *fh_mdl_get(struct fh_window *win, char *name)
{
	struct fh_model *mdl;

	if(!win || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(win->models, name, NULL, (void **)&mdl) != 1) {
		ALARM(ALARM_ERR, "Model could not be found in fh_table");
		goto err_return;
	}

	return mdl;

err_return:
	ALARM(ALARM_ERR, "Failed to get model from model table");
	return NULL;


}


FH_API void fh_mdl_set_uniform(struct fh_model *mdl, char *name, void *ptr)
{
	u32 i;
	struct fh_model_uniform *uniform;

	if(!mdl || !name || !ptr) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	for(i = 0; i < mdl->uniform_number; i++) {
		uniform = &mdl->uniforms[i];

		if(strcmp(uniform->name, name) == 0) {
			memcpy(uniform->data, ptr, uniform->size);
			return;
		}
	}

	ALARM(ALARM_ERR, "Uniform buffer not found");
}


FH_INTERN void fh_mdl_activate_uniforms(struct fh_model *mdl)
{
	u32 i;
	struct fh_model_uniform *uniform;

	if(!mdl)
		return;

	for(i = 0; i < mdl->uniform_number; i++) {
		uniform = &mdl->uniforms[i];

		glBindBuffer(GL_UNIFORM_BUFFER, uniform->bao);
		glBufferData(GL_UNIFORM_BUFFER, uniform->size, uniform->data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform->bao);
	}
}


FH_API void fh_mdl_render(struct fh_model *mdl)
{
	if(!mdl)
		return;

	printf("Activate VAO %d\n", mdl->vao);

	/* Activate the vertex-array-object */
	glBindVertexArray(mdl->vao);

	printf("Use shader %s\n", mdl->shader->name);

	/* Activate shader */
	fh_shd_use(mdl->shader);

	printf("Activate uniforms\n");

	/* Activate the uniform buffers */
	fh_mdl_activate_uniforms(mdl);

	printf("Use texture %s\n", mdl->texture->name);

	/* Activate texture */
	fh_tex_use(mdl->texture);

	/* Finally render the model */
	glDrawElements(GL_TRIANGLES, mdl->index_number, GL_UNSIGNED_INT, NULL);

	printf("Unuse texture and shader\n");

	/* Unuse the active texture and shader */
	fh_tex_unuse();
	fh_shd_unuse();

	printf("Unuse VAO\n");

	/* Unbind the vertex-array-object */
	glBindVertexArray(0);
}


FH_API void fh_mdl_rmv_fnc(u32 size, void *ptr)
{
	struct fh_model *mdl;

	/* SILENCIO! */
	if(size) {}

	if(!ptr)
		return;

	mdl = (struct fh_model *)ptr;

	fh_mdl_destroy(mdl);
}
