#include "shader.h"

#include "core.h"
#include "filesystem.h"

#include <stdlib.h>



FH_API s8 fh_shader_init(void)
{
	struct dbs_table *tbl;
	float vec[3];

	vec3_set(vec, 1, 2, 3);

	printf("Create shader table\n");

	if(!(tbl = dbs_tbl_create(&fh_shader_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create dbs_table");
		goto err_return;
	}

	/* Attach shader table to global core */
	g_fh_core.shaders = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize global shader table");
	return -1;
}


FH_API void fh_shader_close(void)
{
	dbs_tbl_destroy(g_fh_core.shaders);
	g_fh_core.shaders = NULL;
}


FH_API s8 fh_shader_insert(struct fh_shader *shader)
{
	u32 size;
	void **p;

	if(!shader) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}




	size = sizeof(struct fh_shader);
	p = (void **)&shader;
	
	printf("Insert shader: %s (%p)\n", shader->name, p);

	if(dbs_tbl_add(g_fh_core.shaders, shader->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to insert entry into dbs_table");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert new shader into shader list");
	return -1;
}


FH_API void fh_shader_remove(char *name)
{
	if(!name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	dbs_tbl_rmv(g_fh_core.shaders, name);
}

FH_API struct fh_shader *fh_shader_create(char *name, const char *vshader_src,
		const char *fshader_src, s8 var_num, char **vars)
{
	struct fh_shader *shader;

	u16 tmp;
	u16 i;

	u32 vshader;
	u32 fshader;

	s32 success;
	char info_log[512];


	char *vshader_buf = NULL;
	char *fshader_buf = NULL;

	if(!name || !vshader_src || !fshader_src) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Allocate memory for shader.
	 */
	if(!(shader = smalloc(sizeof(struct fh_shader)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for new shader");
		goto err_return;
	}

	printf("Original shader %p\n", shader);

	/*
	 * Allocate memory for variables.
	 */
	shader->var_num = 0;
	shader->vars = NULL;
	
	if(var_num > 0 && vars != NULL) {
		shader->var_num = var_num;

		tmp = sizeof(struct fh_shader_variable) * var_num;
		if(!(shader->vars = smalloc(tmp))) {
			ALARM(ALARM_ERR, "Failed to allocate memory for vars");
			goto err_free_shader;
		}

		for(i = 0; i < shader->var_num; i++) {
			strcpy(shader->vars[i].name, vars[i]);
		}
	}

	/*
	 * Read the vertex and fragment shader files.
	 */
	if(fh_fs_load(vshader_src, &vshader_buf) < 0) {
		ALARM(ALARM_ERR, "Failed to load the vertex shader file");
		goto err_free_shader_vars;
	}
	if(fh_fs_load(fshader_src, &fshader_buf) < 0) {
		ALARM(ALARM_ERR, "Failed to load the fragment shader file");
		goto err_delete_vshader;
	}

	/*
	 * Create and load the vertex shader.
	 */
	vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, (const char **)&vshader_buf, NULL);
	glCompileShader(vshader);

	glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vshader, 512, NULL, info_log);
		ALARM(ALARM_ERR, info_log);
		ALARM(ALARM_ERR, "Failed to compile vertex shader");
		goto err_delete_vshader;
	}

	/*
	 * Create and load the fragment shader.
	 */
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, (const char **)&fshader_buf, NULL);
	glCompileShader(fshader);

	glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fshader, 512, NULL, info_log);
		ALARM(ALARM_ERR, info_log);
		ALARM(ALARM_ERR, "Failed to compile fragment shader");
		goto err_delete_fshader;
	}

	/* 
	 * Free the shader buffers.
	 */

	sfree(vshader_buf);
	sfree(fshader_buf);



	/*
	 * Create shader program.
	 */
	if((shader->program = glCreateProgram()) == 0) {
		ALARM(ALARM_ERR, "Failed to create shader program");
		goto err_delete_fshader;
	}


	/*
	 * Link shaders to program.
	 */
	glAttachShader(shader->program, vshader);
	glAttachShader(shader->program, fshader);
	
	/*
	 * Finally link the program.
	 */
	glLinkProgram(shader->program);

	glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader->program, 512, NULL, info_log);
		ALARM(ALARM_ERR, info_log);
		ALARM(ALARM_ERR, "Failed to link shaders to program");
		goto err_delete_program;
	}

	/*
	 * Finally detach and delete source shaders.
	 */
	glDetachShader(shader->program, vshader);
	glDetachShader(shader->program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);


	/* Now we can set the attributes */
	strcpy(shader->name, name);

	return shader;

err_delete_program:
	glDeleteProgram(shader->program);	

err_delete_fshader:
	glDeleteShader(fshader);

err_free_fshader_buf:
	sfree(fshader_buf);

err_delete_vshader:
	glDeleteShader(vshader);	

err_free_vshader_buf:
	sfree(vshader_buf);

err_free_shader_vars:
	sfree(shader->vars);

err_free_shader:
	sfree(shader);

err_return:
	ALARM(ALARM_ERR, "Failed to create new shader");
	return NULL;
}


FH_API void fh_shader_destroy(struct fh_shader *shader)
{
	if(!shader)
		return;

	glDeleteProgram(shader->program);

	sfree(shader);
}


FH_API struct fh_shader *fh_shader_get(char *name)
{
	struct fh_shader *shader;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(dbs_tbl_get(g_fh_core.shaders, name, NULL, (void **)&shader) != 1) {
		ALARM(ALARM_ERR, "Shader could not be found in dbs_table");
		goto err_return;
	}

	return shader;

err_return:
	ALARM(ALARM_ERR, "Failed to get shader from shader list");
	return NULL;
}


FH_API void fh_shader_use(struct fh_shader *shd)
{
	u8 i;

	if(!shd) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* First activate the shader */
	glUseProgram(shd->program);
	
	/* ...then enable the attributes */
	for(i = 0; i < shd->var_num; i++) {
		glEnableVertexAttribArray(i);
	}
} 


FH_API void fh_shader_unuse(void)
{
	glUseProgram(0);
}


FH_API void fh_shader_rmv_fnc(u32 size, void *ptr)
{
	struct fh_shader *shader;

	/* SILENCIO! */
	if(size) {}

	if(!ptr)
		return;

	shader = (struct fh_shader *)ptr;
	fh_shader_destroy(shader);
}
