#ifndef _WUT_GRAPHIC_BATCH_H
#define _WUT_GRAPHIC_BATCH_H

#include "core/inc/define.h"
#include "core/inc/import.h"


struct wut_Batch;

#include "graphic/inc/shader.h"
#include "graphic/inc/texture.h"

/*
 * This struct is used to represent a single attribute of a vertex like the
 * vertex position, color or UV-coordinates.
 */
struct wut_VertexAttrib {
	s32 	number;		/* The number of elements */
	GLenum 	type;		/* The type of element */
};

enum wut_eUniformType {
	WUT_UNI_1F,
	WUT_UNI_2F,
	WUT_UNI_3F,
	WUT_UNI_4F,

	WUT_UNI_1I,
	WUT_UNI_2I,
	WUT_UNI_3I,
	WUT_UNI_4I,

	WUT_UNI_1UI,
	WUT_UNI_2UI,
	WUT_UNI_3UI,
	WUT_UNI_4UI,

	WUT_UNI_1FV,
	WUT_UNI_2FV,
	WUT_UNI_3FV,
	WUT_UNI_4FV,

	WUT_UNI_1IV,
	WUT_UNI_2IV,
	WUT_UNI_3IV,
	WUT_UNI_4IV,

	WUT_UNI_1UIV,
	WUT_UNI_2UIV,
	WUT_UNI_3UIV,
	WUT_UNI_4UIV,

	WUT_UNI_M2FV,
	WUT_UNI_M3FV,
	WUT_UNI_M4FV
};

#define WUT_UNI_F_ALL		(1<<0)
#define WUT_UNI_F_CLEANUP		(1<<1)

#define WUT_UNI_F_DEFAULT		WUT_UNI_F_ALL|WUT_UNI_F_CLEANUP

struct wut_UniformTemp {
	char 			name[256]; /* Name of uniform in the shader */
	enum wut_eUniformType	type;	   /* Variable type in the shader */
	s32 			limit;     /* Number of preallocated slots */
	u8			flags;	   /* Behaviour flags */
};



struct wut_Uniform {
	s32 			slot;
	enum wut_eUniformType	type;
	s32 			count;
	s32 			number;
	s32 			limit;
	u8 			*data;
	u8 			flags;
	s32 			size;
};


typedef void (*wut_BatchFunc)(struct wut_Batch *ren, void *data);

struct wut_Batch {
	struct wut_Shader *shader;
	struct wut_Texture *texture;

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
	struct wut_Uniform *uniforms;

	/* Optional functions */
	wut_BatchFunc pre_fnc;
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
WUT_API struct wut_Batch *wut_bat_create(struct wut_Shader *shd,
		struct wut_Texture *tex,s32 attribnum,
		struct wut_VertexAttrib *attribs, s32 vtx_cap,
		s32 idx_cap, s32 uninum, struct wut_UniformTemp *unis,
		wut_BatchFunc pre, void *pre_data);


/*
 * Destroy a batch renderer and free the allocated memory.
 *
 * @ren: Pointer to the batch renderer
 */
WUT_API void wut_bat_destroy(struct wut_Batch *ren);


/*
 * Push a new vertex into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @ptr:  Pointer to the data
 *
 * Return: The index of the pushed vertex in the vertex array or -1 if an error
 * 	   occurred
 */
WUT_XMOD s32 wut_bat_push_vertex(struct wut_Batch *ren, void *ptr);


/*
 * Push a new index into the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @idx: The index
 *
 * Returns: The index of the pushed index in the index array or -1 if an error
 * 	    occurred
 */
WUT_XMOD s32 wut_bat_push_index(struct wut_Batch *ren, u32 idx);


/*
 * Set the data of a uniform of the batch renderer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 * @ptr: Pointer to the data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_XMOD s8 wut_bat_set_uniform(struct wut_Batch *ren, u32 slot, void *ptr);


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
WUT_XMOD s32 wut_bat_push_uniform(struct wut_Batch *ren, s32 slot, void *ptr);


/*
 * Reset a uniforms data buffer.
 *
 * @ren: Pointer to the batch renderer
 * @slot: The slot of the uniform in the batch renderer
 */
WUT_XMOD void wut_bat_reset_uniform(struct wut_Batch *ren, s32 slot);


/*
 * Pass the data from the batch renderer onto OpenGL.
 *
 * @ren: Pointer to the batch renderer
 */
WUT_XMOD void wut_bat_flush(struct wut_Batch *ren);

#endif /* _WUT_GRAPHIC_BATCHING_BATCH_H */
