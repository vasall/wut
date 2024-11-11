#include "source/event/inc/event_listener.h"

#include "source/utility/inc/alarm.h"

#include "source/window/inc/window.h"

#include "source/document/inc/element.h"

#include "source/system/inc/system.h"

#include <stdlib.h>


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_XMOD struct wut_EventHandler *wut_hdl_create(void)
{
	struct wut_EventHandler *hdl;

	if(!(hdl = wut_malloc(sizeof(struct wut_EventHandler)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for event handler");
		return NULL;
	}

	hdl->number = 0;
	hdl->listener = NULL;

	return hdl;
}


WUT_XMOD void wut_hdl_destroy(struct wut_EventHandler *hdl)
{
	struct wut_EventListener *run;
	struct wut_EventListener *along;

	struct wut_EventListener *with;
	struct wut_EventListener *me;

	if(!hdl)
		return;

	run = hdl->listener;
	while(run) {
		along = run->next;

		with = along;
		while(with) {
			me = with->below;
			
			wut_free(with);	

			with = me;
		}

		run = along;
	}

	wut_free(hdl);
}


WUT_XMOD void wut_hdl_add(struct wut_EventHandler *hdl, 
		struct wut_EventListener *lst)
{
	struct wut_EventListener *run;
	struct wut_EventListener *prev = NULL;

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


WUT_XMOD void wut_hdl_remove(struct wut_EventHandler *hdl,
		enum wut_eEventType type, char *name)
{
	struct wut_EventListener *prev_column = NULL;
	struct wut_EventListener *run;
	struct wut_EventListener *prev_entry = NULL;
	struct wut_EventListener *run_down;


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


WUT_XMOD s8 wut_hdl_rundow(struct wut_EventHandler *hdl,
		struct wut_Event *evt)
{
	struct wut_EventListener *run;

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


WUT_XMOD struct wut_EventHandler *wut_hdl_retrieve(void *ptr)
{
	if(!ptr)
		return NULL;


	switch(*(u8 *)ptr) {
		case WUT_IDT_WINDOW:
			return ((struct wut_Window *)ptr)->event_handler;

		case WUT_IDT_ELEMENT:
			return ((struct wut_Element *)ptr)->event_handler;
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


WUT_API s8 wut_BindEventListener(struct wut_EventHandler *hdl,
		enum wut_eEventType type, char *name, wut_EventFunc fnc,
		void *data)
{
	struct wut_EventListener *lst;

	if(!hdl) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return -1;
	}

	if(!(lst = wut_malloc(sizeof(struct wut_EventListener)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for listener");
		return -1;
	}

	strcpy(lst->name, name);
	lst->type = type;
	lst->fnc = fnc;
	lst->data = data;

	lst->next = NULL;
	lst->below = NULL;

	wut_hdl_add(hdl, lst);

	return 0;
}


WUT_API void wut_UnbindEventListener(struct wut_EventHandler *hdl,
		enum wut_eEventType type, char *name)
{
	wut_hdl_remove(hdl, type, name);
}
