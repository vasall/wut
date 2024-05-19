#ifndef _WT_GRAPHIC_OBJECT_H
#define _WT_GRAPHIC_OBJECT_H

struct wt_object;

#define WT_OBJECT_NAME_LIM			128
#define WT_OBJECT_ATTRIB_LIM			12
#define WT_OBJECT_UNIFORM_LIM		6
#define WT_OBJECT_ATTRIB_NAME_LIM	128
#define WT_OBJECT_UNIFORM_NAME_LIM	128

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/table.h"
#include "utility/inc/extended_math.h"

#include "window/inc/window.h"

#include "widget/inc/view.h"


struct wt_object_uniform {
	/* The name of the uniform buffer */
	char name[WT_OBJECT_UNIFORM_NAME_LIM];

	/* The size of the uniform buffer in bytes */
	u32 size;

	/* The uniform buffer */
	void *data;

	u32 bao;

	s32 location;
};


struct wt_object {
	/* The name of the object */
	char name[WT_OBJECT_NAME_LIM];

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
	struct wt_shader *shader;
	struct wt_texture *texture;

	/* The uniform buffers for this object */
	u8 uniform_number;
	struct wt_object_uniform uniforms[WT_OBJECT_UNIFORM_LIM];

	/* Pointer to the context this object belongs to */
	struct wt_context *context;

	/* Pointer to the view, if object is attached */
	struct wt_view *view;

	wt_vec3_t position;
	wt_vec3_t rotation;
};


/*
 * Update a uniform buffer.
 *
 * @mdl: Pointer to the object
 * @name: The name of the uniform
 * @ptr: A pointer to copy the data from
 */
WT_API void wt_obj_set_uniform(struct wt_object *mdl, char *name, void *ptr);



/* The callback function to call when removing an entry from the objects table */
WT_API void wt_obj_rmv_fnc(u32 size, void *ptr);

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
WT_API s8 wt_InitObjectTable(struct wt_context *ctx);


/*
 * Close a object table and free the allocated memory.
 * 
 * @ctx: Pointer to the context
 */
WT_API void wt_CloseObjectTable(struct wt_context *ctx);


/*
 * Remove a object from the context it belongs to.
 *
 * @mdl: Pointer to the object
 */
WT_API void wt_RemoveObject(struct wt_object *mdl);


/*
 * Retrieve a object from the object table of a context.
 *
 * @ctx: Pointer to the context
 * @name: The name of the object
 *
 * Returns: A pointer to the object or NULL if either an error occurred or the
 * 	    object could not be found
 */
WT_API struct wt_object *wt_GetObject(struct wt_context *ctx, char *name);


/*
 * Update a uniform buffer of a object.
 *
 * @mdl: Pointer to the object
 * @name: The name of the uniform buffer
 * @ptr: A pointer with the new data
 */
WT_API void wt_SetObjectUniform(struct wt_object *mdl, char *name, void *ptr);


/*
 * Render a object.
 * 
 * @mdl: Pointer to the object
 * @[shd]: The shader to be used
 * @[tex]: The texture to be used
 */
WT_API void wt_RenderObject(struct wt_object *mdl, struct wt_shader *shd,
		struct wt_texture *tex);


#endif /* _WT_GRAPHIC_OBJECT_H */
