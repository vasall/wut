#ifndef _FH_MODEL_H
#define _FH_MODEL_H

#include "define.h"
#include "datatype.h"
#include "table.h"
#include "import.h"


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




/*
 * Initialize the global model list.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_mdl_init(void);


/*
 * Close the global model list.
 */
FH_API void fh_mdl_close(void);


/*
 * Destroy a model, close all OpenGL entries and free the allocated memory.
 *
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_destroy(struct fh_model *mdl);


/*
 * Insert model into the model table. 
 *
 * @mdl: Pointer to the model
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_mdl_insert(struct fh_model *mdl);


/*
 * Remove a model from the model table.
 *
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_remove(struct fh_model *mdl);


/*
 * Render a model.
 * 
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_render(struct fh_model *mdl);



/* The callback function to call when removing an entry from the models table */
FH_API void fh_mdl_rmv_fnc(u32 size, void *ptr);


#endif /* _FH_MODEL_H */
