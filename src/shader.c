#include "shader.h"

#include "core.h"
#include "filesystem.h"

#include <stdlib.h>


FH_API s8 fh_shader_init(void)
{
	struct fh_shader_list *lst;
	s32 i;

	/* Allocate memory for the shader list */
	if(!(lst = smalloc(sizeof(struct fh_shader_list)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for shader list");
		goto err_return;
	}

	lst->num = 0;

	for(i = 0; i < FH_SHADER_LIM; i++)
		lst->shaders[i] = NULL;

	/* Attach shader list to global core */
	g_fh_core.shaders = lst;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize global shader list");
	return -1;
}

FH_API void fh_shader_close(void)
{
	s32 i;
	struct fh_shader_list *lst;

	lst = g_fh_core.shaders;

	if(!lst)
		return;

	/* Go through the list and destroy all existing shaders */
	for(i = 0; i < FH_SHADER_LIM; i++) {
		if(!lst->shaders[i])
			continue;

		fh_shader_destroy(lst->shaders[i]);
		lst->shaders[i] = NULL;
	}

	/* Finally free the allocated memory */
	sfree(lst);
}


FH_API s8 fh_shader_insert(struct fh_shader *shader)
{
	struct fh_shader_list *lst;
	u16 i;

	if(!shader) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	lst = g_fh_core.shaders;

	if(lst->num + 1 > FH_SHADER_LIM) {
		ALARM(ALARM_ERR, "The shader list is full");
		goto err_return;
	}

	for(i = 0; i < FH_SHADER_LIM; i++) {
		if(!lst->shaders[i]) {
			lst->shaders[i] = shader;
			lst->num++;
			return 0;
		}
	}

err_return:
	ALARM(ALARM_ERR, "Failed to insert new shader into shader list");
	return -1;
}


FH_API void fh_shader_remove(char *name)
{
	u16 slot;
	struct fh_shader_list *lst;
	struct fh_shader *ref;


	if(!name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	lst = g_fh_core.shaders;

	if(fh_shader_get(name, &slot)) {
		ref = fh_core_get_ui_shader();
		if(ref != NULL && strcmp(name, ref->name) == 0) {
			fh_core_set_ui_shader(NULL);
		}

		fh_shader_destroy(lst->shaders[slot]);
		lst->shaders[slot] = NULL;
		lst->num--;
	}


}

FH_API struct fh_shader *fh_shader_create(char *name, const char *vshader_src,
		const char *fshader_src, s8 var_num, char *vars)
{
	struct fh_shader *shader;

	u32 t;

	u32 vshader;
	u32 fshader;

	s32 success;
	char info_log[512];


	char *vshader_buf;
	char *fshader_buf;

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

	/*
	 * Allocate memory for variables.
	 */
	shader->var_num = 0;
	shader->vars = NULL;
	if(var_num > 0 && vars != NULL) {
		shader->var_num = var_num;

		t = sizeof(struct fh_shader_variable) * var_num;
		if(!(shaders->vars = smalloc(t))) {
			ALARM(ALARM_ERR, "Failed to allocate memory for vars");
			goto err_free_shader;
		}
	}

	/*
	 * Read the vertex shader file.
	 */
	if(fh_fs_load(vshader_src, &vshader_buf) < 0) {
		ALARM(ALARM_ERR, "Failed to load the vertex shader file");
		goto err_free_shader_vars;
	}

	/*
	 * Create and load the vertex shader.
	 */
	if((vshader = glCreateShader(GL_VERTEX_SHADER)) < 0) {
		ALARM(ALARM_ERR, "Failed to create vertex shader");
		goto err_free_vshader_buf;
	}
	glShaderSource(vshader, 1, &vshader_src, NULL);
	glCompileShader(vshader);

	glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vshader, 512, NULL, info_log);
		ALARM(ALARM_ERR, info_log);
		ALARM(ALARM_ERR, "Failed to compile vertex shader");
		goto err_delete_vshader;
	}

	/*
	 * Read the fragment shader file.
	 */
	if(fh_fs_load(fshader_src, &fshader_buf) < 0) {
		ALARM(ALARM_ERR, "Failed to load the fragment shader file");
		goto err_delete_vshader;
	}

	/*
	 * Create and load the fragment shader.
	 */
	if((fshader = glCreateShader(GL_FRAGMENT_SHADER)) < 0) {
		ALARM(ALARM_ERR, "Failed to create fragment shader");
		goto err_free_fshader_buf;
	}
	glShaderSource(fshader, 1, &fshader_src, NULL);
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


FH_API struct fh_shader *fh_shader_get(char *name, u16 *slot)
{
	u16 i;
	struct fh_shader_list *lst;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	lst = g_fh_core.shaders;
	if(lst->num < 1) {
		ALARM(ALARM_ERR, "The shader list is empty");
		goto err_return;
	}

	for(i = 0; i < FH_SHADER_LIM; i++) {
		if(!lst->shaders[i])
			continue;

		if(strcmp(lst->shaders[i]->name, name) == 0) {
			if(slot)
				*slot = i;

			return lst->shaders[i];
		}
	}


err_return:
	ALARM(ALARM_ERR, "Failed to get shader from shader list");
	return NULL;
}
