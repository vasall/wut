#include "graphic/inc/shader.h"

#include "utility/inc/alarm.h"

#include "core/inc/core.h"

#include "system/inc/file.h"

#include <stdlib.h>


#define WT_SHADER_DEBUG	1


WT_INTERN s8 shd_new_shader(u32 type, const char *src, u32 *shd_out)
{
	u32 shd;
	char info_log[512];
	s32 success;

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
	if(!success) {
		glGetShaderInfoLog(shd, 512, NULL, info_log);
		WT_ALARM(WT_ERROR, info_log);
		WT_ALARM(WT_ERROR, "Failed to compile vertex shader");

		glDeleteShader(shd);
		return -1;
	}

	*shd_out = shd;
	return 0;
}

WT_INTERN s8 shd_extract_inputs(struct wt_shader_inputs *inp, const char *str)
{
	char *line = (char *)str;
	struct wt_shader_var *var;
	s32 num_vars = 0;
	char *start;
	char *end;
	s32 len;
	char loc_str[512];

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
			line++;
		}
		if (*line == '\n') {
			line++;
		}
	}

	inp->num = num_vars;

	return num_vars;
}

WT_INTERN s8 shd_extract_uniforms(struct wt_shader_uniforms *u, const char *str)
{
	char *runner = (char *)str;
	char swap[64];

	u32 counter = 0;
	u32 i = 0;

	struct wt_shader_uniform *uni;

	while((runner = strstr(runner, "binding=")) != NULL) {
		uni = &u->uniform[counter];

		runner += 8;

		i = 0;
		while((swap[i++] = *(runner++)) & 0x50);
		swap[i-1] = 0;

		uni->location = atoi(swap);

		runner = strstr(runner, "uniform ");
		runner += 8;

		i = 0;
		while((swap[i++] = *(runner++)) != 32);
		swap[i-1] = 0;

		strcpy(uni->name, swap);

		counter++;
	}

	u->num = counter;

	return counter;
}


