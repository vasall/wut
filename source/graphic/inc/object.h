#ifndef _WUT_GRAPHIC_OBJECT_H
#define _WUT_GRAPHIC_OBJECT_H

struct wut_Object;

#define WUT_OBJECT_NAME_LIM			128
#define WUT_OBJECT_ATTRIB_LIM			12
#define WUT_OBJECT_UNIFORM_LIM		        6
#define WUT_OBJECT_ATTRIB_NAME_LIM	        128
#define WUT_OBJECT_UNIFORM_NAME_LIM	        128

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "utility/inc/table.h"

#include "math/inc/functions.h"
#include "math/inc/vector.h"

#include "window/inc/window.h"

#include "widget/inc/view.h"


struct wut_ObjectUniform {
	/* The name of the uniform buffer */
	char name[WUT_OBJECT_UNIFORM_NAME_LIM];

	/* The size of the uniform buffer in bytes */
	u32 size;

	/* The uniform buffer */
	void *data;

	u32 bao;

	s32 location;
};


struct wut_Object {
	/* The name of the object */
	char name[WUT_OBJECT_NAME_LIM];

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
	struct wut_Shader *shader;
	struct wut_Texture *texture;

	/* The uniform buffers for this object */
	u8 uniform_number;
	struct wut_ObjectUniform uniforms[WUT_OBJECT_UNIFORM_LIM];

	/* Pointer to the context this object belongs to */
	struct wut_Context *context;

	/* Pointer to the view, if object is attached */
	struct wut_View *view;

	wut_Vec3 position;
	wut_Vec3 rotation;
};


/*
 * Update a uniform buffer.
 *
 * @mdl: Pointer to the object
 * @name: The name of the uniform
 * @ptr: A pointer to copy the data from
 */
WUT_API void wut_obj_set_uniform(struct wut_Object *mdl, char *name, void *ptr);



/* The callback function to call when removing an entry from the objects table */
WUT_API void wut_obj_rmv_fnc(u32 size, void *ptr);

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
WUT_API s8 wut_InitObjectTable(struct wut_Context *ctx);


/*
 * Close a object table and free the allocated memory.
 * 
 * @ctx: Pointer to the context
 */
WUT_API void wut_CloseObjectTable(struct wut_Context *ctx);


/*
 * Remove a object from the context it belongs to.
 *
 * @mdl: Pointer to the object
 */
WUT_API void wut_RemoveObject(struct wut_Object *mdl);


/*
 * Retrieve a object from the object table of a context.
 *
 * @ctx: Pointer to the context
 * @name: The name of the object
 *
 * Returns: A pointer to the object or NULL if either an error occurred or the
 * 	    object could not be found
 */
WUT_API struct wut_Object *wut_GetObject(struct wut_Context *ctx, char *name);


/*
 * Update a uniform buffer of a object.
 *
 * @mdl: Pointer to the object
 * @name: The name of the uniform buffer
 * @ptr: A pointer with the new data
 */
WUT_API void wut_SetObjectUniform(struct wut_Object *mdl, char *name, void *ptr);


/*
 * Render a object.
 * 
 * @mdl: Pointer to the object
 * @[shd]: The shader to be used
 * @[tex]: The texture to be used
 */
WUT_API void wut_RenderObject(struct wut_Object *mdl, struct wut_Shader *shd,
		struct wut_Texture *tex);


#endif /* _WUT_GRAPHIC_OBJECT_H */
