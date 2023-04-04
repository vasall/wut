#ifndef _FH_SHADER_H
#define _FH_SHADER_H

#include "define.h"
#include "import.h"

#define FH_SHADER_LIM	128


struct fh_shader_variable {
	char name[256];

	u32 location;
};


struct fh_shader {
	/* The name of the shader */
	char name[128];

	/* The shader program descriptor */
	u32 program;

	/* The variables used by this shader */
	u8 var_num;
	struct fh_shader_variable *vars;
};

/*
 * Initialize the global shader list.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_shader_init(void);


/*
 * Destroy the global shader-list and free the allocated memory.
 */
FH_API void fh_shader_close(void);


/*
 * Insert a new shader into the global shader list.
 *
 * @shader: The shader to insert
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_shader_insert(struct fh_shader *shader);


/*
 * Remove a shader from the global shader list.
 *
 * @name: The name of the shader to remove
 */
FH_API void fh_shader_remove(char *name);


/*
 * Create a new shader.
 *
 * @name: The name of the shader
 * @vshader_src: The path to the vertex-shader
 * @fshader_src: The path to the fragment-shader
 * @var_num: The number of variables used by the shader
 * @vars: An array containing the names of all variables
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_shader_create(char *name, const char *vshader_src,
		const char *fshader_src, s8 var_num, char **vars);


/*
 * Destroy a shader and free the memory.
 *
 * @shader: A pointer to the shader to destroy
 */
FH_API void fh_shader_destroy(struct fh_shader *shader);


/*
 * Get a shader from the global shader list.
 *
 * @name: The name of the shader to get
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_shader_get(char *name);


/*
 * Activate a shader so it can be used for rendering.
 *
 * @shd: Pointer to the shader to use
 */
FH_API void fh_shader_use(struct fh_shader *shd); 


/*
 * Unuse the active shader.
 */
FH_API void fh_shader_unuse(void);


/*
 * The remove callback function given to the shader table.
 */
FH_API void fh_shader_rmv_fnc(u32 size, void *ptr);

#endif
