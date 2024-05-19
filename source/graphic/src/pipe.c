#include "graphic/inc/pipe.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"


WT_INTERN s32 pip_get_slot(struct wt_pipe *pip)
{
	s32 i;

	for(i = 0; i < pip->alloc; i++) {
		if(pip->entries[i].flags == 0)
			return i;
	}

	return -1;
}


/*
 * Calculate the distance from the camera for all entries in the pipe.
 *
 * @pip: Pointer to the pipe
 */
WT_INTERN void pip_calculate(struct wt_pipe *pip)
{
	u8 i;
	struct wt_pipe_entry *ent;
	struct wt_object *obj;
	wt_vec3_t del;

	for(i = 0; i < pip->number; i++) {
		ent = &pip->entries[i];
		obj = ent->object;

		wt_vec3_sub(obj->position, pip->ref_point, del);
		
		ent->crit = wt_vec2_len(del);
	}
}


/*
 * Calculate the distance from the camera for given slots.
 *
 * @pip: Pointer to the pipe
 * @num: The number of objects to update
 * @slt: The slots of the objects in the pipe
 */
WT_INTERN void pip_calculate_select(struct wt_pipe *pip, u8 num, s32 *slt)
{
	u8 i;
	struct wt_pipe_entry *ent;
	struct wt_object *obj;
	wt_vec3_t del;

	for(i = 0; i < num; i++) {
		ent = &pip->entries[slt[i]];
		obj = ent->object;

		wt_vec3_sub(obj->position, pip->ref_point, del);
		
		ent->crit = wt_vec2_len(del);
	}
}


/*
 * Sort all elements in the pipe.
 *
 * @pip: Pointer to the pipe
 */
WT_INTERN void pip_order(struct wt_pipe *pip)
{
	s32 i;
	s32 j;
	struct wt_pipe_entry *ent;
	struct wt_pipe_entry *run;

	if(pip->number < 1)
		return;

	for(i = 0; i < pip->number; i++) {
		ent = &pip->entries[i];

		ent->previous = -1;
		ent->next = -1;

		ent->flags &= ~WT_PIPE_F_HANDELD;
	}


	pip->start = 0;
	pip->entries[i].flags |= WT_PIPE_F_HANDELD;


	for(i = 1; i < pip->number; i++) {
		ent = &pip->entries[i];

		j = pip->start;
		while(1) {
			run = &pip->entries[j];
			
			if(run->crit < ent->crit) {
				if(ent->previous == -1) {
					pip->start = j;
					ent->next = j;
					run->previous = i;
					break;
				}
				else {
					ent->previous = run->previous;
					ent->next = j;
					run->previous = i;
					pip->entries[ent->previous].next = i;
					break;
				}
			}

			if(run->next == -1) {
				ent->previous = j;
				pip->entries[j].next = i;
			}

			j = run->next;
		}
	}
}


/*
 * Integrate a new entry into the pipe. This requires, that the criteria has
 * already been calculated for the new entry.
 *
 * @pip: Pointer to the pipe
 * @slot: The slot of the new entry
 */
