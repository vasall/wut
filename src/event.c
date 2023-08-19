#include "event.h"

#include "system.h"

#include "core.h"
#include "event_default.h"
#include "document.h"


FH_INTERN s8 evt_hdl_window(struct fh_event *evt)
{
	switch(evt->event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			/* User requests to close window */

			/* If this window is the main window */
			if(evt->window->info & FH_WIN_INFO_MAIN) {
				/* ...quit program */
				fh_core_quit();
			}
			/* Otherwise if it's just a normal subwindow */
			else {
				/* ...close it */
				fh_CloseWindow(evt->window);
			}

			return 1;

		case SDL_WINDOWEVENT_ENTER:
			/* Cursor enters window */

			fh_core_set_active_window(evt->window);

			return 1;

		case SDL_WINDOWEVENT_LEAVE:
			/* Cursor leaves window */

			if(fh_core_is_active_window(evt->window))
				fh_core_set_active_window(NULL);
			
			return 1;



		case SDL_WINDOWEVENT_SHOWN: break;
		case SDL_WINDOWEVENT_HIDDEN: break;
		case SDL_WINDOWEVENT_EXPOSED: break;
		case SDL_WINDOWEVENT_MOVED: break;
		case SDL_WINDOWEVENT_RESIZED: break;
		case SDL_WINDOWEVENT_SIZE_CHANGED: break;
		case SDL_WINDOWEVENT_MINIMIZED: break;
		case SDL_WINDOWEVENT_MAXIMIZED: break;
		case SDL_WINDOWEVENT_RESTORED: break;
		case SDL_WINDOWEVENT_FOCUS_GAINED: break;
		case SDL_WINDOWEVENT_FOCUS_LOST: break;
	}

	return 0;
}


FH_INTERN s8 evt_hdl_mousemotion(struct fh_event *evt)
{
	struct fh_sin2 pos;
	struct fh_element *ele;

	pos.x = evt->event.motion.x;
	pos.y = evt->event.motion.y;

	ele = fh_GetHoveredElement(evt->window->document, &pos);

	return 1;
}


FH_INTERN s8 evt_cfnc_scroll(struct fh_element *w, void *data)
{
	struct fh_sin2 *del = (struct fh_sin2 *)data;
	s8 ret = 0;

	if(w->scrollbar_flags & FH_RESTYLE_SCROLL_H) {
		w->content_offset += del.x;
		ret = 1;
	}

	if(w->scrollbar_flags & FH_RESTYLE_SCROLL_H) {
		w->content_offset += del.x;
		ret = 1;
	}

	return ret;
}


FH_INTERN s8 evt_hdl_mousewheel(struct fh_event *evt)
{
	struct fh_sin2 del;

	struct fh_sin2 pos;
	struct fh_element *ele;

	pos.x = evt->event.motion.x;
	pos.y = evt->event.motion.y;

	ele = fh_GetHoveredElement(evt->window->document, &pos);

	del.x = evt->event.wheel.x;
	del.y = evt->event.wheel.y;

	fh_ApplyElementsRise()
}



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API void fh_evt_process(void)
{
	struct fh_event evt;
	s8 r;

	fh_zeros(&evt, sizeof(struct fh_event));

	while(SDL_PollEvent(&evt.event)) {	
		if(evt.event.type != SDL_QUIT)
			evt.window = fh_GetWindow(evt.event.window.windowID);
		else
			evt.window = NULL;

		switch(evt.event.type) {
			case SDL_QUIT:
				fh_core_quit();
				r = 1;
				break;

			case SDL_WINDOWEVENT: evt_hdl_window(&evt); break;
			
			case SDL_MOUSEMOTION: evt_hdl_mousemotion(&evt); break;
			case SDL_MOUSEWHEEL: evt_hdl_mousewheel(&evt); break;

			case SDL_KEYDOWN:
				break;
		}

	}

	fh_zeros(&evt, sizeof(struct fh_event));
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

