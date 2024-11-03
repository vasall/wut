#ifndef _WUT_GRAPHICSHADER_H
#define _WUT_GRAPHICSHADER_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "component/inc/table.h"

#include "graphic/inc/context.h"

#define WUT_SHADER_LIM		128
#define WUT_SHADER_VAR_LIM	16
#define WUT_SHADER_UNIFORM_LIM	8

/*
 * The following types are used to use different rendering options in the
 * shader.
 */
#define WUT_RENTYPE_DEFAULT	1	/* Block with border and color */
#define WUT_RENTYPE_SCROLL_V	2	/* Vertical scrollbar */
#define WUT_RENTYPE_SCROLL_H	3	/* Horizontal scrollbar */


struct wut_ShaderVar {
	char name[64];
	char type[16];
	u16 location;
};

struct wut_ShaderInputs {
	u8 num;
	struct wut_ShaderVar vars[WUT_SHADER_VAR_LIM];
};



struct wut_ShaderUniform {
	char name[64];
	s16 location;
};


struct wut_ShaderUniforms {
	u8 num;
	struct wut_ShaderUniform uniform[8];
};



struct wut_Shader {
	/* The name of the shader */
	char name[128];

	/* The shader program descriptor */
	u32 program;

	/* The inputs for the shader */
	struct wut_ShaderInputs inputs;

	/* The uniforms for the shader */
	struct wut_ShaderUniforms uniforms;

	/* A reference to the context */
	struct wut_Context *context;
};


/*
 * Create and initialize the shader table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_InitShaderTable(struct wut_Context *ctx);


/*
 * Destroy and free the shader table attached to a context.
 *
 * @ctx: Pointer to the context
 */
WUT_API void wut_CloseShaderTable(struct wut_Context *ctx);


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
WUT_API struct wut_Shader *wut_CreateShader(struct wut_Context *ctx, char *name,
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
WUT_API struct wut_Shader *wut_LoadShader(struct wut_Context *ctx, char *name,
		char *v_pth, char *f_pth);


/*
 * Remove a shader from the global shader list.
 *
 * @shd: Pointer to the shader
 */
WUT_API void wut_RemoveShader(struct wut_Shader *shd);


/*
 * Get a pointer to a shader from the global shader list.
 *
 * @ctx: Pointer to the context
 * @name: The name of the shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
WUT_API struct wut_Shader *wut_GetShader(struct wut_Context *ctx, char *name);


/*
 * Activate a shader so it can be used for rendering.
 *
 * @shd: Pointer to the shader
 */
WUT_API void wut_UseShader(struct wut_Shader *shd); 


/*
 * Unuse the active shader.
 */
WUT_API void wut_UnuseShader(void);

/*
 * Get the location of a input variable in the shader.
 *
 * @shd: Pointer to the shader
 * @var: The name of the variable
 *
 * Returns: The location or -1 if an error occurred
 */
WUT_API s8 wut_ShaderGetInputLoc(struct wut_Shader *shd, char *var);


/*
 * Get the location of a uniform buffer in the shader.
 *
 * @shd: Pointer to the shader
 * @uni: The name of the uniform buffer
 *
 * Returns: The location or -1 if an error occurred
 */
WUT_API s8 wut_ShaderGetUniformLoc(struct wut_Shader *shd, char *uni);


#if 0

/*
 * Display all attributes of the shader in the console.
 *
 * @shd: Pointer to the shader
 */
WUT_API void wut_shd_show_attrib(struct wut_Shader *shd);
#endif

#endif /* _WUT_GRAPHICSHADER_H */
