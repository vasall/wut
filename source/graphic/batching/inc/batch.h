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

#define FH_UNIFORM_F_ALL		(1<<0)
#define FH_UNIFORM_F_CLEANUP	(1<<1)

struct fh_uniform_temp {
	char 					name[256]; /* Name of uniform in the shader */
	enum fh_uniform_type	type;	   /* Variable type in the shader */
	int 					limit;     /* Number of preallocated slots */
	u8						flags;	   /* Behaviour flags */
};



struct fh_uniform {
	int 					slot;
	enum fh_uniform_type	type;
	int 					count;
	int 					number;
	int 					limit;
	u8 						*data;
	u8 						flags;
	int 					size;
};


struct fh_batch {
	GLuint shader;

	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	/* vertex buffer data */
	int vertex_size;
	int vertex_count;
	int vertex_capacity;
	unsigned char *vertices;

	/* index buffer data */
	int index_size;
	int index_count;
	int index_capacity;
	unsigned int *indices;

	/* uniform data */
	int uniform_count;
	struct fh_uniform uniforms[5];
};

/*
 * Create a new batch renderer.
 *
 * @attribnum: The number of attributes for the vertices
 * @attribs: A list of all attributes for the vertices
 * @vlimit: The vertex capacity
 * @ilimit: The index capacity
 * @uninum: The number of uniforms used by this batch renderer
 * @uniforms: A list of all uniforms
 *
 * Returns: Either a new batch renderer or NULL if an error occurred
 */
extern struct fh_batch *fh_batch_create(int attribnum, struct fh_vertex_attrib *attribs, int vertex_capacity,
		int index_capacity, int uniformnum, struct fh_uniform_temp *uniforms);


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
 * Set the data of a uniform of the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 * @ptr: Pointer to the data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_XMOD s8 fh_batch_set_uniform(struct fh_batch *ren, u32 slot, void *ptr);


/*
 * Push a new uniform entry into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 * @ptr: Pointer to the data
 *
 * Returns: Either the index of the new entry in the uniform list or -1 if an
 *	        error occurred
 */
FH_XMOD s32 fh_batch_push_uniform(struct fh_batch *ren, int slot, void *ptr);


/*
 * Reset a uniforms data buffer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 */
FH_XMOD void fh_batch_reset_uniform(struct fh_batch *ren, int slot);


/*
 * Pass the data from the batch renderer onto OpenGL.
 *
 * @ren: Pointer to the batch renderer
 */
FH_XMOD void fh_batch_flush(struct fh_batch *ren);

#endif /* _FH_GRAPHIC_BATCHING_BATCH_H */
