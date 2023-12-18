#include "graphic/batching/inc/batch.h"

#include "system/inc/system.h"

#include "window/subsystems/inc/opengl.h"

FH_INTERN size_t batch_sizeof_GLenum(GLenum type)
{
	switch(type)
	{
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			return sizeof(GLshort);
		case GL_INT_2_10_10_10_REV:
		case GL_INT:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
		case GL_UNSIGNED_INT:
			return sizeof(GLint);
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_DOUBLE:
			return sizeof(GLdouble);
		case GL_FIXED:
			return sizeof(GLfixed);
		case GL_HALF_FLOAT:
			return sizeof(GLhalf);
	}

	return 0;
}


FH_INTERN size_t batch_sizeof_UniformType(enum fh_uniform_type type)
{
	switch(type) {
		case FH_UNIFORM_1F: return sizeof(GLfloat);
		case FH_UNIFORM_2F: return 2*sizeof(GLfloat);
		case FH_UNIFORM_3F: return 3*sizeof(GLfloat);
		case FH_UNIFORM_4F: return 4*sizeof(GLfloat);

		case FH_UNIFORM_1I: return sizeof(GLint);
		case FH_UNIFORM_2I: return 2*sizeof(GLint);
		case FH_UNIFORM_3I: return 3*sizeof(GLint);
		case FH_UNIFORM_4I: return 4*sizeof(GLint);

		case FH_UNIFORM_1UI: return sizeof(GLuint);
		case FH_UNIFORM_2UI: return 2*sizeof(GLuint);
		case FH_UNIFORM_3UI: return 3*sizeof(GLuint);
		case FH_UNIFORM_4UI: return 4*sizeof(GLuint);

		case FH_UNIFORM_1FV: return sizeof(GLfloat);
		case FH_UNIFORM_2FV: return 2*sizeof(GLfloat);
		case FH_UNIFORM_3FV: return 3*sizeof(GLfloat);
		case FH_UNIFORM_4FV: return 4*sizeof(GLfloat);

		case FH_UNIFORM_1IV: return sizeof(GLint);
		case FH_UNIFORM_2IV: return 2*sizeof(GLint);
		case FH_UNIFORM_3IV: return 3*sizeof(GLint);
		case FH_UNIFORM_4IV: return 4*sizeof(GLint);

		case FH_UNIFORM_1UIV: return sizeof(GLuint);
		case FH_UNIFORM_2UIV: return 2*sizeof(GLuint);
		case FH_UNIFORM_3UIV: return 3*sizeof(GLuint);
		case FH_UNIFORM_4UIV: return 4*sizeof(GLuint);

		case FH_UNIFORM_M2FV: return 4*sizeof(GLfloat);
		case FH_UNIFORM_M3FV: return 9*sizeof(GLfloat);
		case FH_UNIFORM_M4FV: return 16*sizeof(GLfloat);
	}

	return 0;
}

