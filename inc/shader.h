#ifndef _FH_SHADER_H
#define _FH_SHADER_H

#include "stdinc.h"
#include "table.h"
#include "context.h"

#define FH_SHADER_LIM		128
#define FH_SHADER_VAR_LIM	16


struct fh_shader_var {
	char name[64];
	char type[16];
	u16 location;
};

struct fh_shader_inputs {
	u8 num;
	struct fh_shader_var vars[FH_SHADER_VAR_LIM];
};


struct fh_shader {
	/* The name of the shader */
	char name[128];

	/* The shader program descriptor */
	u32 program;

	/* The inputs for the shader */
	struct fh_shader_inputs inputs;

	/* A reference to the context */
	struct fh_context *context;
};


/*
 * Create and initialize the shader table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_InitShaderTable(struct fh_context *ctx);


/*
 * Destroy and free the shader table attached to a context.
 *
 * @ctx: Pointer to the context
 */
FH_API void fh_CloseShaderTable(struct fh_context *ctx);


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
FH_API struct fh_shader *fh_CreateShader(struct fh_context *ctx, char *name,
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
FH_API struct fh_shader *fh_LoadShader(struct fh_context *ctx, char *name,
		char *v_pth, char *f_pth);


/*
 * Remove a shader from the global shader list.
 *
 * @shd: Pointer to the shader
 */
FH_API void fh_RemoveShader(struct fh_shader *shd);


/*
 * Get a pointer to a shader from the global shader list.
 *
 * @ctx: Pointer to the context
 * @name: The name of the shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_GetShader(struct fh_context *ctx, char *name);


/*
 * Activate a shader so it can be used for rendering.
 *
 * @shd: Pointer to the shader
 */
FH_API void fh_UseShader(struct fh_shader *shd); 


/*
 * Unuse the active shader.
 */
FH_API void fh_UnuseShader(void);

/*
 * Get the location of a input variable in the shader.
 *
 * @shd: Pointer to the shader
 * @var: The name of the variable
 *
 * Returns: The location or -1 if an error occurred
 */
FH_API s8 fh_ShaderGetLocation(struct fh_shader *shd, char *var);

#if 0

/*
 * Display all attributes of the shader in the console.
 *
 * @shd: Pointer to the shader
 */
FH_API void fh_shd_show_attrib(struct fh_shader *shd);
#endif

#endif
