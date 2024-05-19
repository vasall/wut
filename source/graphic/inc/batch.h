#ifndef _WT_GRAPHIC_BATCH_H
#define _WT_GRAPHIC_BATCH_H

#include "core/inc/define.h"
#include "core/inc/import.h"


struct wt_batch;

#include "graphic/inc/shader.h"
#include "graphic/inc/texture.h"

/*
 * This struct is used to represent a single attribute of a vertex like the
 * vertex position, color or UV-coordinates.
 */
struct wt_vertex_attrib {
	s32 	number;		/* The number of elements */
	GLenum 	type;		/* The type of element */
};

enum wt_uniform_type {
	WT_UNIFORM_1F,
	WT_UNIFORM_2F,
	WT_UNIFORM_3F,
	WT_UNIFORM_4F,

	WT_UNIFORM_1I,
	WT_UNIFORM_2I,
	WT_UNIFORM_3I,
	WT_UNIFORM_4I,

	WT_UNIFORM_1UI,
	WT_UNIFORM_2UI,
	WT_UNIFORM_3UI,
	WT_UNIFORM_4UI,

	WT_UNIFORM_1FV,
	WT_UNIFORM_2FV,
	WT_UNIFORM_3FV,
	WT_UNIFORM_4FV,

	WT_UNIFORM_1IV,
	WT_UNIFORM_2IV,
	WT_UNIFORM_3IV,
	WT_UNIFORM_4IV,

	WT_UNIFORM_1UIV,
	WT_UNIFORM_2UIV,
	WT_UNIFORM_3UIV,
	WT_UNIFORM_4UIV,

	WT_UNIFORM_M2FV,
	WT_UNIFORM_M3FV,
	WT_UNIFORM_M4FV
};

#define WT_UNIFORM_F_ALL		(1<<0)
#define WT_UNIFORM_F_CLEANUP		(1<<1)

#define WT_UNIFORM_F_DEFAULT		WT_UNIFORM_F_ALL|WT_UNIFORM_F_CLEANUP

struct wt_uniform_temp {
	char 			name[256]; /* Name of uniform in the shader */
	enum wt_uniform_type	type;	   /* Variable type in the shader */
	s32 			limit;     /* Number of preallocated slots */
	u8			flags;	   /* Behaviour flags */
};



struct wt_uniform {
	s32 			slot;
	enum wt_uniform_type	type;
	s32 			count;
	s32 			number;
	s32 			limit;
	u8 			*data;
	u8 			flags;
	s32 			size;
};


typedef void (*wt_batch_cfnc)(struct wt_batch *ren, void *data);

struct wt_batch {
	struct wt_shader *shader;
	struct wt_texture *texture;

	u32 vao;
	u32 vbo;
	u32 ibo;

	/* vertex buffer data */
	s32 vertex_size;
	s32 vertex_count;
	s32 vertex_capacity;
	u8 *vertices;

	/* index buffer data */
	s32 index_size;
	s32 index_count;
	s32 index_capacity;
	u32 *indices;

	/* uniform data */
	s32 uniform_count;
	struct wt_uniform *uniforms;

	/* Optional functions */
	wt_batch_cfnc pre_fnc;
	void *pre_fnc_data;
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Create a new batch renderer.
 *
 * @shd: Pointer to the shader to used for this batch renderer
 * @tex: Pointer to the texture or NULL if no texture is used
 * @attribnum: The number of attributes for the vertices
 * @attribs: A list of all attributes for the vertices
 * @vlimit: The vertex capacity
 * @ilimit: The index capacity
 * @uninum: The number of uniforms used by this batch renderer
 * @uniforms: A list of all uniforms
 *
 * Returns: Either a new batch renderer or NULL if an error occurred
 */
WT_API struct wt_batch *wt_batch_create(struct wt_shader *shd,
		struct wt_texture *tex,s32 attribnum,
		struct wt_vertex_attrib *attribs, s32 vtx_cap,
		s32 idx_cap, s32 uninum, struct wt_uniform_temp *unis,
		wt_batch_cfnc pre, void *pre_data);


/*
 * Destroy a batch renderer and free the allocated memory.
 *
 * @ren: Pointer to the batch renderer
 */
WT_API void wt_batch_destroy(struct wt_batch *ren);


/*
 * Push a new vertex into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @ptr:  Pointer to the data
 *
 * Return: The index of the pushed vertex in the vertex array or -1 if an error
 * 	   occurred
 */
WT_XMOD s32 wt_batch_push_vertex(struct wt_batch *ren, void *ptr);


/*
 * Push a new index into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @idx: The index
 *
 * Returns: The index of the pushed index in the index array or -1 if an error
 * 	    occurred
 */
WT_XMOD s32 wt_batch_push_index(struct wt_batch *ren, u32 idx);


/*
 * Set the data of a uniform of the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 * @ptr: Pointer to the data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_XMOD s8 wt_batch_set_uniform(struct wt_batch *ren, u32 slot, void *ptr);


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
WT_XMOD s32 wt_batch_push_uniform(struct wt_batch *ren, s32 slot, void *ptr);


/*
 * Reset a uniforms data buffer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 */
WT_XMOD void wt_batch_reset_uniform(struct wt_batch *ren, s32 slot);


/*
 * Pass the data from the batch renderer onto OpenGL.
 *
 * @ren: Pointer to the batch renderer
 */
WT_XMOD void wt_batch_flush(struct wt_batch *ren);

#endif /* _WT_GRAPHIC_BATCHING_BATCH_H */
