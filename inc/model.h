#ifndef _FH_MODEL_H
#define _FH_MODEL_H

#include "define.h"
#include "import.h"


#define FH_MODEL_NAME_LIM		128
#define FH_MODEL_ATTRIB_LIM		12
#define FH_MODEL_ATTRIB_NAME_LIM	128


struct fh_model_attribute {
	/* The name of the attribute */
	char name[FH_MODEL_ATTRIB_NAME_LIM];

	/* The location in the shader */
	u8 location;
};



struct fh_model {
	/*
	 * The name of the model.
	 */
	char name[FH_MODEL_NAME_LIM];

	/*
	 * The indices for rendering the model.
	 */
	u32 index_number;
	u32 *index_data;

	/*
	 * The data for all vertices.
	 */
	u32 vertex_number;
	u32 vertex_size;
	u8 *vertex_data;

	/*
	 * The array handles used by OpenGL.
	 */
	u32 vao;
	u32 ebo;
	u32 *bao;

	/* The uniform buffer */
	void *uniform_buffer;

	/*
	 * The attributes in used by the model in the shader.
	 */
	u8 attribute_number;
	struct fh_model_attribute attributes[FH_MODEL_ATTRIB_LIM];

	/* The name of the shader used for rendering the model */
	char shader_name[512];

	/* The name of the texture used for rendering the model */
	char texture_name[512];
};




/*
 * Initialize the global model list.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API u8 fh_mdl_init(void);


/*
 * Close the global model list.
 */
FH_API void fh_mdl_close(void);


/*
 * Create a new model and prepare for further configuration.
 *
 * @name: The name of the new model
 *
 * Returns: Either a pointer to the newly created model or NULL if an error
 * 	    occurred
 */
FH_API struct fh_model *fh_mdl_create(char *name);


/*
 * Finalize the model and add it to the model list.
 *
 * @mdl: Pointer to the model
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_mdl_finalize(struct fh_model *mdl);


/*
 * Remove a model from the models table.
 *
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_remove(struct fh_model *mdl);


/*
 * Destroy a model, close all OpenGL entries and free the allocated memory.
 *
 * @mdl: Pointer to the model
 */
FH_API void fh_mdl_destroy(struct fh_model *mdl);




/* The callback function to call when removing an entry from the models table */
FH_API void fh_mdl_rmv_fnc(u32 size, void *ptr);


#endif /* _FH_MODEL_H */
