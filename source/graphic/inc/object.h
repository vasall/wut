#ifndef _FH_GRAPHIC_OBJECT_H
#define _FH_GRAPHIC_OBJECT_H

struct fh_object;

#define FH_OBJECT_NAME_LIM			128
#define FH_OBJECT_ATTRIB_LIM			12
#define FH_OBJECT_UNIFORM_LIM		6
#define FH_OBJECT_ATTRIB_NAME_LIM	128
#define FH_OBJECT_UNIFORM_NAME_LIM	128

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/table.h"
#include "utility/inc/matrix.h"

#include "window/inc/window.h"

#include "widget/inc/view.h"


struct fh_object_uniform {
	/* The name of the uniform buffer */
	char name[FH_OBJECT_UNIFORM_NAME_LIM];

	/* The size of the uniform buffer in bytes */
	u32 size;

	/* The uniform buffer */
	void *data;

	u32 bao;

	s32 location;
};


struct fh_object {
	/* The name of the object */
	char name[FH_OBJECT_NAME_LIM];

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

	/* The uniform buffers for this object */
	u8 uniform_number;
	struct fh_object_uniform uniforms[FH_OBJECT_UNIFORM_LIM];

	/* Pointer to the context this object belongs to */
	struct fh_context *context;

	/* Pointer to the view, if object is attached */
	struct fh_view *view;

	fh_vec3_t position;
	fh_vec3_t rotation;
};


/*
 * Update a uniform buffer.
 *
 * @mdl: Pointer to the object
 * @name: The name of the uniform
 * @ptr: A pointer to copy the data from
 */
FH_API void fh_obj_set_uniform(struct fh_object *mdl, char *name, void *ptr);



/* The callback function to call when removing an entry from the objects table */
FH_API void fh_obj_rmv_fnc(u32 size, void *ptr);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Create and initialize a object table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_InitObjectTable(struct fh_context *ctx);


/*
 * Close a object table and free the allocated memory.
 * 
 * @ctx: Pointer to the context
 */
FH_API void fh_CloseObjectTable(struct fh_context *ctx);


/*
 * Remove a object from the context it belongs to.
 *
 * @mdl: Pointer to the object
 */
FH_API void fh_RemoveObject(struct fh_object *mdl);


/*
 * Retrieve a object from the object table of a context.
 *
 * @ctx: Pointer to the context
 * @name: The name of the object
 *
 * Returns: A pointer to the object or NULL if either an error occurred or the
 * 	    object could not be found
 */
FH_API struct fh_object *fh_GetObject(struct fh_context *ctx, char *name);


/*
 * Update a uniform buffer of a object.
 *
 * @mdl: Pointer to the object
 * @name: The name of the uniform buffer
 * @ptr: A pointer with the new data
 */
FH_API void fh_SetObjectUniform(struct fh_object *mdl, char *name, void *ptr);


/*
 * Render a object.
 * 
 * @mdl: Pointer to the object
 * @[shd]: The shader to be used
 * @[tex]: The texture to be used
 */
FH_API void fh_RenderObject(struct fh_object *mdl, struct fh_shader *shd,
		struct fh_texture *tex);


#endif /* _FH_GRAPHIC_OBJECT_H */
