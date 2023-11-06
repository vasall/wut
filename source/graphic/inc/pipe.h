#ifndef _FH_GRAPHIC_PIPE_H
#define _FH_GRAPHIC_PIPE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "graphic/inc/model.h"
#include "graphic/inc/camera.h"


#define FH_PIPE_MIN		32

#define FH_PIPE_F_USED		(1<<0)

#define FH_PIPE_F_HANDELD	(1<<7)


struct fh_pipe {
	s32 number;
	s32 alloc;

	struct fh_pipe_entry *entries;
	s32 start;

	vec3_t ref_point;
};

struct fh_pipe_entry {
	/* Flags indicating certain states of the entry  */
	u8 			flags;

	/* A pointer to the underlying model */
	struct fh_model 	*model;

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
FH_API struct fh_pipe *fh_CreatePipe(vec3_t ref);


/*
 * Destroy a rendering pipe and free the allocated memory.
 */
FH_API void fh_DestroyPipe(struct fh_pipe *pip);


/*
 * Push a model into the rendering pipe.
 *
 * @pip: Pointer to the pipe
 * @mdl: Pointer to the model
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_PipeAddModel(struct fh_pipe *pip, struct fh_model *mdl);


/*
 * Remove a model from the rendering pipe.
 *
 * @pip: Pointer to the pipe
 * @slot: The slot of the model
 */
FH_API void fh_PipeRemoveModel(struct fh_pipe *pip, s32 slot);


/*
 * Get the slot of a model in the pipe.
 *
 * @pip: Pointer to the pipe
 * @name: The name of the model to search for
 */
FH_API s32 fh_PipeGetSlot(struct fh_pipe *pip, char *name);


/*
 * Update the reference point for the pipe and by doing so reorder all entries.
 *
 * @pip: Pointer to the pipe
 * @ref: The new reference point
 */
FH_API void fh_PipeSetReference(struct fh_pipe *pip, vec3_t ref);


/*
 * Apply a higher level function to all model entries in the pipe, in the sorted
 * order.
 *
 * @pip: Pointer to the pipe
 * @fnc: The callback function to call on every model
 */
FH_API void fh_PipeApply(struct fh_pipe *pip, void (*fnc)(struct fh_model *m));


/*
 * Show the content of the pipe in the console.
 *
 * @pip: Pointer to the pipe
 */
FH_API void fh_PipeShow(struct fh_pipe *pip);

#endif /* _FH_GRAPHIC_PIPE_H */
