#ifndef _FH_MODEL_CONSTRUCTOR_H
#define _FH_MODEL_CONSTRUCTOR_H

#include "stdinc.h"

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

enum fh_model_c_type {
	FH_MODELC_DEFAULT,
	FH_MODELC_RIGGED,
	FH_MODELC_CUSTOM
};

struct fh_model_c {
	/* The name of the new model */
	char name[FH_MODEL_NAME_LIM];

	/* The construction process to utilize */
	enum fh_model_c_type type;

	/* The shader to use for the model */
	struct fh_shader *shader;

	/* The texture to use for the model */
	struct fh_texture *texture;

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
FH_API struct fh_model_c *fh_BeginModelConstr(char *name,
		u32 vnum, u32 inum, u32 *idx);


/*
 * Finalize construction and get the finished model. This function will also
 * insert the model into the model table of the given context.
 *
 * @c: Pointer to the constructor
 * @ctx: Pointer to the context
 * @pos: The initial position of the model
 * @rot: The initial rotation of the model
 *
 * Returns: Either a pointer to the finished model or NULL if an error occurred
 */
FH_API struct fh_model *fh_EndModelConstr(struct fh_model_c *c,
		struct fh_context *ctx, vec3_t pos, vec3_t rot);


/*
 * After the model constructor has been utilized and is not need anymore, this
 * function should be called to cleanup everything and free the allocated
 * memory. The models that have been created using the constructor are
 * unaffected.
 *
 * @c: Pointer to the constructor
 */
FH_API void fh_ModelConstrCleanup(struct fh_model_c *c);


/*
 * Set the texture of the model during construction.
 *
 * @c: Pointer to the model constructor
 * @tex: Pointer to the texture
 */
FH_API void fh_ModelConstrTexture(struct fh_model_c *c, struct fh_texture *tex);


/*
 * Set the shader of the model during construction.
 *
 * @c: Pointer to the model constructor
 * @shd: Pointer to the shader
 */
FH_API void fh_ModelConstrShader(struct fh_model_c *c, struct fh_shader *shd);


/*
 * Attach a new attribute to the model.
 *
 * Use like: fh_ModelConstrAttrib(c, "v_pos", 3, GL_FLOAT, vtx);
 *
 * @c: Pointer to the model constructor
 * @name: The name of the new attribute(has to match the shader input)
 * @size: The size of the element in data type quantities (ie 3 * GL_FLOAT)
 * @type: The data type of an element
 * @data: The buffer containing the data
 */
FH_API void fh_ModelConstrAttrib(struct fh_model_c *c, char *name, u8 size,
		GLenum type, void *data);


/*
 * Add a new uniform buffer to the model.
 *
 * Use like: fh_ModelConstrUniform(c, "camera", sizeof(struct uniform_buffer));
 *
 * @c: Pointer to the model constructor
 * @name: The name of the uniform buffer
 * @size: The size of the uniform buffer in bytes
 */
FH_API void fh_ModelConstrUniform(struct fh_model_c *c, char *name, u32 size);



#endif /* _FH_MODEL_CONSTRUCTOR_H */
