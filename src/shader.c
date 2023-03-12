#include "shader.h"

#include <stdlib.h>



FH_API struct fh_shader *fh_shader_create(const char *vshader_src,
		const char *fshader_src)
{
	struct fh_shader *shader;

	u32 vshader;
	u32 fshader;

	s32 success;
	char info_log[512];


	if(!vshader_src || !fshader_src) {
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
	 * Create and load the vertex shader.
	 */
	if((vshader = glCreateShader(GL_VERTEX_SHADER)) == 0) {
		ALARM(ALARM_ERR, "Failed to create vertex shader");
		goto err_free_shader;
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
	 * Create and load the fragment shader.
	 */
	if((fshader = glCreateShader(GL_FRAGMENT_SHADER)) == 0) {
		ALARM(ALARM_ERR, "Failed to create fragment shader");
		goto err_delete_vshader;
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

	return shader;

err_delete_program:
	glDeleteProgram(shader->program);	

err_delete_fshader:
	glDeleteShader(fshader);

err_delete_vshader:
	glDeleteShader(vshader);	

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
