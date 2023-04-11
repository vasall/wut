#include "shader.h"

#include "alarm.h"
#include "system.h"
#include "core.h"
#include "file.h"
#include "system.h"

#include <stdlib.h>



FH_API s8 fh_shd_init(void)
{
	struct fh_table *tbl;
	float vec[3];

	vec3_set(vec, 1, 2, 3);

	printf("Create shader table\n");

	if(!(tbl = fh_tbl_create(&fh_shd_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create fh_table");
		goto err_return;
	}

	/* Attach shader table to global core */
	g_fh_core.shaders = tbl;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize global shader table");
	return -1;
}


FH_API void fh_shd_close(void)
{
	fh_tbl_destroy(g_fh_core.shaders);
	g_fh_core.shaders = NULL;
}


FH_API s8 fh_shd_insert(struct fh_shader *shader)
{
	u32 size;
	void **p;

	if(!shader) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct fh_shader);
	p = (void **)&shader;

	if(fh_tbl_add(g_fh_core.shaders, shader->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to insert entry into fh_table");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert new shader into shader list");
	return -1;
}


FH_API void fh_shd_remove(char *name)
{
	if(!name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_tbl_rmv(g_fh_core.shaders, name);
}

FH_INTERN s8 fh_shd_new_shader(u32 type, char *src, u32 *shd_out)
{
	u32 shd;
	char info_log[512];
	s32 success;

	if(!src) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Create and compile the shader.
	 */
	shd = glCreateShader(type);
	glShaderSource(shd, 1, (const char **)&src, NULL);
	glCompileShader(shd);

	glGetShaderiv(shd, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shd, 512, NULL, info_log);
		ALARM(ALARM_ERR, info_log);
		ALARM(ALARM_ERR, "Failed to compile vertex shader");
		goto err_delete_shd;
	}

	printf("Shader %d\n", shd);

	*shd_out = shd;
	return 0;

err_delete_shd:
	glDeleteShader(shd);

err_return:
	ALARM(ALARM_ERR, "Failed to create shader");
	return -1;

}

FH_INTERN s8 fh_shd_extract_inputs(struct fh_shader_inputs *inp, char *str)
{
	char *line = str;
	struct fh_shader_var *var;
	s32 num_vars = 0;
	char *start;
	char *end;
	s32 len;
	char loc_str[512];

	if(!inp || !str) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	while (*line != '\0') {
		if (strstr(line, "in ")) {
			var = &inp->vars[num_vars];

			/* Extract variable type */
			start = strstr(line, "in ") + 3;
			end = strstr(start, " ");
			len = end - start;
			strncpy(var->type, start, len);
			var->type[len] = '\0';

			/* Extract variable name */
			start = end + 1;
			end = strstr(start, ";");
			len = end - start;
			strncpy(var->name, start, len);
			var->name[len] = '\0';

			/* Extract variable location */
			start = strstr(line, "location=") + 9;
			end = strstr(start, ")");
			len = end - start;
			strncpy(loc_str, start, len);
			loc_str[len] = '\0';
			var->location = atoi(loc_str);

			line = end;
			num_vars++;
		}

		/* Advance to next line */
		while(*line != '\n' && *line != '\0') {
			printf("%c", *line);
			line++;
		}
		printf("\n");
		if (*line == '\n') {
			line++;
		}
	}

	inp->num = num_vars;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to extract shader inputs");
	return -1;
}

FH_API struct fh_shader *fh_shd_create(char *name, char *v_src, char *f_src)
{
	struct fh_shader *shader;

	u32 vshader;
	u32 fshader;

	s32 success;
	char info_log[512];


	if(!name || !v_src || !f_src) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Allocate memory for shader.
	 */
	if(!(shader = fh_malloc(sizeof(struct fh_shader)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for new shader");
		goto err_return;
	}

	/*
	 * Extract the input variables from the vertex-shader.
	 */
	if(fh_shd_extract_inputs(&shader->inputs, v_src) < 0)
		goto err_free_shader;


	/*
	 * Create and compile the vertex- and fragment-shader.
	 */
	if(fh_shd_new_shader(GL_VERTEX_SHADER, v_src, &vshader) < 0)
		goto err_free_shader;

	if(fh_shd_new_shader(GL_FRAGMENT_SHADER, f_src, &fshader) < 0)
		goto err_delete_vshader;

	printf("B\n");


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

err_delete_vshader:
	glDeleteShader(vshader);	

err_free_shader:
	fh_free(shader);

err_return:
	ALARM(ALARM_ERR, "Failed to create new shader");
	return NULL;
}


FH_API struct fh_shader *fh_shd_load(char *name, char *v_pth, char *f_pth)
{
	char *v_buf;
	char *f_buf;
	struct fh_shader *shader;

	if(!name || !v_pth || !f_pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/*
	 * Read the vertex and fragment shader files.
	 */
	if(fh_fs_text(v_pth, NULL, &v_buf) < 0) {
		ALARM(ALARM_ERR, "Failed to load the vertex shader file");
		goto err_return;
	}

	if(fh_fs_text(f_pth, NULL, &f_buf) < 0) {
		ALARM(ALARM_ERR, "Failed to load the fragment shader file");
		goto err_free_v_buf;
	}

	/*
	 * Pass the source code for the shaders to the create function and
	 * create shader program.
	 */
	if(!(shader = fh_shd_create(name, v_buf, f_buf)))
		goto err_free_f_buf;

	fh_free(v_buf);
	fh_free(f_buf);

	return shader;

err_free_f_buf:
	fh_free(v_buf);

err_free_v_buf:
	fh_free(f_buf);

err_return:
	ALARM(ALARM_ERR, "Failed to load new shader");
	return NULL;
}


FH_API void fh_shd_destroy(struct fh_shader *shader)
{
	if(!shader)
		return;

	glDeleteProgram(shader->program);

	fh_free(shader);
}


FH_API struct fh_shader *fh_shd_get(char *name)
{
	struct fh_shader *shader;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(g_fh_core.shaders, name, NULL, (void **)&shader) != 1) {
		ALARM(ALARM_ERR, "Shader could not be found in fh_table");
		goto err_return;
	}

	return shader;

err_return:
	ALARM(ALARM_ERR, "Failed to get shader from shader table");
	return NULL;
}


FH_API void fh_shd_use(struct fh_shader *shd)
{
	u8 i;

	if(!shd) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* First activate the shader */
	glUseProgram(shd->program);

	/* ...then enable the attributes */
	for(i = 0; i < shd->inputs.num; i++)
		glEnableVertexAttribArray(shd->inputs.vars[i].location);
} 


FH_API void fh_shd_unuse(void)
{
	glUseProgram(0);
}


FH_API s8 fh_shd_loc(struct fh_shader *shd, char *var)
{
	s8 i;

	if(!shd || !var) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < shd->inputs.num; i++) {
		if(strcmp(shd->inputs.vars[i].name, var) == 0)
			return i;
	}

	return -1;
}


FH_API void fh_shd_show_attrib(struct fh_shader *shd)
{
	s32 i;
	struct fh_shader_var *var;

	if(!shd)
		return;


	printf("Shader attributes for %s:\n", shd->name);

	for(i = 0; i < shd->inputs.num; i++) {
		var = &shd->inputs.vars[i];
		printf("[%d]: %s %s\n", var->location, var->type, var->name);
	}
}


FH_API void fh_shd_rmv_fnc(u32 size, void *ptr)
{
	struct fh_shader *shader;

	/* SILENCIO! */
	if(size) {}

	if(!ptr)
		return;

	shader = (struct fh_shader *)ptr;
	fh_shd_destroy(shader);
}
