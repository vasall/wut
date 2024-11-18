#include "source/graphic/inc/shader.h"

#include "source/utility/inc/alarm.h"

#include "source/core/inc/core.h"

#include "source/system/inc/file.h"

#include <stdlib.h>


#define WUT_SHADER_DEBUG	1


WUT_INTERN s8 shd_new_shader(u32 type, const char *src, u32 *shd_out)
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
		WUT_ALARM(WUT_ERROR, info_log);
		WUT_ALARM(WUT_ERROR, "Failed to compile vertex shader");

		glDeleteShader(shd);
		return -1;
	}

	*shd_out = shd;
	return 0;
}

WUT_INTERN s8 shd_extract_inputs(struct wut_ShaderInputs *inp, const char *str)
{
	char *line = (char *)str;
	struct wut_ShaderVar *var;
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

WUT_INTERN s8 shd_extract_uniforms(struct wut_ShaderUniforms *u, const char *str)
{
	char *runner = (char *)str;
	char swap[64];

	u32 counter = 0;
	u32 i = 0;

	struct wut_ShaderUniform *uni;

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


WUT_INTERN struct wut_Shader *shd_create(char *name, const char *v_src,
		const char *f_src)
{
	struct wut_Shader *shader;

	u32 vshader;
	u32 fshader;

	s32 success;
	char info_log[512];

	/*
	 * Allocate memory for shader.
	 */
	if(!(shader = wut_malloc(sizeof(struct wut_Shader)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for new shader");
		goto err_return;
	}

	/*
	 * Extract the input variables from the vertex-shader.
	 */
	if(shd_extract_inputs(&shader->inputs, v_src) < 1) {
		WUT_ALARM(WUT_ERROR, "Failed to extract input");
		goto err_free_shader;
	}

	if(shd_extract_uniforms(&shader->uniforms, v_src) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to extract uniforms");
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
		WUT_ALARM(WUT_ERROR, "Failed to create shader program");
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
		WUT_ALARM(WUT_ERROR, info_log);
		WUT_ALARM(WUT_ERROR, "Failed to link shaders to program");
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
	wut_free(shader);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new shader");
	return NULL;
}


WUT_INTERN struct wut_Shader *shd_load(char *name, char *v_pth, char *f_pth)
{
	char *v_buf = NULL;
	char *f_buf = NULL;
	struct wut_Shader *shader;

	/*
	 * Read the vertex and fragment shader files.
	 */
	if(wut_fs_text(v_pth, NULL, &v_buf) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to load the vertex shader file");
		goto err_return;
	}

	if(wut_fs_text(f_pth, NULL, &f_buf) < 0) {
		WUT_ALARM(WUT_ERROR, "Failed to load the fragment shader file");
		goto err_free_v_buf;
	}

	/*
	 * Pass the source code for the shaders to the create function and
	 * create shader program.
	 */
	if(!(shader = shd_create(name, v_buf, f_buf)))
		goto err_free_f_buf;

	wut_free(v_buf);
	wut_free(f_buf);

	return shader;

err_free_f_buf:
	wut_free(v_buf);

err_free_v_buf:
	wut_free(f_buf);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load new shader");
	return NULL;
}


WUT_INTERN void shd_destroy(struct wut_Shader *shd)
{
	glDeleteProgram(shd->program);

	wut_free(shd);
}


WUT_INTERN void shd_rmv_fnc(u32 size, void *ptr)
{
	struct wut_Shader *shader;

	WUT_IGNORE(size);

	if(!ptr)
		return;

	shader = (struct wut_Shader *)ptr;
	shd_destroy(shader);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API s8 wut_InitShaderTable(struct wut_Context *ctx)
{
	struct wut_Table *tbl;
	float vec[3];

	if(!ctx) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	wut_vec3_set(vec, 1, 2, 3);

	if(!(tbl = wut_CreateTable(&shd_rmv_fnc))) {
		WUT_ALARM(WUT_ERROR, "Failed to create wut_Table");
		goto err_return;
	}

	/* Attach the shader table to the context */
	ctx->shaders = tbl;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to initialize shader table");
	return -1;
}


WUT_API void wut_CloseShaderTable(struct wut_Context *ctx)
{
	if(!ctx) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_DestroyTable(ctx->shaders);
	ctx->shaders = NULL;
}


WUT_API struct wut_Shader *wut_CreateShader(struct wut_Context *ctx, char *name,
		const char *v_src, const char *f_src)
{
	struct wut_Shader *shd;
	u32 size;
	void **p;

	if(!ctx || !name || !v_src || !f_src) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shd = shd_create(name, v_src, f_src)))
		goto err_return;

	/* Set additional attributes */
	shd->context = ctx;

	/* Insert shader into table */
	size = sizeof(struct wut_Shader);
	p = (void **)&shd;
	if(wut_ContextAdd(ctx, WUT_CONTEXT_SHADERS, name, size, p) < 0)
		goto err_destroy_shader;

	return shd;

err_destroy_shader:
	shd_destroy(shd);

err_return:
	printf("Failed to create shader: %s\n", name);
	WUT_ALARM(WUT_ERROR, "Failed to create new shader");
	return NULL;
}


WUT_API struct wut_Shader *wut_LoadShader(struct wut_Context *ctx, char *name,
		char *v_pth, char *f_pth)
{
	struct wut_Shader *shd;
	u32 size;
	void **p;

	if(!ctx || !name || !v_pth || !f_pth) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shd = shd_load(name, v_pth, f_pth)))
		goto err_return;

	/* Set additional attributes */
	shd->context = ctx;

	/* Insert shader into table */
	size = sizeof(struct wut_Shader);
	p = (void **)&shd;
	if(wut_ContextAdd(ctx, WUT_CONTEXT_SHADERS, name, size, p) < 0)
		goto err_destroy_shader;

	return shd;

err_destroy_shader:
	shd_destroy(shd);

err_return:
	printf("Failed to load shader: %s (%s, %s)\n", name, v_pth, f_pth);
	WUT_ALARM(WUT_ERROR, "Failed to load shader");
	return NULL;
}


WUT_API void wut_RemoveShader(struct wut_Shader *shd)
{
	if(!shd) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_ContextRemove(shd->context, WUT_CONTEXT_SHADERS, shd->name);
}


WUT_API struct wut_Shader *wut_GetShader(struct wut_Context *ctx, char *name)
{
	struct wut_Shader *shader;

	if(!ctx || !name) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(wut_GetTable(ctx->shaders, name, NULL, (void **)&shader) != 1)
		return NULL;

	return shader;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to get shader from shader table");
	return NULL;
}


WUT_API void wut_UseShader(struct wut_Shader *shd)
{
	if(!shd) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	/* First activate the shader */
	glUseProgram(shd->program);
} 


WUT_API void wut_UnuseShader(void)
{
	glUseProgram(0);
}


WUT_API s8 wut_ShaderGetInputLoc(struct wut_Shader *shd, char *var)
{
	s8 i;

	if(!shd || !var) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < shd->inputs.num; i++) {
		if(strcmp(shd->inputs.vars[i].name, var) == 0)
			return i;
	}

	return -1;
}


WUT_API s8 wut_ShaderGetUniformLoc(struct wut_Shader *shd, char *uni)
{
	u8 i;

	if(!shd || !uni) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return -1;
	}

	for(i = 0; i < shd->uniforms.num; i++) {
		if(strcmp(shd->uniforms.uniform[i].name, uni) == 0)
			return i;
	}

	return -1;
}


#if 0

WUT_API void wut_shd_show(struct wut_Shader *shd)
{
	s32 i;
	struct wut_ShaderVar *var;

	if(!shd)
		return;


	printf("Shader attributes for %s:\n", shd->name);

	for(i = 0; i < shd->inputs.num; i++) {
		var = &shd->inputs.vars[i];
		printf("[%d]: %s %s\n", var->location, var->type, var->name);
	}
}
#endif
