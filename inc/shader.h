#ifndef _FH_SHADER_H
#define _FH_SHADER_H

#include "define.h"
#include "datatype.h"
#include "table.h"
#include "import.h"
#include "window.h"

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
};

/*
 * Initialize the shader table for a window.
 *
 * @win: Pointer to the window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_shd_init(struct fh_window *win);


/*
 * Destroy the global shader-list and free the allocated memory.
 */
FH_API void fh_shd_close(struct fh_window *win);


/*
 * Insert a new shader into the shader table of a window.
 *
 * @win: Pointer to the window
 * @shader: The shader to insert
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_shd_insert(struct fh_window *win, struct fh_shader *shader);


/*
 * Remove a shader from the shader table of a window.
 * This will also destroy the shader.
 *
 * @win: Pointer to the window
 * @shader: A pointer to the shader to remove
 */
FH_API void fh_shd_remove(struct fh_window *win, struct fh_shader *shader);


/*
 * Create a new shader. This function will automatically extract all input
 * variables for the vertex shader.
 *
 * @name: The name of the shader
 * @v_src: The source code for the vertex-shader
 * @f_src: The source code for the fragment-shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_shd_create(char *name, char *v_src, char *f_src);


/*
 * This function will load the source code from the vertex- and
 * fragment-shader files and pass it onto the fh_shd_create() function.
 *
 * @name: The name of the shader
 * @v_pth: The path to the vertex-shader
 * @f_pth: The path ot the fragment-shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_shd_load(char *name, char *v_pth, char *f_pth);


/*
 * Destroy a shader and free the memory.
 *
 * @shader: A pointer to the shader to destroy
 */
FH_API void fh_shd_destroy(struct fh_shader *shader);


/*
 * Get a shader from the shader table of a window.
 *
 * @win: Pointer to the window
 * @name: The name of the shader to get
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_shd_get(struct fh_window *win, char *name);


/*
 * Activate a shader so it can be used for rendering.
 *
 * @shd: Pointer to the shader to use
 */
FH_API void fh_shd_use(struct fh_shader *shd); 


/*
 * Unuse the active shader.
 */
FH_API void fh_shd_unuse(void);


/*
 * Get the location of a input variable in the shader.
 *
 * @shd: Pointer to the shader
 * @var: The name of the variable
 *
 * Returns: The location or -1 if an error occurred
 */
FH_API s8 fh_shd_loc(struct fh_shader *shd, char *var);


/*
 * Display all attributes of the shader in the console.
 *
 * @shd: Pointer to the shader
 */
FH_API void fh_shd_show_attrib(struct fh_shader *shd);


/*
 * The remove callback function given to the shader table.
 */
FH_API void fh_shd_rmv_fnc(u32 size, void *ptr);

#endif
