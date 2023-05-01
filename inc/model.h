#ifndef _FH_MODEL_H
#define _FH_MODEL_H

#include "define.h"
#include "datatype.h"
#include "table.h"
#include "import.h"
#include "window.h"

#define FH_MODEL_NAME_LIM		128
#define FH_MODEL_ATTRIB_LIM		12
#define FH_MODEL_UNIFORM_LIM		6
#define FH_MODEL_ATTRIB_NAME_LIM	128
#define FH_MODEL_UNIFORM_NAME_LIM	128


struct fh_model_uniform {
	/* The name of the uniform buffer */
	char name[FH_MODEL_UNIFORM_NAME_LIM];

	/* The size of the uniform buffer in bytes */
	u32 size;

	/* The uniform buffer */
	void *data;

	u32 bao;
};


struct fh_model {
	/* The name of the model */
	char name[FH_MODEL_NAME_LIM];

	/* The vertex data */
	u32 vertex_number;
	u16 vertex_stride;
	u8 *vertex_buffer;

	/* The index data */
	u32 index_number;
	u32 *index_buffer;

	/* The vertex-array-object and buffer-array-objects */
	u32 vao;
	u32 bao;
	u32 ebo;

	/* References to both the shader and texture */
	struct fh_shader *shader;
	struct fh_texture *texture;

	/* The uniform buffers for this model */
	u8 uniform_number;
	struct fh_model_uniform uniforms[FH_MODEL_UNIFORM_LIM];
};



#include "model_constructor.h"


/*
 * Create and initialize a model table for a window.
 *
 * @win: Pointer to the window
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_mdl_init(struct fh_window *win);


/*
 * Close a model table and free the allocated memory.
 * 
 * @win: Pointer to the window
 */
FH_API void fh_mdl_close(struct fh_window *win);


/*
 * Create a finalized model from a model constructor.
 *
 * @win: Pointer to the window
 * @c: Pointer to the model constructor
 *
 * Returns: Either a pointer to the finished model or NULL if an error occurred
 */
FH_API struct fh_model *fh_mdl_create(struct fh_window *win,
		struct fh_model_c *c) ;


/*
 * Destroy a model, close all OpenGL entries and free the allocated memory.
 *
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_destroy(struct fh_model *mdl);


/*
 * Insert model into the model table. 
 *
 * @win: Pointer to the window
 * @mdl: Pointer to the model
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_mdl_insert(struct fh_window *win, struct fh_model *mdl);


/*
 * Remove a model from the model table.
 * This will also destroy the model.
 *
 * @win: Pointer to the window
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_remove(struct fh_window *win, struct fh_model *mdl);


/*
 * Get a model from the model table.
 *
 * @win: Pointer to the window
 * @name: The name of the model
 *
 * Returns: A pointer to the model or NULL if either an error occurred or the
 * 	    model could not be found
 */
FH_API struct fh_model *fh_mdl_get(struct fh_window *win, char *name);



/*
 * Update a uniform buffer.
 *
 * @mdl: Pointer to the model
 * @name: The name of the uniform
 * @ptr: A pointer to copy the data from
 */
FH_API void fh_mdl_set_uniform(struct fh_model *mdl, char *name, void *ptr);


/*
 * Render a model.
 * 
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_render(struct fh_model *mdl);



/* The callback function to call when removing an entry from the models table */
FH_API void fh_mdl_rmv_fnc(u32 size, void *ptr);


#endif /* _FH_MODEL_H */
