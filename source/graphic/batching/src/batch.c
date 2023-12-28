#include "graphic/batching/inc/batch.h"

#include "system/inc/system.h"

#include "window/subsystems/inc/opengl.h"

#include "core/inc/predefined.h"


FH_INTERN size_t batch_sizeof_GLenum(GLenum type)
{
	switch(type) {
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
			glUniform1f(uniform->slot, 
					((GLfloat *)uniform->data)[0]);
			return;
		case FH_UNIFORM_2F:
			glUniform2f(uniform->slot, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1]);
			return;
		case FH_UNIFORM_3F:
			glUniform3f(uniform->slot, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1],
					((GLfloat *)uniform->data)[2]);
			return;
		case FH_UNIFORM_4F:
			glUniform4f(uniform->slot, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1],
					((GLfloat *)uniform->data)[2],
					((GLfloat *)uniform->data)[3]);
			return;

		case FH_UNIFORM_1I:
			glUniform1i(uniform->slot, 
					((GLint *)uniform->data)[0]);
			return;
		case FH_UNIFORM_2I:
			glUniform2i(uniform->slot, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1]);
			return;
		case FH_UNIFORM_3I:
			glUniform3i(uniform->slot, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1],
					((GLint *)uniform->data)[2]);
			return;
		case FH_UNIFORM_4I:
			glUniform4i(uniform->slot, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1],
					((GLint *)uniform->data)[2],
					((GLint *)uniform->data)[3]);
			return;

		case FH_UNIFORM_1UI:
			glUniform1ui(uniform->slot, 
					((GLuint *)uniform->data)[0]);
			return;
		case FH_UNIFORM_2UI:
			glUniform2ui(uniform->slot, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1]);
			return;
		case FH_UNIFORM_3UI:
			glUniform3ui(uniform->slot, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1],
					((GLuint *)uniform->data)[2]);
			return;
		case FH_UNIFORM_4UI:
			glUniform4ui(uniform->slot, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1],
					((GLuint *)uniform->data)[2],
					((GLuint *)uniform->data)[3]);
			return;

		case FH_UNIFORM_1FV:
			glUniform1fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_2FV:
			glUniform2fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_3FV:
			glUniform3fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_4FV:
			glUniform4fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;

		case FH_UNIFORM_1IV:
			glUniform1iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;
		case FH_UNIFORM_2IV:
			glUniform2iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;
		case FH_UNIFORM_3IV:
			glUniform3iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;
		case FH_UNIFORM_4IV:
			glUniform4iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;

		case FH_UNIFORM_1UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;
		case FH_UNIFORM_2UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;
		case FH_UNIFORM_3UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;
		case FH_UNIFORM_4UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;

		case FH_UNIFORM_M2FV:
			glUniformMatrix2fv(uniform->slot, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_M3FV:
			glUniformMatrix3fv(uniform->slot, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
		case FH_UNIFORM_M4FV:
			glUniformMatrix4fv(uniform->slot, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
	}
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_batch *fh_batch_create(struct fh_shader *shd, int attribnum,
		struct fh_vertex_attrib *attribs, int vtx_cap, int idx_cap, 
		int uninum, struct fh_uniform_temp *unis)
{
	struct fh_batch *ren;

	s32 vsize;
	s32 i;

	int offset;

	ren = malloc(sizeof(struct fh_batch));

	/* calculate the size of a single vertex */
	vsize = 0;
	for(i = 0; i < attribnum; i++) {
		vsize += (batch_sizeof_GLenum(attribs[i].type) * attribs[i].number);
	}


	/* create the vertex array object */
	glGenVertexArrays(1, &ren->vao);
	glBindVertexArray(ren->vao);

	/* create the dynamic vertex buffer */
	glGenBuffers(1, &ren->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ren->vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize * vtx_cap,
			NULL, GL_DYNAMIC_DRAW);

	/* create the dynamic index buffer */
	glGenBuffers(1, &ren->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, U32_S * idx_cap,
			NULL, GL_STREAM_DRAW);


	/* enable and configure vertex data */
	offset = 0;
	for(i = 0; i < attribnum; i++) {
		glEnableVertexAttribArray(i);

		switch(attribs[i].type) {
			case GL_INT:
				glVertexAttribIPointer(
						i, 
						attribs[i].number,
						attribs[i].type,
						vsize,
						(void *)(offset)
						);
				break;


			default:
				glVertexAttribPointer(
						i, 
						attribs[i].number,
						attribs[i].type,
						GL_FALSE,
						vsize,
						(void *)(offset)
						);
		}

		offset += (attribs[i].number * batch_sizeof_GLenum(attribs[i].type));
	}

	ren->vertex_size = vsize;
	ren->vertex_count = 0;
	ren->vertex_capacity = vtx_cap;
	ren->vertices = malloc(vsize * vtx_cap);


	/* Initialize and configure index data */
	ren->index_capacity = idx_cap;
	ren->index_count = 0;
	ren->indices = malloc(U32_S * idx_cap);

	ren->shader = shd;

	if(uninum > 0) {
		/* configure uniform data */
		for(i = 0; i < uninum; i++) {
			ren->uniforms[i].slot = glGetUniformLocation(shd->program, unis[i].name);
			ren->uniforms[i].type = unis[i].type;
			ren->uniforms[i].size = batch_sizeof_UniformType(unis[i].type);
			ren->uniforms[i].number = 0;
			ren->uniforms[i].limit = unis[i].limit;
			ren->uniforms[i].data = malloc(ren->uniforms[i].size * unis[i].limit);
			ren->uniforms[i].flags = unis[i].flags;
		}
	}
	ren->uniform_count = uninum;

	return ren;
}

FH_API int fh_batch_push_vertex(struct fh_batch *renderer, void *ptr)
{
	int offset = renderer->vertex_size * renderer->vertex_count;

	if(renderer->vertex_count == renderer->vertex_capacity) {
		return -1;
	}

	memcpy(renderer->vertices + offset, ptr, renderer->vertex_size);
	renderer->vertex_count++;

	return (renderer->vertex_count-1);
}

FH_API int fh_batch_push_index(struct fh_batch *renderer, unsigned int idx)
{
	if(renderer->index_count >= renderer->index_capacity) {
		return -1;
	}

	renderer->indices[renderer->index_count] = idx;
	renderer->index_count++;

	return (renderer->index_count - 1);
}

FH_API int fh_batch_push_uniform(struct fh_batch *renderer, int index, void *ptr)
{
	int offset = renderer->uniforms[index].size * renderer->uniforms[index].number;

	memcpy(renderer->uniforms[index].data + offset, ptr, renderer->uniforms[index].size);
	renderer->uniforms[index].number++;

	return (renderer->uniforms[index].number-1);
}

FH_API void fh_batch_reset_uniform(struct fh_batch *renderer, int index)
{
	renderer->uniforms[index].number = 0;
}


FH_API void fh_batch_flush(struct fh_batch *renderer)
{
	int i;

	if (renderer->vertex_count == 0) {
		return;
	}

	/* Activate the relevant shader */
	fh_UseShader(renderer->shader);

	/* Pass the uniform view matrix onto the shader */
	for(i = 0; i < renderer->uniform_count; i++) {
		batch_write_uniform(&renderer->uniforms[i]);
	}

	/* Bind the vertex buffer and write the data */
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vertex_size * renderer->vertex_count,
			renderer->vertices);

	/* Bind the index buffer and write data*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * renderer->index_count,
			renderer->indices);


	/* Finally render the data */
	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
	glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);

	renderer->vertex_count = 0;
	renderer->index_count = 0;

	for(i = 0; i < renderer->uniform_count; i++) {
		if(renderer->uniforms[i].flags & FH_UNIFORM_F_CLEANUP) {
			fh_batch_reset_uniform(renderer, i);
		}
	}
}
