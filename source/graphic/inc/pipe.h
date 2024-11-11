#ifndef _WUT_GRAPHIC_PIPE_H
#define _WUT_GRAPHIC_PIPE_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/graphic/inc/object.h"
#include "source/graphic/inc/camera.h"

#include "source/math/inc/functions.h"
#include "source/math/inc/vector.h"

#define WUT_PIPE_MIN		32

#define WUT_PIPE_F_USED		(1<<0)

#define WUT_PIPE_F_HANDELD	(1<<7)


struct wut_Pipe {
	s32                     number;
	s32                     alloc;

	struct wut_PipeEntry    *entries;
	s32                     start;
	wut_Vec3                ref_point;
};

struct wut_PipeEntry {
	/* Flags indicating certain states of the entry  */
	u8 			flags;

	/* A pointer to the underlying object */
	struct wut_Object 	*object;

	/* The evaluation criteria */
	f32 			crit;

	/* The slot of the previous and next entry */
	s32 			previous;
	s32 			next;
};


/*
 * Create a new rendering pipe.
 *
 * @ref: The reference point
 *
 * Returns: Either a pointer to the pipe or NULL if an error occurred
 */
WUT_XMOD struct wut_Pipe *wut_pip_create(wut_Vec3 ref);


/*
 * Destroy a rendering pipe and free the allocated memory.
 */
WUT_XMOD void wut_pip_destroy(struct wut_Pipe *pip);


/*
 * Push a object into the rendering pipe.
 *
 * @pip: Pointer to the pipe
 * @obj: Pointer to the object
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_XMOD s8 wut_pip_add(struct wut_Pipe *pip, struct wut_Object *object);


/*
 * Remove a object from the rendering pipe.
 *
 * @pip: Pointer to the pipe
 * @slot: The slot of the object
 */
WUT_XMOD void wut_pip_remove(struct wut_Pipe *pip, s32 slot);


/*
 * Get the slot of a object in the pipe.
 *
 * @pip: Pointer to the pipe
 * @name: The name of the object to search for
 */
WUT_XMOD s32 wut_pip_get(struct wut_Pipe *pip, char *name);


/*
 * Update the reference point for the pipe and by doing so reorder all entries.
 *
 * @pip: Pointer to the pipe
 * @ref: The new reference point
 */
WUT_XMOD void wut_pip_set_reference(struct wut_Pipe *pip, wut_Vec3 ref);


/*
 * Apply a higher level function to all object entries in the pipe, in the sorted
 * order.
 *
 * @pip: Pointer to the pipe
 * @fnc: The callback function to call on every object
 */
WUT_XMOD void wut_pip_apply(struct wut_Pipe *pip, void (*fnc)(struct wut_Object *o));


/*
 * Show the content of the pipe in the console.
 *
 * @pip: Pointer to the pipe
 */
WUT_XMOD void wut_pip_dump(struct wut_Pipe *pip);

#endif /* _WUT_GRAPHIC_PIPE_H */