WT_INTERN struct wt_shader *shd_create(char *name, const char *v_src,
		const char *f_src)
{
	struct wt_shader *shader;

	u32 vshader;
	u32 fshader;

	s32 success;
	char info_log[512];

	/*
	 * Allocate memory for shader.
	 */
	if(!(shader = wt_malloc(sizeof(struct wt_shader)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for new shader");
		goto err_return;
	}

	/*
	 * Extract the input variables from the vertex-shader.
	 */
	if(shd_extract_inputs(&shader->inputs, v_src) < 1) {
		WT_ALARM(WT_ERROR, "Failed to extract input");
		goto err_free_shader;
	}

	if(shd_extract_uniforms(&shader->uniforms, v_src) < 0) {
		WT_ALARM(WT_ERROR, "Failed to extract uniforms");
		goto err_free_shader;
	}

	/*
	 * Create and compile the vertex- and fragment-shader.
	 */
	if(shd_new_shader(GL_VERTEX_SHADER, v_src, &vshader) < 0)
		goto err_free_shader;

	if(shd_new_shader(GL_FRAGMENT_SHADER, f_src, &fshader) < 0)
		goto err_delete_vshader;


	/*
	 * Create shader program.
	 */
	if((shader->program = glCreateProgram()) == 0) {
		WT_ALARM(WT_ERROR, "Failed to create shader program");
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
		WT_ALARM(WT_ERROR, info_log);
		WT_ALARM(WT_ERROR, "Failed to link shaders to program");
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
	wt_free(shader);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new shader");
	return NULL;
}


WT_INTERN struct wt_shader *shd_load(char *name, char *v_pth, char *f_pth)
{
	char *v_buf = NULL;
	char *f_buf = NULL;
	struct wt_shader *shader;

	/*
	 * Read the vertex and fragment shader files.
	 */
	if(wt_fs_text(v_pth, NULL, &v_buf) < 0) {
		WT_ALARM(WT_ERROR, "Failed to load the vertex shader file");
		goto err_return;
	}

	if(wt_fs_text(f_pth, NULL, &f_buf) < 0) {
		WT_ALARM(WT_ERROR, "Failed to load the fragment shader file");
		goto err_free_v_buf;
	}

	/*
	 * Pass the source code for the shaders to the create function and
	 * create shader program.
	 */
	if(!(shader = shd_create(name, v_buf, f_buf)))
		goto err_free_f_buf;

	wt_free(v_buf);
	wt_free(f_buf);

	return shader;

err_free_f_buf:
	wt_free(v_buf);

err_free_v_buf:
	wt_free(f_buf);

err_return:
	WT_ALARM(WT_ERROR, "Failed to load new shader");
	return NULL;
}


WT_INTERN void shd_destroy(struct wt_shader *shd)
{
	glDeleteProgram(shd->program);

	wt_free(shd);
}


WT_INTERN void shd_rmv_fnc(u32 size, void *ptr)
{
	struct wt_shader *shader;

	wt_Ignore(size);

	if(!ptr)
		return;

	shader = (struct wt_shader *)ptr;
	shd_destroy(shader);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_API s8 wt_InitShaderTable(struct wt_context *ctx)
{
	struct wt_table *tbl;
	float vec[3];

	if(!ctx) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	wt_vec3_set(vec, 1, 2, 3);

	if(!(tbl = wt_tbl_create(&shd_rmv_fnc))) {
		WT_ALARM(WT_ERROR, "Failed to create wt_table");
		goto err_return;
	}

	/* Attach the shader table to the context */
	ctx->shaders = tbl;

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to initialize shader table");
	return -1;
}


WT_API void wt_CloseShaderTable(struct wt_context *ctx)
{
	if(!ctx) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_tbl_destroy(ctx->shaders);
	ctx->shaders = NULL;
}


WT_API struct wt_shader *wt_CreateShader(struct wt_context *ctx, char *name,
		const char *v_src, const char *f_src)
{
	struct wt_shader *shd;
	u32 size;
	void **p;

	if(!ctx || !name || !v_src || !f_src) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	printf("Fail 1\n");
	if(!(shd = shd_create(name, v_src, f_src)))
		goto err_return;

	printf("Fail 2\n");
	/* Set additional attributes */
	shd->context = ctx;

	/* Insert shader into table */
	size = sizeof(struct wt_shader);
	p = (void **)&shd;
	if(wt_ContextAdd(ctx, WT_CONTEXT_SHADERS, name, size, p) < 0)
		goto err_destroy_shader;

	printf("Fail 3\n");
	return shd;

err_destroy_shader:
	shd_destroy(shd);

err_return:
	printf("Failed to create shader: %s\n", name);
	WT_ALARM(WT_ERROR, "Failed to create new shader");
	return NULL;
}


WT_API struct wt_shader *wt_LoadShader(struct wt_context *ctx, char *name,
		char *v_pth, char *f_pth)
{
	struct wt_shader *shd;
	u32 size;
	void **p;

	if(!ctx || !name || !v_pth || !f_pth) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shd = shd_load(name, v_pth, f_pth)))
		goto err_return;

	/* Set additional attributes */
	shd->context = ctx;

	/* Insert shader into table */
	size = sizeof(struct wt_shader);
	p = (void **)&shd;
	if(wt_ContextAdd(ctx, WT_CONTEXT_SHADERS, name, size, p) < 0)
		goto err_destroy_shader;

	return shd;

err_destroy_shader:
	shd_destroy(shd);

err_return:
	printf("Failed to load shader: %s (%s, %s)\n", name, v_pth, f_pth);
	WT_ALARM(WT_ERROR, "Failed to load shader");
	return NULL;
}


WT_API void wt_RemoveShader(struct wt_shader *shd)
{
	if(!shd) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_ContextRemove(shd->context, WT_CONTEXT_SHADERS, shd->name);
}


WT_API struct wt_shader *wt_GetShader(struct wt_context *ctx, char *name)
{
	struct wt_shader *shader;

	if(!ctx || !name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(wt_tbl_get(ctx->shaders, name, NULL, (void **)&shader) != 1)
		return NULL;

	return shader;

err_return:
	WT_ALARM(WT_ERROR, "Failed to get shader from shader table");
	return NULL;
}


WT_API void wt_UseShader(struct wt_shader *shd)
{
	if(!shd) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	/* First activate the shader */
	glUseProgram(shd->program);
} 


WT_API void wt_UnuseShader(void)
{
	glUseProgram(0);
}


WT_API s8 wt_ShaderGetInputLoc(struct wt_shader *shd, char *var)
{
	s8 i;

	if(!shd || !var) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < shd->inputs.num; i++) {
		if(strcmp(shd->inputs.vars[i].name, var) == 0)
			return i;
	}

	return -1;
}


WT_API s8 wt_ShaderGetUniformLoc(struct wt_shader *shd, char *uni)
{
	u8 i;

	if(!shd || !uni) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < shd->uniforms.num; i++) {
		if(strcmp(shd->uniforms.uniform[i].name, uni) == 0)
			return i;
	}

	return -1;
}


#if 0

WT_API void wt_shd_show(struct wt_shader *shd)
{
	s32 i;
	struct wt_shader_var *var;

	if(!shd)
		return;


	printf("Shader attributes for %s:\n", shd->name);

	for(i = 0; i < shd->inputs.num; i++) {
		var = &shd->inputs.vars[i];
		printf("[%d]: %s %s\n", var->location, var->type, var->name);
	}
}
#endif
