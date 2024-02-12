#include "graphic/resources/inc/shader.h"

#include "utility/alarm/inc/alarm.h"

#include "core/inc/core.h"

#include "system/inc/file.h"

#include <stdlib.h>


#define FH_SHADER_DEBUG	1


FH_INTERN s8 shd_new_shader(u32 type, const char *src, u32 *shd_out)
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
		FH_ALARM(FH_ERROR, info_log);
		FH_ALARM(FH_ERROR, "Failed to compile vertex shader");

		glDeleteShader(shd);
		return -1;
	}

	*shd_out = shd;
	return 0;
}

FH_INTERN s8 shd_extract_inputs(struct fh_shader_inputs *inp, const char *str)
{
	char *line = (char *)str;
	struct fh_shader_var *var;
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

FH_INTERN s8 shd_extract_uniforms(struct fh_shader_uniforms *u, const char *str)
{
	char *runner = (char *)str;
	char swap[64];

	u32 counter = 0;
	u32 i = 0;

	struct fh_shader_uniform *uni;

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


FH_INTERN struct fh_shader *shd_create(char *name, const char *v_src,
		const char *f_src)
{
	struct fh_shader *shader;

	u32 vshader;
	u32 fshader;

	s32 success;
	char info_log[512];

	/*
	 * Allocate memory for shader.
	 */
	if(!(shader = fh_malloc(sizeof(struct fh_shader)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for new shader");
		goto err_return;
	}

	/*
	 * Extract the input variables from the vertex-shader.
	 */
	if(shd_extract_inputs(&shader->inputs, v_src) < 1) {
		FH_ALARM(FH_ERROR, "Failed to extract input");
		goto err_free_shader;
	}

	if(shd_extract_uniforms(&shader->uniforms, v_src) < 0) {
		FH_ALARM(FH_ERROR, "Failed to extract uniforms");
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
		FH_ALARM(FH_ERROR, "Failed to create shader program");
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
		FH_ALARM(FH_ERROR, info_log);
		FH_ALARM(FH_ERROR, "Failed to link shaders to program");
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
	FH_ALARM(FH_ERROR, "Failed to create new shader");
	return NULL;
}


FH_INTERN struct fh_shader *shd_load(char *name, char *v_pth, char *f_pth)
{
	char *v_buf = NULL;
	char *f_buf = NULL;
	struct fh_shader *shader;

	/*
	 * Read the vertex and fragment shader files.
	 */
	if(fh_fs_text(v_pth, NULL, &v_buf) < 0) {
		FH_ALARM(FH_ERROR, "Failed to load the vertex shader file");
		goto err_return;
	}

	if(fh_fs_text(f_pth, NULL, &f_buf) < 0) {
		FH_ALARM(FH_ERROR, "Failed to load the fragment shader file");
		goto err_free_v_buf;
	}

	/*
	 * Pass the source code for the shaders to the create function and
	 * create shader program.
	 */
	if(!(shader = shd_create(name, v_buf, f_buf)))
		goto err_free_f_buf;

	fh_free(v_buf);
	fh_free(f_buf);

	return shader;

err_free_f_buf:
	fh_free(v_buf);

err_free_v_buf:
	fh_free(f_buf);

err_return:
	FH_ALARM(FH_ERROR, "Failed to load new shader");
	return NULL;
}


FH_INTERN void shd_destroy(struct fh_shader *shd)
{
	glDeleteProgram(shd->program);

	fh_free(shd);
}


FH_INTERN void shd_rmv_fnc(u32 size, void *ptr)
{
	struct fh_shader *shader;

	fh_Ignore(size);

	if(!ptr)
		return;

	shader = (struct fh_shader *)ptr;
	shd_destroy(shader);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API s8 fh_InitShaderTable(struct fh_context *ctx)
{
	struct fh_table *tbl;
	float vec[3];

	if(!ctx) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	fh_vec3_set(vec, 1, 2, 3);

	if(!(tbl = fh_tbl_create(&shd_rmv_fnc))) {
		FH_ALARM(FH_ERROR, "Failed to create fh_table");
		goto err_return;
	}

	/* Attach the shader table to the context */
	ctx->shaders = tbl;

	return 0;

err_return:
	FH_ALARM(FH_ERROR, "Failed to initialize shader table");
	return -1;
}


FH_API void fh_CloseShaderTable(struct fh_context *ctx)
{
	if(!ctx) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_tbl_destroy(ctx->shaders);
	ctx->shaders = NULL;
}


FH_API struct fh_shader *fh_CreateShader(struct fh_context *ctx, char *name,
		const char *v_src, const char *f_src)
{
	struct fh_shader *shd;
	u32 size;
	void **p;

	if(!ctx || !name || !v_src || !f_src) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	printf("Fail 1\n");
	if(!(shd = shd_create(name, v_src, f_src)))
		goto err_return;

	printf("Fail 2\n");
	/* Set additional attributes */
	shd->context = ctx;

	/* Insert shader into table */
	size = sizeof(struct fh_shader);
	p = (void **)&shd;
	if(fh_ContextAdd(ctx, FH_CONTEXT_SHADERS, name, size, p) < 0)
		goto err_destroy_shader;

	printf("Fail 3\n");
	return shd;

err_destroy_shader:
	shd_destroy(shd);

err_return:
	printf("Failed to create shader: %s\n", name);
	FH_ALARM(FH_ERROR, "Failed to create new shader");
	return NULL;
}


FH_API struct fh_shader *fh_LoadShader(struct fh_context *ctx, char *name,
		char *v_pth, char *f_pth)
{
	struct fh_shader *shd;
	u32 size;
	void **p;

	if(!ctx || !name || !v_pth || !f_pth) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shd = shd_load(name, v_pth, f_pth)))
		goto err_return;

	/* Set additional attributes */
	shd->context = ctx;

	/* Insert shader into table */
	size = sizeof(struct fh_shader);
	p = (void **)&shd;
	if(fh_ContextAdd(ctx, FH_CONTEXT_SHADERS, name, size, p) < 0)
		goto err_destroy_shader;

	return shd;

err_destroy_shader:
	shd_destroy(shd);

err_return:
	printf("Failed to load shader: %s (%s, %s)\n", name, v_pth, f_pth);
	FH_ALARM(FH_ERROR, "Failed to load shader");
	return NULL;
}


FH_API void fh_RemoveShader(struct fh_shader *shd)
{
	if(!shd) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_ContextRemove(shd->context, FH_CONTEXT_SHADERS, shd->name);
}


FH_API struct fh_shader *fh_GetShader(struct fh_context *ctx, char *name)
{
	struct fh_shader *shader;

	if(!ctx || !name) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(fh_tbl_get(ctx->shaders, name, NULL, (void **)&shader) != 1)
		return NULL;

	return shader;

err_return:
	FH_ALARM(FH_ERROR, "Failed to get shader from shader table");
	return NULL;
}


FH_API void fh_UseShader(struct fh_shader *shd)
{
	if(!shd) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	/* First activate the shader */
	glUseProgram(shd->program);
} 


FH_API void fh_UnuseShader(void)
{
	glUseProgram(0);
}


FH_API s8 fh_ShaderGetInputLoc(struct fh_shader *shd, char *var)
{
	s8 i;

	if(!shd || !var) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < shd->inputs.num; i++) {
		if(strcmp(shd->inputs.vars[i].name, var) == 0)
			return i;
	}

	return -1;
}


FH_API s8 fh_ShaderGetUniformLoc(struct fh_shader *shd, char *uni)
{
	u8 i;

	if(!shd || !uni) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < shd->uniforms.num; i++) {
		if(strcmp(shd->uniforms.uniform[i].name, uni) == 0)
			return i;
	}

	return -1;
}


#if 0

FH_API void fh_shd_show(struct fh_shader *shd)
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
#endif
