#ifndef _WT_GRAPHIC_PIPE_H
#define _WT_GRAPHIC_PIPE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/object.h"
#include "graphic/inc/camera.h"

#include "utility/inc/extended_math.h"

#define WT_PIPE_MIN		32

#define WT_PIPE_F_USED		(1<<0)

#define WT_PIPE_F_HANDELD	(1<<7)


struct wt_pipe {
	s32 number;
	s32 alloc;

	struct wt_pipe_entry *entries;
	s32 start;
	wt_vec3_t ref_point;
};

struct wt_pipe_entry {
	/* Flags indicating certain states of the entry  */
	u8 			flags;

	/* A pointer to the underlying object */
	struct wt_object 	*object;

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
WT_API struct wt_pipe *wt_CreatePipe(wt_vec3_t ref);


/*
 * Destroy a rendering pipe and free the allocated memory.
 */
WT_API void wt_DestroyPipe(struct wt_pipe *pip);


/*
 * Push a object into the rendering pipe.
 *
 * @pip: Pointer to the pipe
 * @obj: Pointer to the object
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WT_API s8 wt_PipeAddObject(struct wt_pipe *pip, struct wt_object *object);


/*
 * Remove a object from the rendering pipe.
 *
 * @pip: Pointer to the pipe
 * @slot: The slot of the object
 */
WT_API void wt_PipeRemoveObject(struct wt_pipe *pip, s32 slot);


/*
 * Get the slot of a object in the pipe.
 *
 * @pip: Pointer to the pipe
 * @name: The name of the object to search for
 */
WT_API s32 wt_PipeGetSlot(struct wt_pipe *pip, char *name);


/*
 * Update the reference point for the pipe and by doing so reorder all entries.
 *
 * @pip: Pointer to the pipe
 * @ref: The new reference point
 */
WT_API void wt_PipeSetReference(struct wt_pipe *pip, wt_vec3_t ref);


/*
 * Apply a higher level function to all object entries in the pipe, in the sorted
 * order.
 *
 * @pip: Pointer to the pipe
 * @fnc: The callback function to call on every object
 */
WT_API void wt_PipeApply(struct wt_pipe *pip, void (*fnc)(struct wt_object *o));


/*
 * Show the content of the pipe in the console.
 *
 * @pip: Pointer to the pipe
 */
WT_API void wt_PipeShow(struct wt_pipe *pip);

#endif /* _WT_GRAPHIC_PIPE_H */
