#include "event/inc/event_listener.h"

#include "utility/alarm/inc/alarm.h"

#include "window/inc/window.h"

#include "document/inc/element.h"

#include "system/inc/system.h"

#include <stdlib.h>


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_XMOD struct fh_event_handler *fh_handler_create(void)
{
	struct fh_event_handler *hdl;

	if(!(hdl = fh_malloc(sizeof(struct fh_event_handler)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for event handler");
		return NULL;
	}

	hdl->number = 0;
	hdl->listener = NULL;

	return hdl;
}


FH_XMOD void fh_handler_destroy(struct fh_event_handler *hdl)
{
	struct fh_event_listener *run;
	struct fh_event_listener *along;

	struct fh_event_listener *with;
	struct fh_event_listener *me;

	if(!hdl)
		return;

	run = hdl->listener;
	while(run) {
		along = run->next;

		with = along;
		while(with) {
			me = with->below;
			
			fh_free(with);	

			with = me;
		}

		run = along;
	}

	fh_free(hdl);
}


FH_XMOD void fh_handler_add(struct fh_event_handler *hdl, 
		struct fh_event_listener *lst)
{
	struct fh_event_listener *run;
	struct fh_event_listener *prev = NULL;

	/*
	 * If the table is empty just add it to the beginning.
	 */
	if(hdl->number < 1) {
		hdl->listener = lst;
		hdl->number++;
		return;
	}

	/*
	 * Otherwise check if there're already listeners of the same type, and
	 * if yes, just add it to the bottom of the list.
	 */
	run = hdl->listener;	
	while(run) {
	
		if(run->type == lst->type) {
			while(run->below)
				run = run->below;

			run->below = lst;
			hdl->number++;
			return;
		}

		prev = run;
		run = run->next;
	}

	/*
	 * If there are no similar ones, just add it as it's own new column.
	 */
	prev->next = lst;
	hdl->number++;
}


FH_XMOD void fh_handler_remove(struct fh_event_handler *hdl,
		enum fh_event_type type, char *name)
{
	struct fh_event_listener *prev_column = NULL;
	struct fh_event_listener *run;
	struct fh_event_listener *prev_entry = NULL;
	struct fh_event_listener *run_down;


	run =  hdl->listener;
	while(run) {
		if(run->type == type) {
			run_down = run;
			while(run_down) {
				if(strcmp(run_down->name, name) == 0) {
					if(prev_entry) {
						prev_entry->below =
							run_down->below;
						hdl->number--;
						return;
					}

					prev_column->next = run_down;
					run_down->next = run->next;
					hdl->number--;
					return;
				}

				prev_entry = run_down;
				run_down = run_down->below;
			}
		}

		prev_column = run;
		run = run->next;
	}
}


FH_XMOD s8 fh_handler_rundow(struct fh_event_handler *hdl,
		struct fh_event *evt)
{
	struct fh_event_listener *run;

	run = hdl->listener;
	while(run) {
		if(run->type == evt->type) {
			while(run) {
				if(run->fnc(evt, run->data))
					return 1;

				run = run->below;
			}
		}

		run = run->next;
	}

	return 0;
}


FH_XMOD struct fh_event_handler *fh_handler_retrieve(void *ptr)
{
	if(!ptr)
		return NULL;


	switch(*(u8 *)ptr) {
		case FH_IDT_WINDOW:
			return ((struct fh_window *)ptr)->event_handler;

		case FH_IDT_ELEMENT:
			return ((struct fh_element *)ptr)->event_handler;
	}

	return NULL;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API s8 fh_BindEventListener(struct fh_event_handler *hdl,
		enum fh_event_type type, char *name, fh_evt_cfnc fnc,
		void *data)
{
	struct fh_event_listener *lst;

	if(!(lst = fh_malloc(sizeof(struct fh_event_listener)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for listener");
		return -1;
	}

	strcpy(lst->name, name);
	lst->type = type;
	lst->fnc = fnc;
	lst->data = data;

	lst->next = NULL;
	lst->below = NULL;

	fh_handler_add(hdl, lst);

	return 0;
}


FH_API void fh_UnbindEventListener(struct fh_event_handler *hdl,
		enum fh_event_type type, char *name)
{
	fh_handler_remove(hdl, type, name);
}
