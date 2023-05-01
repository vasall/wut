#ifndef _FH_MODEL_CONSTRUCTOR_H
#define _FH_MODEL_CONSTRUCTOR_H

#include "define.h"
#include "datatype.h"
#include "window.h"

struct fh_model_c;


#include "model.h"


struct fh_model_c_attrib {
	/* The name of the attribute */
	char name[128];

	/* 
	 * The number of elements the attribute consists of for every vertex.
	 * The total size of the attribute for each vertex is callculated as:
	 * 	elements * element_size ie 3 * sizeof(f32)
	 */
	u8 elements;
	u8 element_size;

	/* The type of each element */
	GLenum type;

	/* the buffer containing the attribute data for all vertices */
	u8 *data;
};

struct fh_model_c_unibuf {
	/* The name of the new uniform buffer */
	char name[128];

	/* The size of the uniform buffer in bytes */
	u32 size;
};

struct fh_model_c {
	/* The name of the new model */
	char name[FH_MODEL_NAME_LIM];

	/* The shader to use for this model */
	char shader[128];

	/* The name of the texture to use for this model */
	char texture[128];

	/* The number of vertices */
	u32 vtx_num;

	/* The index data */
	u32 idx_num;
	u32 *idx;

	/* The attributes of the new model */
	u32 attrib_num;
	struct fh_model_c_attrib attribs[FH_MODEL_ATTRIB_LIM];

	/* The uniform buffers for the new model */
	u32 unibuf_num;
	struct fh_model_c_unibuf unibufs[FH_MODEL_UNIFORM_LIM]; 
};


/*
 * Create a new model constructor and begin creating a new model.
 *
 * @name: The name of the new model
 * @vnum: The number of vertices
 * @inum: The number of indices
 * @idx: The index data
 *
 * Returns: A new constructor or NULL if an error occurred
 */
FH_API struct fh_model_c *fh_mdlc_begin(char *name, u32 vnum,
		u32 inum, u32 *idx);


/*
 * Set the shader of the model during construction.
 *
 * @c: Pointer to the model constructor
 * @shader: The name of the shader(which has to be loaded already)
 */
FH_API void fh_mdlc_shader(struct fh_model_c *c, char *shader);


/*
 * Set the texture of the model during construction.
 *
 * @c: Pointer to the model constructor
 * @texture: The name of the texture(which has to be loaded already)
 */
FH_API void fh_mdlc_texture(struct fh_model_c *c, char *texture);


/*
 * Attach a new attribute to the model.
 *
 * @c: Pointer to the model constructor
 * @name: The name of the new attribute(has to match the shader input)
 * @size: The size of the element in data type quantities (ie 3 * GL_FLOAT)
 * @type: The data type of an element
 * @data: The buffer containing the data
 */
FH_API void fh_mdlc_attrib(struct fh_model_c *c, char *name, u8 size,
		GLenum type, void *data);


/*
 * Add a new uniform buffer to the model.
 *
 * @c: Pointer to the model constructor
 * @name: The name of the uniform buffer
 * @size: The size of the uniform buffer in bytes
 */
FH_API void fh_mdlc_uniform(struct fh_model_c *c, char *name, u32 size);


/*
 * Finalize the model and get the model. To destroy the constructor call
 * fh_mdlc_destroy().
 *
 * @c: Pointer to the model constructor
 */
FH_API struct fh_model *fh_mdlc_finish(struct fh_model_c *c);


/*
 * Destroy the model constructor and free the allocated memory.
 */
FH_API void fh_mdlc_destroy(struct fh_model_c *c);





#endif /* _FH_MODEL_CONSTRUCTOR_H */
