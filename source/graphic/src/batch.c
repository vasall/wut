#include "graphic/inc/batch.h"

#include "system/inc/system.h"

#include "utility/inc/alarm.h"

#include "window/inc/opengl.h"

#include "core/inc/predefined.h"


WUT_INTERN size_t bat_sizeof_GLenum(GLenum type)
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


WUT_INTERN size_t bat_sizeof_UniformType(enum wut_eUniformType type)
{
	switch(type) {
		case WUT_UNI_1F: return sizeof(GLfloat);
		case WUT_UNI_2F: return 2*sizeof(GLfloat);
		case WUT_UNI_3F: return 3*sizeof(GLfloat);
		case WUT_UNI_4F: return 4*sizeof(GLfloat);

		case WUT_UNI_1I: return sizeof(GLint);
		case WUT_UNI_2I: return 2*sizeof(GLint);
		case WUT_UNI_3I: return 3*sizeof(GLint);
		case WUT_UNI_4I: return 4*sizeof(GLint);

		case WUT_UNI_1UI: return sizeof(GLuint);
		case WUT_UNI_2UI: return 2*sizeof(GLuint);
		case WUT_UNI_3UI: return 3*sizeof(GLuint);
		case WUT_UNI_4UI: return 4*sizeof(GLuint);

		case WUT_UNI_1FV: return sizeof(GLfloat);
		case WUT_UNI_2FV: return 2*sizeof(GLfloat);
		case WUT_UNI_3FV: return 3*sizeof(GLfloat);
		case WUT_UNI_4FV: return 4*sizeof(GLfloat);

		case WUT_UNI_1IV: return sizeof(GLint);
		case WUT_UNI_2IV: return 2*sizeof(GLint);
		case WUT_UNI_3IV: return 3*sizeof(GLint);
		case WUT_UNI_4IV: return 4*sizeof(GLint);

		case WUT_UNI_1UIV: return sizeof(GLuint);
		case WUT_UNI_2UIV: return 2*sizeof(GLuint);
		case WUT_UNI_3UIV: return 3*sizeof(GLuint);
		case WUT_UNI_4UIV: return 4*sizeof(GLuint);

		case WUT_UNI_M2FV: return 4*sizeof(GLfloat);
		case WUT_UNI_M3FV: return 9*sizeof(GLfloat);
		case WUT_UNI_M4FV: return 16*sizeof(GLfloat);
	}

	return 0;
}

