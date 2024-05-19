#ifndef _WT_GRAPHIC_CONSTRUCTOR_H
#define _WT_GRAPHIC_CONSTRUCTOR_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/object.h"

#include "window/inc/window.h"

struct wt_object_c;


#include "graphic/inc/object.h"


struct wt_object_c_attrib {
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

struct wt_object_c_unibuf {
	/* The name of the new uniform buffer */
	char name[128];

	/* The size of the uniform buffer in bytes */
	u32 size;
};

enum wt_object_c_type {
	WT_OBJECTC_DEFAULT,
	WT_OBJECTC_RIGGED,
	WT_OBJECTC_CUSTOM
};

struct wt_object_c {
	/* The name of the new object */
	char name[WT_OBJECT_NAME_LIM];

	/* The construction process to utilize */
	enum wt_object_c_type type;

	/* The shader to use for the object */
	struct wt_shader *shader;

	/* The texture to use for the object */
	struct wt_texture *texture;

	/* The number of vertices */
	u32 vtx_num;

	/* The index data */
	u32 idx_num;
	u32 *idx;

	/* The attributes of the new object */
	u32 attrib_num;
	struct wt_object_c_attrib attribs[WT_OBJECT_ATTRIB_LIM];

	/* The uniform buffers for the new object */
	u32 unibuf_num;
	struct wt_object_c_unibuf unibufs[WT_OBJECT_UNIFORM_LIM]; 
};


/*
 * Create a new object constructor and begin creating a new object.
 *
 * @name: The name of the new object
 * @vnum: The number of vertices
 * @inum: The number of indices
 * @idx: The index data
 *
 * Returns: A new constructor or NULL if an error occurred
 */
WT_API struct wt_object_c *wt_BeginObjectConstr(char *name,
		u32 vnum, u32 inum, u32 *idx);


/*
 * Finalize construction and get the finished object. This function will also
 * insert the object into the object table of the given context.
 *
 * @c: Pointer to the constructor
 * @ctx: Pointer to the context
 * @pos: The initial position of the object
 * @rot: The initial rotation of the object
 *
 * Returns: Either a pointer to the finished object or NULL if an error occurred
 */
WT_API struct wt_object *wt_EndObjectConstr(struct wt_object_c *c,
		struct wt_context *ctx, wt_vec3_t pos, wt_vec3_t rot);


/*
 * After the object constructor has been utilized and is not need anymore, this
 * function should be called to cleanup everything and free the allocated
 * memory. The objects that have been created using the constructor are
 * unaffected.
 *
 * @c: Pointer to the constructor
 */
WT_API void wt_ObjectConstrCleanup(struct wt_object_c *c);


/*
 * Set the texture of the object during construction.
 *
 * @c: Pointer to the object constructor
 * @tex: Pointer to the texture
 */
WT_API void wt_ObjectConstrTexture(struct wt_object_c *c, struct wt_texture *tex);


/*
 * Set the shader of the object during construction.
 *
 * @c: Pointer to the object constructor
 * @shd: Pointer to the shader
 */
WT_API void wt_ObjectConstrShader(struct wt_object_c *c, struct wt_shader *shd);


/*
 * Attach a new attribute to the object.
 *
 * Use like: wt_ObjectConstrAttrib(c, "v_pos", 3, GL_FLOAT, vtx);
 *
 * @c: Pointer to the object constructor
 * @name: The name of the new attribute(has to match the shader input)
 * @size: The size of the element in data type quantities (ie 3 * GL_FLOAT)
 * @type: The data type of an element
 * @data: The buffer containing the data
 */
WT_API void wt_ObjectConstrAttrib(struct wt_object_c *c, char *name, u8 size,
		GLenum type, void *data);


/*
 * Add a new uniform buffer to the object.
 *
 * Use like: wt_ObjectConstrUniform(c, "camera", sizeof(struct uniform_buffer));
 *
 * @c: Pointer to the object constructor
 * @name: The name of the uniform buffer
 * @size: The size of the uniform buffer in bytes
 */
WT_API void wt_ObjectConstrUniform(struct wt_object_c *c, char *name, u32 size);



#endif /* _WT_GRAPHIC_CONSTRUCTOR_H */