WT_INTERN void pip_order_single(struct wt_pipe *pip, u8 slot)
{
	u32 c;
	u32 i;
	struct wt_pipe_entry *ent;
	struct wt_pipe_entry *run;

	ent = &pip->entries[slot];

	if(pip->number < 1) {
		pip->start = slot;

		ent->previous = -1;
		ent->next = -1;

		return;
	}

	printf("Begin\n");

	c = 0;
	i = pip->start;
	while(1 && (c++ < 1000)) {
		run = &pip->entries[i];

		if(ent->crit > run->crit) {
			if(run->previous == -1) {
				pip->start = slot;

				ent->next = i;
				run->previous = slot;

				return;
			}

			ent->previous = run->previous;
			ent->next = i;
			run->previous = slot;
			pip->entries[ent->previous].next = slot;

			return;
		}

		if(run->next == -1) {
			ent->previous = i;
			run->next = slot;

			return;
		}


		i = run->next;
	}
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_API struct wt_pipe *wt_CreatePipe(wt_vec3_t ref)
{
	struct wt_pipe *pip;
	u32 size;
	s32 i;

	if(!(pip = wt_malloc(sizeof(struct wt_pipe)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for pipe");
		goto err_return;
	}

	/* Set the attributes */
	pip->number = 0;
	pip->alloc = WT_PIPE_MIN;
	pip->start = -1;
	wt_vec3_cpy(pip->ref_point, ref);

	/* Preallocate slots for the objects */
	size = sizeof(struct wt_pipe_entry) * pip->alloc;
	if(!(pip->entries = wt_malloc(size))) {
		WT_ALARM(WT_ERROR, "Failed to preallocate memory for entries");
		goto err_free_pip;
	}

	/* And clear the flags for the entries */
	for(i = 0; i < pip->alloc; i++) {
		pip->entries[i].flags = 0;
		pip->entries[i].previous = -1;
		pip->entries[i].next = -1;
	}

	return pip;

err_free_pip:
	wt_free(pip);

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new pipe");
	return NULL;
}


WT_API void wt_DestroyPipe(struct wt_pipe *pip)
{
	if(!pip) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_free(pip->entries);
	wt_free(pip);
}


WT_API s8 wt_PipeAddObject(struct wt_pipe *pip, struct wt_object *obj)
{
	s32 slot;
	struct wt_pipe_entry *ent;

	if(!pip || !obj) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if((slot = pip_get_slot(pip)) < 0) {
		WT_ALARM(WT_ERROR, "No more free slots");
		goto err_return;
	}


	ent = &pip->entries[slot];

	ent->flags = WT_PIPE_F_USED;
	ent->object = obj;

	pip_calculate_select(pip, 1, &slot);

	pip_order_single(pip, slot);

	pip->number++;

	return 0;

err_return:
	WT_ALARM(WT_ERROR, "Failed to insert object into pipe");
	return -1;
}


WT_API void wt_PipeRemoveObject(struct wt_pipe *pip, s32 slot)
{
	struct wt_pipe_entry *ent;
	struct wt_pipe_entry *hdl;

	if(!pip || slot < 0) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	ent = &pip->entries[slot];


	if(ent->next != -1) {
		hdl = &pip->entries[ent->next];
		hdl->previous = ent->previous;
	}

	if(ent->previous != -1) {
		hdl = &pip->entries[ent->previous];
		hdl->next = ent->next;
	}
	else {
		pip->start = ent->next;
	}

	ent->flags = 0;

	pip->number--;

}


WT_API s32 wt_PipeGetSlot(struct wt_pipe *pip, char *name)
{
	s32 i;
	struct wt_pipe_entry *run;

	if(!pip || !name) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(pip->number < 1) {
		WT_ALARM(WT_ERROR, "The pipe is empty");
		return -1;
	}

	i = pip->start;
	while(1) {
		run = &pip->entries[i];

		if(strcmp(run->object->name, name) == 0)
			return i;

		i = run->next;
	}
	
	WT_ALARM(WT_ERROR, "Object could not be found in the pipe");
	return -1;
}


WT_API void wt_PipeSetReference(struct wt_pipe *pip, wt_vec3_t ref)
{
	if(!pip) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_vec3_cpy(pip->ref_point, ref);

	pip_calculate(pip);
	pip_order(pip);
}


WT_API void wt_PipeApply(struct wt_pipe *pip, void (*fnc)(struct wt_object *m))
{
	s32 itr;
	struct wt_pipe_entry *ent;

	if(!pip || !fnc) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	itr = pip->start;
	while(itr != -1) {
		ent = &pip->entries[itr];

		wt_RenderObject(ent->object, NULL, NULL);

		itr = ent->next;
	}
}


WT_API void wt_PipeShow(struct wt_pipe *pip)
{
	s32 itr;
	s32 c;

	if(!pip)
		return;

	printf("Pipe [%d element(s)]:\n", pip->number);

	itr = pip->start;
	c = 0;
	while(itr != -1) {
		printf("%d: %s (%d)", c, pip->entries[itr].object->name, itr);
		itr = pip->entries[itr].next;
		c++;
	}

}