WUT_INTERN void bat_write_uniform(struct wut_Uniform *uniform)
{
	u32 count;

	if(uniform->flags & WUT_UNI_F_ALL) {
		count = uniform->limit;
	}
	else {
		count = uniform->number;
	}


	switch(uniform->type) {
		case WUT_UNI_1F:
			glUniform1f(uniform->slot, 
					((GLfloat *)uniform->data)[0]);
			return;
		case WUT_UNI_2F:
			glUniform2f(uniform->slot, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1]);
			return;
		case WUT_UNI_3F:
			glUniform3f(uniform->slot, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1],
					((GLfloat *)uniform->data)[2]);
			return;
		case WUT_UNI_4F:
			glUniform4f(uniform->slot, 
					((GLfloat *)uniform->data)[0],
					((GLfloat *)uniform->data)[1],
					((GLfloat *)uniform->data)[2],
					((GLfloat *)uniform->data)[3]);
			return;

		case WUT_UNI_1I:
			glUniform1i(uniform->slot, 
					((GLint *)uniform->data)[0]);
			return;
		case WUT_UNI_2I:
			glUniform2i(uniform->slot, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1]);
			return;
		case WUT_UNI_3I:
			glUniform3i(uniform->slot, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1],
					((GLint *)uniform->data)[2]);
			return;
		case WUT_UNI_4I:
			glUniform4i(uniform->slot, 
					((GLint *)uniform->data)[0],
					((GLint *)uniform->data)[1],
					((GLint *)uniform->data)[2],
					((GLint *)uniform->data)[3]);
			return;

		case WUT_UNI_1UI:
			glUniform1ui(uniform->slot, 
					((GLuint *)uniform->data)[0]);
			return;
		case WUT_UNI_2UI:
			glUniform2ui(uniform->slot, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1]);
			return;
		case WUT_UNI_3UI:
			glUniform3ui(uniform->slot, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1],
					((GLuint *)uniform->data)[2]);
			return;
		case WUT_UNI_4UI:
			glUniform4ui(uniform->slot, 
					((GLuint *)uniform->data)[0],
					((GLuint *)uniform->data)[1],
					((GLuint *)uniform->data)[2],
					((GLuint *)uniform->data)[3]);
			return;

		case WUT_UNI_1FV:
			glUniform1fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;
		case WUT_UNI_2FV:
			glUniform2fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;
		case WUT_UNI_3FV:
			glUniform3fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;
		case WUT_UNI_4FV:
			glUniform4fv(uniform->slot,
					count, (GLfloat *)uniform->data);
			return;

		case WUT_UNI_1IV:
			glUniform1iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;
		case WUT_UNI_2IV:
			glUniform2iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;
		case WUT_UNI_3IV:
			glUniform3iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;
		case WUT_UNI_4IV:
			glUniform4iv(uniform->slot,
					count, (GLint *)uniform->data);
			return;

		case WUT_UNI_1UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;
		case WUT_UNI_2UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;
		case WUT_UNI_3UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;
		case WUT_UNI_4UIV:
			glUniform1uiv(uniform->slot,
					count, (GLuint *)uniform->data);
			return;

		case WUT_UNI_M2FV:
			glUniformMatrix2fv(uniform->slot, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
		case WUT_UNI_M3FV:
			glUniformMatrix3fv(uniform->slot, count,
					GL_FALSE, (GLfloat *)uniform->data);
			return;
		case WUT_UNI_M4FV:
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

WUT_API struct wut_Batch *wut_bat_create(struct wut_Shader *shd,
		struct wut_Texture *tex, s32 attribnum,
		struct wut_VertexAttrib *attribs, s32 vtx_cap,
		s32 idx_cap, s32 uninum, struct wut_UniformTemp *unis,
		wut_BatchFunc pre, void *pre_data)
{
	struct wut_Batch *ren;

	s32 vsize;
	s32 i;

	s32 offset;

	ren = malloc(sizeof(struct wut_Batch));

	/* calculate the size of a single vertex */
	vsize = 0;
	for(i = 0; i < attribnum; i++) {
		vsize += (bat_sizeof_GLenum(attribs[i].type) * attribs[i].number);
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

		offset += (attribs[i].number * bat_sizeof_GLenum(attribs[i].type));
	}

	ren->vertex_size = vsize;
	ren->vertex_count = 0;
	ren->vertex_capacity = vtx_cap;
	if(!(ren->vertices = wut_malloc(vsize * vtx_cap))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for vertices");
		goto err_delete_buffers;
	}


	/* Initialize and configure index data */
	ren->index_capacity = idx_cap;
	ren->index_count = 0;
	if(!(ren->indices = wut_malloc(U32_S * idx_cap))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for indices");
		goto err_free_vertices;
	}

	/* Set the shader and texture for this batch ren */
	ren->shader = shd;
	ren->texture = tex;

	if(uninum > 0) {
		if(!(ren->uniforms = wut_malloc(uninum * sizeof(struct wut_Uniform)))) {
			WUT_ALARM(WUT_ERROR, "Failed to allocate mem for uniforms");
			goto err_free_indices;
		}

		/* configure uniform data */
		for(i = 0; i < uninum; i++) {
			ren->uniforms[i].slot = glGetUniformLocation(shd->program, unis[i].name);
			ren->uniforms[i].type = unis[i].type;
			ren->uniforms[i].size = bat_sizeof_UniformType(unis[i].type);
			ren->uniforms[i].number = 0;
			ren->uniforms[i].limit = unis[i].limit;
			if(!(ren->uniforms[i].data = wut_malloc(ren->uniforms[i].size * unis[i].limit))) {
				goto err_free_uniforms;
			}
			ren->uniforms[i].flags = unis[i].flags;
		}
	}
	ren->uniform_count = uninum;

	/* Set callback functions */
	ren->pre_fnc = pre;
	ren->pre_fnc_data = pre_data;

	return ren;

err_free_uniforms:
	for(i = i - 1; i >= 0; i--) {
		wut_free(ren->uniforms[i].data);
	}
	wut_free(ren->uniforms);

err_free_indices:
	wut_free(ren->indices);

err_free_vertices:
	wut_free(ren->vertices);

err_delete_buffers:
	glDeleteBuffers(1, &ren->ibo);
	glDeleteBuffers(1, &ren->vbo);
	glDeleteVertexArrays(1, &ren->vao);

	return NULL;
}


WUT_API void wut_bat_destroy(struct wut_Batch *ren)
{
	s32 i;

	if(!ren) return;

	if(ren->uniform_count > 0) {
		for(i = ren->uniform_count - 1; i >= 0; i--) {
			wut_free(ren->uniforms[i].data);
		}
		wut_free(ren->uniforms);
	}

	wut_free(ren->indices);
	wut_free(ren->vertices);

	glDeleteBuffers(1, &ren->ibo);
	glDeleteBuffers(1, &ren->vbo);
	glDeleteVertexArrays(1, &ren->vao);
}


WUT_API s32 wut_bat_push_vertex(struct wut_Batch *ren, void *ptr)
{
	s32 offset = ren->vertex_size * ren->vertex_count;

	if(ren->vertex_count == ren->vertex_capacity) {
		return -1;
	}

	memcpy(ren->vertices + offset, ptr, ren->vertex_size);
	ren->vertex_count++;

	return (ren->vertex_count-1);
}

WUT_API s32 wut_bat_push_index(struct wut_Batch *ren, u32 idx)
{
	if(ren->index_count >= ren->index_capacity) {
		return -1;
	}

	ren->indices[ren->index_count] = idx;
	ren->index_count++;

	return (ren->index_count - 1);
}

WUT_API s32 wut_bat_push_uniform(struct wut_Batch *ren, s32 index, void *ptr)
{
	s32 offset = ren->uniforms[index].size * ren->uniforms[index].number;

	memcpy(ren->uniforms[index].data + offset, ptr, ren->uniforms[index].size);
	ren->uniforms[index].number++;

	return (ren->uniforms[index].number - 1);
}

WUT_API void wut_bat_reset_uniform(struct wut_Batch *ren, s32 index)
{
	ren->uniforms[index].number = 0;
}


WUT_API void wut_bat_flush(struct wut_Batch *ren)
{
	s32 i;

	if (ren->vertex_count == 0) {
		return;
	}

	if(ren->pre_fnc) {
		ren->pre_fnc(ren, ren->pre_fnc_data);
	}

	/* Activate the relevant shader */
	wut_UseShader(ren->shader);

	/* Activate the relevant shader */
	wut_UseTexture(ren->texture);

	/* Pass the uniform view matrix onto the shader */
	for(i = 0; i < ren->uniform_count; i++) {
		bat_write_uniform(&ren->uniforms[i]);
	}

	/* Bind the vertex buffer and write the data */
	glBindBuffer(GL_ARRAY_BUFFER, ren->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ren->vertex_size * ren->vertex_count,
			ren->vertices);

	/* Bind the index buffer and write data*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(u32) * ren->index_count,
			ren->indices);


	/* Finally render the data */
	glBindVertexArray(ren->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->ibo);
	glDrawElements(GL_TRIANGLES, ren->index_count, GL_UNSIGNED_INT, 0);

	ren->vertex_count = 0;
	ren->index_count = 0;

	for(i = 0; i < ren->uniform_count; i++) {
		if(ren->uniforms[i].flags & WUT_UNI_F_CLEANUP) {
			wut_bat_reset_uniform(ren, i);
		}
	}
}
