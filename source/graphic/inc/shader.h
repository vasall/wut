#ifndef _WT_GRAPHIC_RESOURCES_SHADER_H
#define _WT_GRAPHIC_RESOURCES_SHADER_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/table.h"

#include "graphic/inc/context.h"

#define WT_SHADER_LIM		128
#define WT_SHADER_VAR_LIM	16
#define WT_SHADER_UNIFORM_LIM	8

/*
 * The following types are used to use different rendering options in the
 * shader.
 */
#define WT_RENTYPE_DEFAULT	1	/* Block with border and color */
#define WT_RENTYPE_SCROLL_V	2	/* Vertical scrollbar */
#define WT_RENTYPE_SCROLL_H	3	/* Horizontal scrollbar */


struct wt_shader_var {
	char name[64];
	char type[16];
	u16 location;
};

struct wt_shader_inputs {
	u8 num;
	struct wt_shader_var vars[WT_SHADER_VAR_LIM];
};



struct wt_shader_uniform {
	char name[64];
	s16 location;
};


struct wt_shader_uniforms {
	u8 num;
	struct wt_shader_uniform uniform[8];
};



struct wt_shader {
	/* The name of the shader */
	char name[128];

	/* The shader program descriptor */
	u32 program;

	/* The inputs for the shader */
	struct wt_shader_inputs inputs;

	/* The uniforms for the shader */
	struct wt_shader_uniforms uniforms;

	/* A reference to the context */
	struct wt_context *context;
};


/*
 * Create and initialize the shader table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_InitShaderTable(struct wt_context *ctx);


/*
 * Destroy and free the shader table attached to a context.
 *
 * @ctx: Pointer to the context
 */
WT_API void wt_CloseShaderTable(struct wt_context *ctx);


/*
 * This function will take the given source code for the vertex- and
 * fragment-shader and create a new OpenGL shader program and add it to the
 * shader table.
 *
 * @ctx: Pointer to the context
 * @name: The name of the shader
 * @v_src: The source code for the vertex-shader
 * @f_src: The source code for the fragment-shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
WT_API struct wt_shader *wt_CreateShader(struct wt_context *ctx, char *name,
		const char *v_src, const char *f_src);


/*
 * Load the given vertex- and fragment-shader-files and create an OpenGL shader
 * program and add it to the shader table.
 *
 * @ctx: Pointer to the context
 * @name: The name of the shader
 * @v_pth: The path to the vertex shader file
 * @f_pth: The path to the fragment shader file
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
WT_API struct wt_shader *wt_LoadShader(struct wt_context *ctx, char *name,
		char *v_pth, char *f_pth);


/*
 * Remove a shader from the global shader list.
 *
 * @shd: Pointer to the shader
 */
WT_API void wt_RemoveShader(struct wt_shader *shd);


/*
 * Get a pointer to a shader from the global shader list.
 *
 * @ctx: Pointer to the context
 * @name: The name of the shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
WT_API struct wt_shader *wt_GetShader(struct wt_context *ctx, char *name);


/*
 * Activate a shader so it can be used for rendering.
 *
 * @shd: Pointer to the shader
 */
WT_API void wt_UseShader(struct wt_shader *shd); 


/*
 * Unuse the active shader.
 */
WT_API void wt_UnuseShader(void);

/*
 * Get the location of a input variable in the shader.
 *
 * @shd: Pointer to the shader
 * @var: The name of the variable
 *
 * Returns: The location or -1 if an error occurred
 */
WT_API s8 wt_ShaderGetInputLoc(struct wt_shader *shd, char *var);


/*
 * Get the location of a uniform buffer in the shader.
 *
 * @shd: Pointer to the shader
 * @uni: The name of the uniform buffer
 *
 * Returns: The location or -1 if an error occurred
 */
WT_API s8 wt_ShaderGetUniformLoc(struct wt_shader *shd, char *uni);


#if 0

/*
 * Display all attributes of the shader in the console.
 *
 * @shd: Pointer to the shader
 */
WT_API void wt_shd_show_attrib(struct wt_shader *shd);
#endif

#endif /* _WT_GRAPHIC_RESOURCES_SHADER_H */
