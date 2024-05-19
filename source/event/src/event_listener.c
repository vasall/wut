#include "event/inc/event_listener.h"

#include "utility/inc/alarm.h"

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


WT_XMOD struct wt_event_handler *wt_handler_create(void)
{
	struct wt_event_handler *hdl;

	if(!(hdl = wt_malloc(sizeof(struct wt_event_handler)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for event handler");
		return NULL;
	}

	hdl->number = 0;
	hdl->listener = NULL;

	return hdl;
}


WT_XMOD void wt_handler_destroy(struct wt_event_handler *hdl)
{
	struct wt_event_listener *run;
	struct wt_event_listener *along;

	struct wt_event_listener *with;
	struct wt_event_listener *me;

	if(!hdl)
		return;

	run = hdl->listener;
	while(run) {
		along = run->next;

		with = along;
		while(with) {
			me = with->below;
			
			wt_free(with);	

			with = me;
		}

		run = along;
	}

	wt_free(hdl);
}


WT_XMOD void wt_handler_add(struct wt_event_handler *hdl, 
		struct wt_event_listener *lst)
{
	struct wt_event_listener *run;
	struct wt_event_listener *prev = NULL;

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


WT_XMOD void wt_handler_remove(struct wt_event_handler *hdl,
		enum wt_event_type type, char *name)
{
	struct wt_event_listener *prev_column = NULL;
	struct wt_event_listener *run;
	struct wt_event_listener *prev_entry = NULL;
	struct wt_event_listener *run_down;


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


WT_XMOD s8 wt_handler_rundow(struct wt_event_handler *hdl,
		struct wt_event *evt)
{
	struct wt_event_listener *run;

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


WT_XMOD struct wt_event_handler *wt_handler_retrieve(void *ptr)
{
	if(!ptr)
		return NULL;


	switch(*(u8 *)ptr) {
		case WT_IDT_WINDOW:
			return ((struct wt_window *)ptr)->event_handler;

		case WT_IDT_ELEMENT:
			return ((struct wt_element *)ptr)->event_handler;
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


WT_API s8 wt_BindEventListener(struct wt_event_handler *hdl,
		enum wt_event_type type, char *name, wt_evt_cfnc fnc,
		void *data)
{
	struct wt_event_listener *lst;

	if(!hdl) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(!(lst = wt_malloc(sizeof(struct wt_event_listener)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for listener");
		return -1;
	}

	strcpy(lst->name, name);
	lst->type = type;
	lst->fnc = fnc;
	lst->data = data;

	lst->next = NULL;
	lst->below = NULL;

	wt_handler_add(hdl, lst);

	return 0;
}


WT_API void wt_UnbindEventListener(struct wt_event_handler *hdl,
		enum wt_event_type type, char *name)
{
	wt_handler_remove(hdl, type, name);
}
