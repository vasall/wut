#include "event/inc/event.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include "core/inc/core.h"

#include "event/inc/event_default.h"

#include "document/inc/document.h"


#define WUT_EVT_DEBUG   0

WUT_INTERN s8 evt_is_mouse(u32 type)
{
	if(type == SDL_MOUSEMOTION) 	return 1;
	if(type == SDL_MOUSEBUTTONDOWN)	return 1;
	if(type == SDL_MOUSEBUTTONUP)	return 1;
	if(type == SDL_MOUSEWHEEL) 	return 1;
	return 0;
}


WUT_INTERN void evt_get_position(u32 t, SDL_Event *raw, wut_iVec2 p)
{
	switch(t) {
		case SDL_MOUSEMOTION:
			p[0] = raw->motion.x;
			p[1] = raw->motion.y;
			return;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			p[0] = raw->button.x;
			p[1] = raw->button.y;
			return;

		case SDL_MOUSEWHEEL:
			p[0] = raw->wheel.x;
			p[1] = raw->wheel.y;
			return;

		default:
			p[0] = 0;
			p[1] = 0;
	}
}


WUT_INTERN void evt_collect_info(SDL_Event *raw, struct wut_EventContext *ctx)
{
	u32 type = raw->type;
        struct wut_Document *doc;
	struct wut_Element *ele = NULL;
	wut_iVec2 pos = {0, 0};
        struct wut_Window *win;

	/*
	 * First get the window.
	 *
	 * By default the active window should be used. But in certain cases
	 * like if the user closes the program, the main window will be
	 * selected.
	 */
	if(type == SDL_QUIT) {
		win = wut_cor_get_main_window();
	}
	else {
		win = wut_GetWindow(raw->window.windowID);
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
		evt_get_position(type, raw, pos);
		
                switch(type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				ele = win->document->track_table.hovered;
                                break;

			default:
				ele = wut_doc_hovered(win->document, pos); 
		}
	}
	else if(win) {
		ele = win->document->track_table.selected;
	}
	else {
                /* TODO */
	}
	

	/*
	 * Attach the info to the context.
	 */
        wut_ivec2_cpy(ctx->position, pos);
	ctx->window = win;
	ctx->element = ele;
}


WUT_INTERN void evt_translate_type(struct wut_Event *evt)
{
	switch(evt->raw.type) {
		case SDL_WINDOWEVENT_CLOSE:
			evt->type = WUT_EVT_WINDOWCLOSE;
			return;

		case SDL_WINDOWEVENT_ENTER:
			evt->type = WUT_EVT_WINDOWENTER;
			return;

		case SDL_WINDOWEVENT_LEAVE:
			evt->type = WUT_EVT_WINDOWLEAVE;
			return;

		case SDL_WINDOWEVENT_SHOWN:
			evt->type = WUT_EVT_WINDOWSHOWN;
			return;

		case SDL_WINDOWEVENT_HIDDEN:
			evt->type = WUT_EVT_WINDOWHIDDEN;
			return;

		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			evt->type = WUT_EVT_WINDOWRESIZED;
			return;

		case SDL_MOUSEMOTION:
			evt->type = WUT_EVT_MOUSEMOTION;
			return;

		case SDL_MOUSEBUTTONDOWN:
			evt->type = WUT_EVT_MOUSEBUTTONDOWN;
			return;

		case SDL_MOUSEBUTTONUP:
			evt->type = WUT_EVT_MOUSEBUTTONUP;
			return;

		case SDL_MOUSEWHEEL:
			evt->type = WUT_EVT_MOUSEWHEEL;
			return;

		case SDL_KEYDOWN:
			evt->type = WUT_EVT_KEYDOWN;
			return;

		case SDL_KEYUP:
			evt->type = WUT_EVT_KEYUP;
			return;

		default:
#if WUT_EVT_DEBUG
			printf("Event type unknown (%d)\n", evt->raw.type);
#endif
			return;
	}
}


WUT_INTERN struct wut_EventHandler *evt_find_handler(struct wut_Event *evt)
{
	if(evt->context.element) return evt->context.element->event_handler;
	if(evt->context.window) return evt->context.window->event_handler;
	return NULL;
}


WUT_INTERN s8 evt_raise_context(struct wut_Event *evt)
{
	if(!evt->context.element)
		return 0;

	if(!evt->context.element->parent)
		return 0;

	evt->context.element = evt->context.element->parent;
	return 1;
}


WUT_INTERN s8 evt_rundow(struct wut_EventHandler *hdl, struct wut_Event *evt)
{
	s8 ret = 0;

	/*
	 * Check if one of the custom event handler uses the event.
	 */
	if(wut_hdl_rundow(hdl, evt))
		ret = 1;

	/*
	 * If the user activated the PREVENT_DEFAULT flag, the code will skip
	 * the next part.
	 */
	if(evt->flags & WUT_EVT_F_PDEF)
		return ret;

	/*
	 *  Apply the default handler onto the given event.
	 */
	if(wut_evd_rundown(evt))
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


WUT_XMOD void wut_evt_update(void)
{
	struct wut_Event evt;

	wut_zeros(&evt, sizeof(struct wut_Event));

	while(SDL_PollEvent(&evt.raw)) {
		if(evt.raw.type == SDL_QUIT) {
			wut_cor_quit();
			return;
		}
	
		/* 
		 * First collect the relative context for this event like the
		 * related window and element.
		 */
		evt_collect_info(&evt.raw, &evt.context);

		/*
		 * Second translate the SDL-event-type to a WUT-event-type.
		 */
		evt_translate_type(&evt);

		/*
		 * Then trigger the enhanced event internally so it can be
		 * handled.
		 */
		wut_evt_trigger(&evt);
	}

	wut_zeros(&evt, sizeof(struct wut_Event));
}


WUT_XMOD void wut_evt_trigger(struct wut_Event *evt)
{
	struct wut_EventHandler *hdl;

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


WUT_XMOD void wut_evt_trigger_raw(enum wut_eEventType type,
		struct wut_Window *win, struct wut_Element *ele)
{
	struct wut_Event evt;

	evt.type = type;
	evt.context.window = win;
	evt.context.element = ele;
	evt.flags = 0;

	wut_evt_trigger(&evt);
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


WUT_API void wut_PreventDefault(struct wut_Event *evt)
{
	if(!evt) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	evt->flags |= WUT_EVT_F_PDEF; 	
}
