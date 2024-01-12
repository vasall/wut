#include "event/inc/event.h"

#include "utility/alarm/inc/alarm.h"

#include "system/inc/system.h"

#include "core/inc/core.h"

#include "event/inc/event_default.h"

#include "document/inc/document.h"



FH_INTERN s8 evt_is_mouse(u32 type)
{
	if(type == SDL_MOUSEMOTION) 	return 1;
	if(type == SDL_MOUSEBUTTONDOWN)	return 1;
	if(type == SDL_MOUSEBUTTONUP)	return 1;
	if(type == SDL_MOUSEWHEEL) 	return 1;
	return 0;
}


FH_INTERN void evt_get_position(u32 t, SDL_Event *raw, struct fh_sin2 *p)
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


FH_INTERN void evt_collect_info(SDL_Event *raw, struct fh_event_context *ctx)
{
	u32 type = raw->type;
	struct fh_window *win;
	struct fh_element *ele = NULL;
	struct fh_sin2 pos;

	/*
	 * First get the window.
	 *
	 * By default the active window should be used. But in certain cases
	 * like if the user closes the program, the main window will be
	 * selected.
	 */
	if(type == SDL_QUIT) {
		win = fh_core_get_main_window();
	}
	else {
		win = fh_GetWindow(raw->window.windowID);
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
		printf("Event is mouse!!!\n");

		switch(type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				ele = win->hovered;
				break;

			default:
				evt_get_position(type, raw, &pos);
				ele = fh_GetHoveredElement(win->document, &pos); 
		}
		
		printf("Event is mouse (%p, %s)\n", ele, ele->name);
	}
	else if(win) {
		ele = win->selected;
		printf("Event is window\n");
	}
	else {
		printf("Event is nothing\n");
	}
	

	/*
	 * Attach the info to the context.
	 */
	ctx->window = win;
	ctx->element = ele;
}


FH_INTERN void evt_translate_type(struct fh_event *evt)
{
	switch(evt->raw.type) {
		case SDL_WINDOWEVENT_CLOSE:
			evt->type = FH_EVT_WINDOWCLOSE;
			return;

		case SDL_WINDOWEVENT_ENTER:
			evt->type = FH_EVT_WINDOWENTER;
			return;

		case SDL_WINDOWEVENT_LEAVE:
			evt->type = FH_EVT_WINDOWLEAVE;
			return;

		case SDL_WINDOWEVENT_SHOWN:
			evt->type = FH_EVT_WINDOWSHOWN;
			return;

		case SDL_WINDOWEVENT_HIDDEN:
			evt->type = FH_EVT_WINDOWHIDDEN;
			return;

		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			evt->type = FH_EVT_WINDOWRESIZED;
			return;

		case SDL_MOUSEMOTION:
			evt->type = FH_EVT_MOUSEMOTION;
			return;

		case SDL_MOUSEBUTTONDOWN:
			evt->type = FH_EVT_MOUSEBUTTONDOWN;
			return;

		case SDL_MOUSEBUTTONUP:
			evt->type = FH_EVT_MOUSEBUTTONUP;
			return;

		case SDL_MOUSEWHEEL:
			evt->type = FH_EVT_MOUSEWHEEL;
			return;

		case SDL_KEYDOWN:
			evt->type = FH_EVT_KEYDOWN;
			return;

		case SDL_KEYUP:
			evt->type = FH_EVT_KEYUP;
			return;

		default:
			printf("Event type unknown\n");
			return;
	}
}


FH_INTERN struct fh_event_handler *evt_find_handler(struct fh_event *evt)
{
	if(evt->context.element) return evt->context.element->event_handler;
	if(evt->context.window) return evt->context.window->event_handler;
	return NULL;
}


FH_INTERN s8 evt_raise_context(struct fh_event *evt)
{
	if(!evt->context.element)
		return 0;

	if(!evt->context.element->parent)
		return 0;

	evt->context.element = evt->context.element->parent;
	return 1;
}


FH_INTERN s8 evt_rundow(struct fh_event_handler *hdl, struct fh_event *evt)
{
	s8 ret = 0;

	/*
	 * Check if one of the custom event handler uses the event.
	 */
	if(fh_handler_rundow(hdl, evt))
		ret = 1;

	/*
	 * If the user activated the PREVENT_DEFAULT flag, the code will skip
	 * the next part.
	 */
	if(evt->flags & FH_EVT_F_PDEF)
		return ret;

	/*
	 *  Apply the default handler onto the given event.
	 */
	if(fh_evtdef_rundown(evt))
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


FH_XMOD void fh_event_update(void)
{
	struct fh_event evt;

	fh_zeros(&evt, sizeof(struct fh_event));

	printf("Update events\n");

	while(SDL_PollEvent(&evt.raw)) {
		printf(">> Poll new\n");

		if(evt.raw.type == SDL_QUIT) {
			fh_core_quit();
			return;
		}
		
		printf("Collect info\n");
		/* 
		 * First collect the relative context for this event like the
		 * related window and element.
		 */
		evt_collect_info(&evt.raw, &evt.context);

		printf("Translate type\n");
		/*
		 * Second translate the SDL-event-type to a FH-event-type.
		 */
		evt_translate_type(&evt);

		printf("Rundown\n");
		/*
		 * Then trigger the enhanced event internally so it can be
		 * handled.
		 */
		fh_event_trigger(&evt);
		printf("done\n");
		printf("--------------------------------------\n");

	}

	fh_zeros(&evt, sizeof(struct fh_event));
}


FH_XMOD void fh_event_trigger(struct fh_event *evt)
{
	struct fh_event_handler *hdl;

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


FH_XMOD void fh_event_trigger_raw(enum fh_event_type type,
		struct fh_window *win, struct fh_element *ele)
{
	struct fh_event evt;

	evt.type = type;
	evt.context.window = win;
	evt.context.element = ele;
	evt.flags = 0;

	fh_event_trigger(&evt);
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API void fh_PreventDefault(struct fh_event *evt)
{
	if(!evt) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	evt->flags |= FH_EVT_F_PDEF; 	
}
