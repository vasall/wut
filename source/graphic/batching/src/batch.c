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


static int batch_compile_source(unsigned int type, const char *src, unsigned int *shd_out)
{
	unsigned int shd;
	char info_log[512];
	int success;

	/*
	 * Create and compile the shader.
	 */
	shd = glCreateShader(type);
	glShaderSource(shd, 1, &src, NULL);
	glCompileShader(shd);

	/*
	 * Check if something went wrong.
	 */
	glGetShaderiv(shd, GL_COMPILE_STATUS, &success);
	if(success != GL_TRUE) {
		glGetShaderInfoLog(shd, 512, NULL, info_log);
		printf("Failed to compile shader source: %s\n", info_log);
		glDeleteShader(shd);
		return -1;
	}

	*shd_out = shd;
	return 0;
}


static int batch_load_shader(const char *v_src, const char *f_src, unsigned int *prog)
{
	unsigned int vshader;
	unsigned int fshader;

	int success;
	char info_log[512];

	/*
	 * Create and compile the vertex- and fragment-shader.
	 */
	printf("Compile vertex shader\n");
	if(batch_compile_source(GL_VERTEX_SHADER, v_src, &vshader) < 0)
		goto erfh_batch_return;

	printf("Compile fragment shader\n");
	if(batch_compile_source(GL_FRAGMENT_SHADER, f_src, &fshader) < 0)
		goto erfh_batch_delete_vshader;


	/*
	 * Create shader program.
	 */
	if((*prog = glCreateProgram()) == 0) {
		printf("Failed to create shader program\n");
		goto erfh_batch_delete_fshader;
	}


	/*
	 * Link shaders to program.
	 */
	glAttachShader(*prog, vshader);
	glAttachShader(*prog, fshader);

	/*
	 * Finally link the program.
	 */
	glLinkProgram(*prog);

	glGetProgramiv(*prog, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*prog, 512, NULL, info_log);
		printf("Failed to link shader\n");
		printf("Error: %s", info_log);
		goto erfh_batch_delete_program;
	}

	/*
	 * Finally detach and delete source shaders.
	 */
	glDetachShader(*prog, vshader);
	glDetachShader(*prog, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return 0;

erfh_batch_delete_program:
	glDeleteProgram(*prog);	

erfh_batch_delete_fshader:
	glDeleteShader(fshader);

erfh_batch_delete_vshader:
	glDeleteShader(vshader);	

erfh_batch_return:
	return -1;
}

static int calculate_vertex_size(int attribnum, struct fh_vertex_attrib *attribs)
{
	int i;
	int size = 0;

	for(i = 0; i < attribnum; i++) {
		size += (batch_sizeof_GLenum(attribs[i].type) * attribs[i].number);
	}

	return size;
}

extern struct fh_batch *fh_batch_create(int attribnum, struct fh_vertex_attrib *attribs, int vertex_capacity,
		int index_capacity, int uniformnum, struct fh_uniform_temp *uniforms)
{
	GLuint program;
	struct fh_batch *ren;

	char *vertex;
	char *fragment;

	int vsize;
	int i;

	int offset;

	ren = malloc(sizeof(struct fh_batch));

	/* calculate the size of a single vertex */
	vsize = calculate_vertex_size(attribnum, attribs);


	/* create the vertex array object */
	glGenVertexArrays(1, &ren->vao);
	glBindVertexArray(ren->vao);

	/* create the dynamic vertex buffer */
	glGenBuffers(1, &ren->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ren->vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize * vertex_capacity, NULL,GL_DYNAMIC_DRAW);

	/* create the dynamic index buffer */
	glGenBuffers(1, &ren->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ren->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_capacity, NULL, GL_STREAM_DRAW);


	/* enable and configure vertex data */
	offset = 0;
	for(i = 0; i < attribnum; i++) {

		printf("Enable %d with offset %d and %d elements\n", i, offset, attribs[i].number);

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
	ren->vertex_capacity = vertex_capacity;
	ren->vertices = malloc(vsize * vertex_capacity);


	/* Initialize and configure index data */
	ren->index_capacity = index_capacity;
	ren->index_count = 0;
	ren->indices = malloc(sizeof(unsigned int) * index_capacity);

	if (batch_load_shader(fh_ps_batch_vshader, fh_ps_batch_fshader, &program) < 0) {
		exit(1);
	}

	ren->shader = program;

	if(uniformnum>0) {
		/* configure uniform data */
		for(i = 0; i < uniformnum; i++) {
			ren->uniforms[i].slot = glGetUniformLocation(program, uniforms[i].name);
			ren->uniforms[i].type = uniforms[i].type;
			ren->uniforms[i].size = batch_sizeof_UniformType(uniforms[i].type);
			ren->uniforms[i].number = 0;
			ren->uniforms[i].limit = uniforms[i].limit;
			ren->uniforms[i].data = malloc(ren->uniforms[i].size * uniforms[i].limit);
			ren->uniforms[i].flags = uniforms[i].flags;
		}
	}
	ren->uniform_count = uniformnum;

	return ren;
}

extern int fh_batch_push_vertex(struct fh_batch *renderer, void *ptr)
{
	int offset = renderer->vertex_size * renderer->vertex_count;

	if(renderer->vertex_count == renderer->vertex_capacity) {
		return -1;
	}

	memcpy(renderer->vertices + offset, ptr, renderer->vertex_size);
	renderer->vertex_count++;

	return (renderer->vertex_count-1);
}

extern int fh_batch_push_index(struct fh_batch *renderer, unsigned int idx)
{
	if(renderer->index_count >= renderer->index_capacity) {
		return -1;
	}

	renderer->indices[renderer->index_count] = idx;
	renderer->index_count++;

	return (renderer->index_count - 1);
}

extern int fh_batch_push_uniform(struct fh_batch *renderer, int index, void *ptr)
{
	int offset = renderer->uniforms[index].size * renderer->uniforms[index].number;

	memcpy(renderer->uniforms[index].data + offset, ptr, renderer->uniforms[index].size);
	renderer->uniforms[index].number++;

	return (renderer->uniforms[index].number-1);
}

extern void fh_batch_reset_uniform(struct fh_batch *renderer, int index)
{
	renderer->uniforms[index].number = 0;
}


extern void fh_batch_flush(struct fh_batch *renderer)
{
	int i;

	if (renderer->vertex_count == 0) {
		return;
	}

	/* Activate the relevant shader */
	glUseProgram(renderer->shader);

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
