#include "graphic/inc/pipe.h"

#include "system/inc/system.h"


FH_INTERN s32 pip_get_slot(struct fh_pipe *pip)
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
FH_INTERN void pip_calculate(struct fh_pipe *pip)
{
	u8 i;
	struct fh_pipe_entry *ent;
	struct fh_model *mdl;
	vec3_t del;

	for(i = 0; i < pip->number; i++) {
		ent = &pip->entries[i];
		mdl = ent->model;

		vec3_sub(mdl->position, pip->ref_point, del);
		
		ent->crit = vec2_len(del);
	}
}


/*
 * Calculate the distance from the camera for given slots.
 *
 * @pip: Pointer to the pipe
 * @num: The number of models to update
 * @slt: The slots of the models in the pipe
 */
FH_INTERN void pip_calculate_select(struct fh_pipe *pip, u8 num, s32 *slt)
{
	u8 i;
	struct fh_pipe_entry *ent;
	struct fh_model *mdl;
	vec3_t del;

	for(i = 0; i < num; i++) {
		ent = &pip->entries[slt[i]];
		mdl = ent->model;

		vec3_sub(mdl->position, pip->ref_point, del);
		
		ent->crit = vec2_len(del);
	}
}


/*
 * Sort all elements in the pipe.
 *
 * @pip: Pointer to the pipe
 */
FH_INTERN void pip_order(struct fh_pipe *pip)
{
	s32 i;
	s32 j;
	struct fh_pipe_entry *ent;
	struct fh_pipe_entry *run;

	if(pip->number < 1)
		return;

	for(i = 0; i < pip->number; i++) {
		ent = &pip->entries[i];

		ent->previous = -1;
		ent->next = -1;

		ent->flags &= ~FH_PIPE_F_HANDELD;
	}


	pip->start = 0;
	pip->entries[i].flags |= FH_PIPE_F_HANDELD;


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
FH_INTERN void pip_order_single(struct fh_pipe *pip, u8 slot)
{
	u32 c;
	u32 i;
	struct fh_pipe_entry *ent;
	struct fh_pipe_entry *run;

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

FH_API struct fh_pipe *fh_CreatePipe(vec3_t ref)
{
	struct fh_pipe *pip;
	u32 size;
	s32 i;

	if(!(pip = fh_malloc(sizeof(struct fh_pipe)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for pipe");
		goto err_return;
	}

	/* Set the attributes */
	pip->number = 0;
	pip->alloc = FH_PIPE_MIN;
	pip->start = -1;
	vec3_cpy(pip->ref_point, ref);

	/* Preallocate slots for the models */
	size = sizeof(struct fh_pipe_entry) * pip->alloc;
	if(!(pip->entries = fh_malloc(size))) {
		ALARM(ALARM_ERR, "Failed to preallocate memory for entries");
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
	fh_free(pip);

err_return:
	ALARM(ALARM_ERR, "Failed to create new pipe");
	return NULL;
}


FH_API void fh_DestroyPipe(struct fh_pipe *pip)
{
	if(!pip) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_free(pip->entries);
	fh_free(pip);
}


FH_API s8 fh_PipeAddModel(struct fh_pipe *pip, struct fh_model *mdl)
{
	s32 slot;
	struct fh_pipe_entry *ent;

	if(!pip || !mdl) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if((slot = pip_get_slot(pip)) < 0) {
		ALARM(ALARM_ERR, "No more free slots");
		goto err_return;
	}


	ent = &pip->entries[slot];

	ent->flags = FH_PIPE_F_USED;
	ent->model = mdl;

	pip_calculate_select(pip, 1, &slot);

	pip_order_single(pip, slot);

	pip->number++;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert model into pipe");
	return -1;
}


FH_API void fh_PipeRemoveModel(struct fh_pipe *pip, s32 slot)
{
	struct fh_pipe_entry *ent;
	struct fh_pipe_entry *hdl;

	if(!pip || slot < 0) {
		ALARM(ALARM_WARN, "Input parameters invalid");
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


FH_API s32 fh_PipeGetSlot(struct fh_pipe *pip, char *name)
{
	s32 i;
	struct fh_pipe_entry *run;

	if(!pip || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return -1;
	}

	if(pip->number < 1) {
		ALARM(ALARM_ERR, "The pipe is empty");
		return -1;
	}

	i = pip->start;
	while(1) {
		run = &pip->entries[i];

		if(strcmp(run->model->name, name) == 0)
			return i;

		i = run->next;
	}
	
	ALARM(ALARM_ERR, "Model could not be found in the pipe");
	return -1;
}


FH_API void fh_PipeSetReference(struct fh_pipe *pip, vec3_t ref)
{
	if(!pip) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	vec3_cpy(pip->ref_point, ref);

	pip_calculate(pip);
	pip_order(pip);
}


FH_API void fh_PipeApply(struct fh_pipe *pip, void (*fnc)(struct fh_model *m))
{
	s32 itr;
	struct fh_pipe_entry *ent;

	if(!pip || !fnc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	itr = pip->start;
	while(itr != -1) {
		ent = &pip->entries[itr];

		fh_RenderModel(ent->model, NULL, NULL);

		itr = ent->next;
	}
}


FH_API void fh_PipeShow(struct fh_pipe *pip)
{
	s32 itr;
	s32 c;

	if(!pip)
		return;

	printf("Pipe [%d element(s)]:\n", pip->number);

	itr = pip->start;
	c = 0;
	while(itr != -1) {
		printf("%d: %s (%d)", c, pip->entries[itr].model->name, itr);
		itr = pip->entries[itr].next;
		c++;
	}

}
