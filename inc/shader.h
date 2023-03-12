#ifndef _FH_SHADER_H
#define _FH_SHADER_H

#include "define.h"
#include "import.h"


struct fh_shader {
	/* The name of the shader */
	char name[128];

	/* The shader program descriptor */
	u32 program;
};



/*
 * Create a new shader.
 *
 * @vshader_src: The path to the vertex-shader
 * @fshader_src: The path to the fragment-shader
 *
 * Returns: Either a pointer to the created shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_shader_create(const char *vshader_src,
		const char *fshader_src);


/*
 * Destroy a shader and free the memory.
 *
 * @shader: Pointer to the shader
 */
FH_API void fh_shader_destroy(struct fh_shader *shader);


#endif