FH_INTERN void batch_write_uniform(struct fh_uniform *uniform)
{
	u32 count;

	if(uniform->flags & FH_UNIFORM_F_ALL) {
		count = uniform->limit;
	}
	else {
		count = uniform->number;
	}


	switch(uniform->type) {
		case FH_UNIFORM_1F:
			glUniform1f(uniform->location, 
					((GLfloat *)uniform->data)[0]);
			return;
		case FH_UNIFORM_2F:
			glUniform2f(uniform->location, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1]);
			return;
		case FH_UNIFORM_3F:
			glUniform3f(uniform->location, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1],
					((GLfloat *)uniform->data)[2]);
			return;
		case FH_UNIFORM_4F:
			glUniform4f(uniform->location, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1],
					((GLfloat *)uniform->data)[2],
					((GLfloat *)uniform->data)[3]);
			return;

		case FH_UNIFORM_1I:
			glUniform1i(uniform->location, 
					((GLint *)uniform->data)[0]);
			return;
		case FH_UNIFORM_2I:
			glUniform2i(uniform->location, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1]);
			return;
		case FH_UNIFORM_3I:
			glUniform3i(uniform->location, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1],
					((GLint *)uniform->data)[2]);
			return;
		case FH_UNIFORM_4I:
			glUniform4i(uniform->location, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1],
					((GLint *)uniform->data)[2],
					((GLint *)uniform->data)[3]);
			return;

		case FH_UNIFORM_1UI:
			glUniform1ui(uniform->location, 
					((GLuint *)uniform->data)[0]);
			return;
		case FH_UNIFORM_2UI:
			glUniform2ui(uniform->location, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1]);
			return;
		case FH_UNIFORM_3UI:
			glUniform3ui(uniform->location, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1],
					((GLuint *)uniform->data)[2]);
			return;
		case FH_UNIFORM_4UI:
			glUniform4ui(uniform->location, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1],
					((GLuint *)uniform->data)[2],
					((GLuint *)uniform->data)[3]);
			return;

		case FH_UNIFORM_1FV:
			glUniform1fv(uniform->location,
					count, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_2FV:
			glUniform2fv(uniform->location,
					count, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_3FV:
			glUniform3fv(uniform->location,
					count, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_4FV:
			glUniform4fv(uniform->location,
					count, (GLfloat *)uniform->data);
			return;

		case FH_UNIFORM_1IV:
			glUniform1iv(uniform->location,
					count, (GLint *)uniform->data);
			return;
		case FH_UNIFORM_2IV:
			glUniform2iv(uniform->location,
					count, (GLint *)uniform->data);
			return;
		case FH_UNIFORM_3IV:
			glUniform3iv(uniform->location,
					count, (GLint *)uniform->data);
			return;
		case FH_UNIFORM_4IV:
			glUniform4iv(uniform->location,
					count, (GLint *)uniform->data);
			return;

		case FH_UNIFORM_1UIV:
			glUniform1uiv(uniform->location,
					count, (GLuint *)uniform->data);
			return;
		case FH_UNIFORM_2UIV:
			glUniform1uiv(uniform->location,
					count, (GLuint *)uniform->data);
			return;
		case FH_UNIFORM_3UIV:
			glUniform1uiv(uniform->location,
					count, (GLuint *)uniform->data);
			return;
		case FH_UNIFORM_4UIV:
			glUniform1uiv(uniform->location,
					count, (GLuint *)uniform->data);
			return;

		case FH_UNIFORM_M2FV:
			glUniformMatrix2fv(uniform->location, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_M3FV:
			glUniformMatrix3fv(uniform->location, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_M4FV:
			glUniformMatrix4fv(uniform->location, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
	}
}





FH_XMOD struct fh_batch *fh_batch_create(struct fh_shader *shd, u32 attribnum, 
		struct fh_vertex_attrib *attribs, u32 vlimit, u32 ilimit,
		u32 uninum, struct fh_uniform_temp *unis)
{
	u32 i;
	u32 vsize;
	u32 offset;
	u32 temp;

	struct fh_batch *ren;

	if(attribnum < 1 || vlimit < 1 || ilimit < 1) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return NULL;
	}

	/* Allocate the batch renderer */
	if(!(ren = fh_malloc(sizeof(struct fh_batch)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for batch");
		goto err_return;
	}

	/* Calculate the size of a single vertex */
	vsize = 0;
	for(i = 0; i < attribnum; i++) {
		temp = batch_sizeof_GLenum(attribs[i].type);
		vsize += temp * attribs[i].number;
	}

	/*  Create the Vertex Array Object */
	glGenVertexArrays(1, &ren->gl_objects[0]);
	glBindVertexArray(ren->gl_objects[0]);


	glGenBuffers(2, &ren->gl_objects[1]);

	/* Create and initialize the Vertex Buffer Object */
	glBindBuffer(GL_ARRAY_BUFFER, ren->gl_objects[1]);
	glBufferData(GL_ARRAY_BUFFER, vsize * vlimit, NULL, GL_DYNAMIC_DRAW);

	/* Create and initialize the Index Buffer Object */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->gl_objects[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, U32_S * ilimit, NULL,
			GL_STREAM_DRAW);

	/* Enable and configure vertex data */
	offset = 0;
	for(i = 0; i < attribnum; i++) {
		glEnableVertexAttribArray(i);

		if(attribs[i].type == GL_INT) {
			glVertexAttribIPointer(
					i,
					attribs[i].number,
					attribs[i].type,
					vsize,
					(void *)(offset)
					);
		}
		else {
			glVertexAttribPointer(
					i,
					attribs[i].number,
					attribs[i].type,
					GL_FALSE,
					vsize,
					(void *)(offset)
					);
		}

		temp = batch_sizeof_GLenum(attribs[i].type);
		offset += attribs[i].number * temp;
	}

	/* Copy data into batch renderer and allocate memory */
	ren->shader = shd;

	ren->vertex_size = vsize;
	ren->vertex_number = 0;
	ren->vertex_limit = vlimit;
	if(!(ren->vertices = fh_malloc(vsize * vlimit))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for vertices");
		goto err_delete_glObjects;
	}

	ren->index_limit = ilimit;
	ren->index_number = 0;
	if(!(ren->indices = fh_malloc(U32_S * ilimit))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for indices");
		goto err_free_vertices;
	}

	/* Configure uniform data */
	if(!(ren->uniforms = fh_malloc(sizeof(struct fh_uniform) * uninum))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for uniforms");
		goto err_free_indices;
	}

	for(i = 0; i < uninum; i++) {
		temp = glGetUniformLocation(shd->program, unis[i].name);
		ren->uniforms[i].location = temp;
		ren->uniforms[i].type = unis[i].type;
		ren->uniforms[i].size = batch_sizeof_UniformType(unis[i].type);
		ren->uniforms[i].number = 0;
		ren->uniforms[i].limit = unis[i].limit;
		temp = ren->uniforms[i].size * ren->uniforms[i].limit;	
		if(!(ren->uniforms[i].data = fh_malloc(temp))) {
			ALARM(ALARM_ERR, "Failed to allocate uniform data");
			break;
		}
		ren->uniforms[i].flags = unis[i].flags;
	}
	if(i != uninum)
		goto err_free_uniforms;

	ren->uniform_number = uninum;

	return ren;

err_free_uniforms:
	i--;
	do {
		fh_free(ren->uniforms[i].data);
	} while(i-- != 0);
	fh_free(ren->uniforms);

err_free_indices:
	fh_free(ren->indices);

err_free_vertices:
	fh_free(ren->vertices);

err_delete_glObjects:
	glDeleteBuffers(2, &ren->gl_objects[1]);
	glDeleteVertexArrays(1, &ren->gl_objects[0]);

	/* err_free_ren: */
	fh_free(ren);

err_return:
	return NULL;
}


FH_XMOD s32 fh_batch_push_vertex(struct fh_batch *ren, void *ptr)
{
	u32 offset = ren->vertex_size * ren->vertex_number;

	if(ren->vertex_number >= ren->vertex_limit) {
		ALARM(ALARM_WARN, "Vertex limit has been reached");
		return -1;
	}

	memcpy(ren->vertices + offset, ptr, ren->vertex_size);
	ren->vertex_number++;

	return (ren->vertex_number - 1);
}


FH_XMOD s32 fh_batch_push_index(struct fh_batch *ren, u32 idx)
{
	if(ren->index_number >= ren->index_limit) {
		ALARM(ALARM_ERR, "Index limit has been reached");
		return -1;
	}

	ren->indices[ren->index_number] = idx;
	ren->index_number++;

	return (ren->index_number - 1);
}


FH_XMOD s32 fh_batch_push_uniform(struct fh_batch *ren, u32 slot, void *ptr)
{
	u32 offset = ren->uniforms[slot].size * ren->uniforms[slot].number;

	if(ren->uniforms[slot].number >= ren->uniforms[slot].limit) {
		ALARM(ALARM_ERR, "Uniform entry limit has been reached");
		return -1;
	}

	memcpy(ren->uniforms[slot].data + offset, ptr, ren->uniforms[slot].size);
	ren->uniforms[slot].number++;

	return (ren->uniforms[slot].number - 1);
}


FH_XMOD void fh_batch_reset_uniform(struct fh_batch *ren, u32 slot)
{
	ren->uniforms[slot].number = 0;
}


FH_XMOD void fh_batch_flush(struct fh_batch *ren)
{
	u32 i;

	if(ren->vertex_number == 0) {
		return;
	}

	/* Activate the relevant shader */
	fh_UseShader(ren->shader);

	/* Pass the uniform view matrix onto the shader */
	for(i = 0; i < ren->uniform_number; i++) {
		batch_write_uniform(&ren->uniforms[i]);
	}

	/* Bind the vertex buffer and write the data */
	glBindBuffer(GL_ARRAY_BUFFER, ren->gl_objects[1]);
	printf("A\n");
	glBufferSubData(GL_ARRAY_BUFFER,
			0, 
			ren->vertex_size * ren->vertex_number,
			ren->vertices
	);
	printf("B %d\n", U32_S * ren->index_number);

	/* Bind the index buffer and write data*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->gl_objects[2]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
			0, 
			U32_S * ren->index_number,
			ren->indices
	);
	printf("C\n");


	/* Finally render the data */
	glBindVertexArray(ren->gl_objects[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->gl_objects[2]);
	glDrawElements(GL_TRIANGLES, ren->index_number, GL_UNSIGNED_INT, 0);

	ren->vertex_number = 0;
	ren->index_number = 0;

	for(i = 0; i < ren->uniform_number; i++) {
		if(ren->uniforms[i].flags & FH_UNIFORM_F_CLEANUP) {
			fh_batch_reset_uniform(ren, i);
		}
	}
}
