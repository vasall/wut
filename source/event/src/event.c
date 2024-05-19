#include "event/inc/event.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include "core/inc/core.h"

#include "event/inc/event_default.h"

#include "document/inc/document.h"



WT_INTERN s8 evt_is_mouse(u32 type)
{
	if(type == SDL_MOUSEMOTION) 	return 1;
	if(type == SDL_MOUSEBUTTONDOWN)	return 1;
	if(type == SDL_MOUSEBUTTONUP)	return 1;
	if(type == SDL_MOUSEWHEEL) 	return 1;
	return 0;
}


WT_INTERN void evt_get_position(u32 t, SDL_Event *raw, struct wt_sin2 *p)
{
	switch(t) {
		case SDL_MOUSEMOTION:
			p->x = raw->motion.x;
			p->y = raw->motion.y;
			return;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			p->x = raw->button.x;
			p->y = raw->button.y;
			return;

		case SDL_MOUSEWHEEL:
			p->x = raw->wheel.x;
			p->y = raw->wheel.y;
			return;

		default:
			p->x = 0;
			p->y = 0;
	}
}


WT_INTERN void evt_collect_info(SDL_Event *raw, struct wt_event_context *ctx)
{
	u32 type = raw->type;
	struct wt_window *win;
	struct wt_element *ele = NULL;
	struct wt_sin2 pos;

	/*
	 * First get the window.
	 *
	 * By default the active window should be used. But in certain cases
	 * like if the user closes the program, the main window will be
	 * selected.
	 */
	if(type == SDL_QUIT) {
		win = wt_core_get_main_window();
	}
	else {
		win = wt_GetWindow(raw->window.windowID);
	}

	/*
	 * Second get the element.
	 *
	 * Depending on the event type, a different element will be selected. In
	 * case of a mouse input, the element the mouse is hovering over will be
	 * selected. In case of a keyboard input, the selected element will the
	 * used.
	 */
	if(evt_is_mouse(type)) {

		switch(type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				ele = win->hovered;
				break;

			default:
				evt_get_position(type, raw, &pos);
				ele = wt_GetHoveredElement(win->document, &pos); 
		}
	}
	else if(win) {
		ele = win->selected;
	}
	else {
	}
	

	/*
	 * Attach the info to the context.
	 */
	ctx->window = win;
	ctx->element = ele;
}


WT_INTERN void evt_translate_type(struct wt_event *evt)
{
	switch(evt->raw.type) {
		case SDL_WINDOWEVENT_CLOSE:
			evt->type = WT_EVT_WINDOWCLOSE;
			return;

		case SDL_WINDOWEVENT_ENTER:
			evt->type = WT_EVT_WINDOWENTER;
			return;

		case SDL_WINDOWEVENT_LEAVE:
			evt->type = WT_EVT_WINDOWLEAVE;
			return;

		case SDL_WINDOWEVENT_SHOWN:
			evt->type = WT_EVT_WINDOWSHOWN;
			return;

		case SDL_WINDOWEVENT_HIDDEN:
			evt->type = WT_EVT_WINDOWHIDDEN;
			return;

		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			evt->type = WT_EVT_WINDOWRESIZED;
			return;

		case SDL_MOUSEMOTION:
			evt->type = WT_EVT_MOUSEMOTION;
			return;

		case SDL_MOUSEBUTTONDOWN:
			evt->type = WT_EVT_MOUSEBUTTONDOWN;
			return;

		case SDL_MOUSEBUTTONUP:
			evt->type = WT_EVT_MOUSEBUTTONUP;
			return;

		case SDL_MOUSEWHEEL:
			evt->type = WT_EVT_MOUSEWHEEL;
			return;

		case SDL_KEYDOWN:
			evt->type = WT_EVT_KEYDOWN;
			return;

		case SDL_KEYUP:
			evt->type = WT_EVT_KEYUP;
			return;

		default:
			printf("Event type unknown (%d)\n", evt->raw.type);
			return;
	}
}


WT_INTERN struct wt_event_handler *evt_find_handler(struct wt_event *evt)
{
	if(evt->context.element) return evt->context.element->event_handler;
	if(evt->context.window) return evt->context.window->event_handler;
	return NULL;
}


WT_INTERN s8 evt_raise_context(struct wt_event *evt)
{
	if(!evt->context.element)
		return 0;

	if(!evt->context.element->parent)
		return 0;

	evt->context.element = evt->context.element->parent;
	return 1;
}


WT_INTERN s8 evt_rundow(struct wt_event_handler *hdl, struct wt_event *evt)
{
	s8 ret = 0;

	/*
	 * Check if one of the custom event handler uses the event.
	 */
	if(wt_handler_rundow(hdl, evt))
		ret = 1;

	/*
	 * If the user activated the PREVENT_DEFAULT flag, the code will skip
	 * the next part.
	 */
	if(evt->flags & WT_EVT_F_PDEF)
		return ret;

	/*
	 *  Apply the default handler onto the given event.
	 */
	if(wt_evtdef_rundown(evt))
		ret = 1;

	return ret;
}



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_XMOD void wt_event_update(void)
{
	struct wt_event evt;

	wt_zeros(&evt, sizeof(struct wt_event));

	while(SDL_PollEvent(&evt.raw)) {
		if(evt.raw.type == SDL_QUIT) {
			wt_core_quit();
			return;
		}
	
		/* 
		 * First collect the relative context for this event like the
		 * related window and element.
		 */
		evt_collect_info(&evt.raw, &evt.context);

		/*
		 * Second translate the SDL-event-type to a FH-event-type.
		 */
		evt_translate_type(&evt);

		/*
		 * Then trigger the enhanced event internally so it can be
		 * handled.
		 */
		wt_event_trigger(&evt);
	}

	wt_zeros(&evt, sizeof(struct wt_event));
}


WT_XMOD void wt_event_trigger(struct wt_event *evt)
{
	struct wt_event_handler *hdl;

	/*
	 * First get the initial handler.
	 */
	if(!(hdl = evt_find_handler(evt)))
		return;

	/*
	 * Then try handling the event.
	 */
	while(!evt_rundow(hdl, evt)) {
		/*
		 * If no element responds, raise to the next level.
		 */
		if(!evt_raise_context(evt))
			return;

		/*
		 * Then get the current handler again.
		 */
		if(!(hdl = evt_find_handler(evt)))
			return;
	}
}


WT_XMOD void wt_event_trigger_raw(enum wt_event_type type,
		struct wt_window *win, struct wt_element *ele)
{
	struct wt_event evt;

	evt.type = type;
	evt.context.window = win;
	evt.context.element = ele;
	evt.flags = 0;

	wt_event_trigger(&evt);
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WT_API void wt_PreventDefault(struct wt_event *evt)
{
	if(!evt) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	evt->flags |= WT_EVT_F_PDEF; 	
}
