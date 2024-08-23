#include "graphic/inc/pipe.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"


WUT_INTERN s32 pip_get_slot(struct wut_Pipe *pip)
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
WUT_INTERN void pip_calculate(struct wut_Pipe *pip)
{
	u8 i;
	struct wut_PipeEntry *ent;
	struct wut_Object *obj;
	wut_Vec3 del;

	for(i = 0; i < pip->number; i++) {
		ent = &pip->entries[i];
		obj = ent->object;

		wut_vec3_sub(obj->position, pip->ref_point, del);
		
		ent->crit = wut_vec2_len(del);
	}
}


/*
 * Calculate the distance from the camera for given slots.
 *
 * @pip: Pointer to the pipe
 * @num: The number of objects to update
 * @slt: The slots of the objects in the pipe
 */
WUT_INTERN void pip_calculate_select(struct wut_Pipe *pip, u8 num, s32 *slt)
{
	u8 i;
	struct wut_PipeEntry *ent;
	struct wut_Object *obj;
	wut_Vec3 del;

	for(i = 0; i < num; i++) {
		ent = &pip->entries[slt[i]];
		obj = ent->object;

		wut_vec3_sub(obj->position, pip->ref_point, del);
		
		ent->crit = wut_vec2_len(del);
	}
}


/*
 * Sort all elements in the pipe.
 *
 * @pip: Pointer to the pipe
 */
WUT_INTERN void pip_order(struct wut_Pipe *pip)
{
	s32 i;
	s32 j;
	struct wut_PipeEntry *ent;
	struct wut_PipeEntry *run;

	if(pip->number < 1)
		return;

	for(i = 0; i < pip->number; i++) {
		ent = &pip->entries[i];

		ent->previous = -1;
		ent->next = -1;

		ent->flags &= ~WUT_PIPE_F_HANDELD;
	}


	pip->start = 0;
	pip->entries[i].flags |= WUT_PIPE_F_HANDELD;


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
WUT_INTERN void pip_order_single(struct wut_Pipe *pip, u8 slot)
{
	u32 c;
	u32 i;
	struct wut_PipeEntry *ent;
	struct wut_PipeEntry *run;

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

WUT_XMOD struct wut_Pipe *wut_pip_create(wut_Vec3 ref)
{
	struct wut_Pipe *pip;
	u32 size;
	s32 i;

	if(!(pip = wut_malloc(sizeof(struct wut_Pipe)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for pipe");
		goto err_return;
	}

	/* Set the attributes */
	pip->number = 0;
	pip->alloc = WUT_PIPE_MIN;
	pip->start = -1;
	wut_vec3_cpy(pip->ref_point, ref);

	/* Preallocate slots for the objects */
	size = sizeof(struct wut_PipeEntry) * pip->alloc;
	if(!(pip->entries = wut_malloc(size))) {
		WUT_ALARM(WUT_ERROR, "Failed to preallocate memory for entries");
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
	wut_free(pip);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new pipe");
	return NULL;
}


WUT_XMOD void wut_pip_destroy(struct wut_Pipe *pip)
{
	if(!pip) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_free(pip->entries);
	wut_free(pip);
}


WUT_XMOD s8 wut_pip_add(struct wut_Pipe *pip, struct wut_Object *obj)
{
	s32 slot;
	struct wut_PipeEntry *ent;

	if(!pip || !obj) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if((slot = pip_get_slot(pip)) < 0) {
		WUT_ALARM(WUT_ERROR, "No more free slots");
		goto err_return;
	}


	ent = &pip->entries[slot];

	ent->flags = WUT_PIPE_F_USED;
	ent->object = obj;

	pip_calculate_select(pip, 1, &slot);

	pip_order_single(pip, slot);

	pip->number++;

	return 0;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to insert object into pipe");
	return -1;
}


WUT_XMOD void wut_pip_remove(struct wut_Pipe *pip, s32 slot)
{
	struct wut_PipeEntry *ent;
	struct wut_PipeEntry *hdl;

	if(!pip || slot < 0) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
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


WUT_XMOD s32 wut_pip_get(struct wut_Pipe *pip, char *name)
{
	s32 i;
	struct wut_PipeEntry *run;

	if(!pip || !name) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(pip->number < 1) {
		WUT_ALARM(WUT_ERROR, "The pipe is empty");
		return -1;
	}

	i = pip->start;
	while(1) {
		run = &pip->entries[i];

		if(strcmp(run->object->name, name) == 0)
			return i;

		i = run->next;
	}
	
	WUT_ALARM(WUT_ERROR, "Object could not be found in the pipe");
	return -1;
}


WUT_XMOD void wut_pip_set_reference(struct wut_Pipe *pip, wut_Vec3 ref)
{
	if(!pip) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_vec3_cpy(pip->ref_point, ref);

	pip_calculate(pip);
	pip_order(pip);
}


WUT_XMOD void wut_pip_apply(struct wut_Pipe *pip, void (*fnc)(struct wut_Object *m))
{
	s32 itr;
	struct wut_PipeEntry *ent;

	if(!pip || !fnc) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	itr = pip->start;
	while(itr != -1) {
		ent = &pip->entries[itr];

		wut_RenderObject(ent->object, NULL, NULL);

		itr = ent->next;
	}
}


WUT_XMOD void wut_pip_dump(struct wut_Pipe *pip)
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
