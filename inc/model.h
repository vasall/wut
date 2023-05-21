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

	s32 location;
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

	/* Pointer to the context this model belongs to */
	struct fh_context *context;

	vec3_t position;
	vec3_t rotation;
};


/*
 * Update a uniform buffer.
 *
 * @mdl: Pointer to the model
 * @name: The name of the uniform
 * @ptr: A pointer to copy the data from
 */
FH_API void fh_mdl_set_uniform(struct fh_model *mdl, char *name, void *ptr);



/* The callback function to call when removing an entry from the models table */
FH_API void fh_mdl_rmv_fnc(u32 size, void *ptr);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Create and initialize a model table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_InitModelTable(struct fh_context *ctx);


/*
 * Close a model table and free the allocated memory.
 * 
 * @ctx: Pointer to the context
 */
FH_API void fh_CloseModelTable(struct fh_context *ctx);


/*
 * Remove a model from the context it belongs to.
 *
 * @mdl: Pointer to the model
 */
FH_API void fh_RemoveModel(struct fh_model *mdl);


/*
 * Retrieve a model from the model table of a context.
 *
 * @ctx: Pointer to the context
 * @name: The name of the model
 *
 * Returns: A pointer to the model or NULL if either an error occurred or the
 * 	    model could not be found
 */
FH_API struct fh_model *fh_GetModel(struct fh_context *ctx, char *name);


/*
 * Update a uniform buffer of a model.
 *
 * @mdl: Pointer to the model
 * @name: The name of the uniform buffer
 * @ptr: A pointer with the new data
 */
FH_API void fh_SetModelUniform(struct fh_model *mdl, char *name, void *ptr);


/*
 * Render a model.
 * 
 * @mdl: Pointer to the model
 * @[shd]: The shader to be used
 * @[tex]: The texture to be used
 */
FH_API void fh_RenderModel(struct fh_model *mdl, struct fh_shader *shd,
		struct fh_texture *tex);


#endif /* _FH_MODEL_H */
