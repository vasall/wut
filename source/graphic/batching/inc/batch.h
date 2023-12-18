#ifndef _FH_GRAPHIC_BATCHING_BATCH_H
#define _FH_GRAPHIC_BATCHING_BATCH_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/resources/inc/shader.h"




/*
 * This struct is used to represent a single attribute of a vertex like the
 * vertex position, color or UV-coordinates.
 */
struct fh_vertex_attrib {
	int 	number;		/* The number of elements */
	GLenum 	type;		/* The type of element */
};

enum fh_uniform_type {
	FH_UNIFORM_1F,
	FH_UNIFORM_2F,
	FH_UNIFORM_3F,
	FH_UNIFORM_4F,

	FH_UNIFORM_1I,
	FH_UNIFORM_2I,
	FH_UNIFORM_3I,
	FH_UNIFORM_4I,

	FH_UNIFORM_1UI,
	FH_UNIFORM_2UI,
	FH_UNIFORM_3UI,
	FH_UNIFORM_4UI,

	FH_UNIFORM_1FV,
	FH_UNIFORM_2FV,
	FH_UNIFORM_3FV,
	FH_UNIFORM_4FV,

	FH_UNIFORM_1IV,
	FH_UNIFORM_2IV,
	FH_UNIFORM_3IV,
	FH_UNIFORM_4IV,

	FH_UNIFORM_1UIV,
	FH_UNIFORM_2UIV,
	FH_UNIFORM_3UIV,
	FH_UNIFORM_4UIV,

	FH_UNIFORM_M2FV,
	FH_UNIFORM_M3FV,
	FH_UNIFORM_M4FV
};

#define FH_UNIFORM_F_ALL	(1<<0)
#define FH_UNIFORM_F_CLEANUP	(1<<1)

struct fh_uniform_temp {
	char 			name[256]; /* Name of uniform in the shader */
	enum fh_uniform_type	type;	   /* Variable type in the shader */
	u32 			limit;     /* Number of preallocated slots */
	u8			flags;	   /* Behaviour flags */
};



struct fh_uniform {
	u32 			location;
	enum fh_uniform_type	type;
	u32 			size;
	u32 			number;
	u32 			limit;
	u8 			*data;
	u8 			flags;
};


struct fh_batch {
	struct fh_shader 	*shader;

	/* 0: VAO, 1: VBO, 2: IBO */
	u32 			gl_objects[3];

	/* vertex buffer data */
	u32 			vertex_size;
	u32 			vertex_number;
	u32 			vertex_limit;
	u8 			*vertices;

	/* index buffer data */
	u32 			index_size;
	u32 			index_number;
	u32 			index_limit;
	u8 			*indices;

	/* uniform data  */
	u32 			uniform_number;
	struct fh_uniform 	*uniforms;
};


/*
 * Create a new batch renderer.
 *
 * @shd: Pointer to the shader to use for this batch renderer
 * @attribnum: The number of attributes for the vertices
 * @attribs: A list of all attributes for the vertices
 * @vlimit: The vertex capacity
 * @ilimit: The index capacity
 * @uninum: The number of uniforms used by this batch renderer
 * @uniforms: A list of all uniforms
 *
 * Returns: Either a new batch renderer or NULL if an error occurred
 */
FH_XMOD struct fh_batch *fh_batch_create(struct fh_shader *shd, u32 attribnum, 
		struct fh_vertex_attrib *vattribs, u32 vlimit, u32 ilimit,
		u32 uninum, struct fh_uniform_temp *unis);


/*
 * Push a new vertex into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @ptr:  Pointer to the data
 *
 * Return: The index of the pushed vertex in the vertex array or -1 if an error
 * 	   occurred
 */
FH_XMOD s32 fh_batch_push_vertex(struct fh_batch *ren, void *ptr);


/*
 * Push a new index into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @idx: The index
 *
 * Returns: The index of the pushed index in the index array or -1 if an error
 * 	    occurred
 */
FH_XMOD s32 fh_batch_push_index(struct fh_batch *ren, u32 idx);


/*
 * Push a new uniform entry into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 * @ptr: Pointer to the data
 */
FH_XMOD s32 fh_batch_push_uniform(struct fh_batch *ren, u32 slot, void *ptr);


/*
 * Reset a uniforms data buffer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 */
FH_XMOD void fh_batch_reset_uniform(struct fh_batch *ren, u32 slot);


/*
 * Pass the data from the batch renderer onto OpenGL.
 *
 * @ren: Pointer to the batch renderer
 */
FH_XMOD void fh_batch_flush(struct fh_batch *ren);

#endif /* _FH_GRAPHIC_BATCHING_BATCH_H */
