#include "event_default.h"


#include "core.h"
#include "document.h"

#define FH_EVTDEF_DEBUG		1


FH_INTERN s8 def_dump(struct fh_event *evt)
{
	if(evt->context.window)
		printf("window: %s   ", evt->context.window->name);

	if(evt->context.element)
		printf("element: %s   ", evt->context.element->name);

	printf("\n");
}


FH_INTERN s8 def_windowclose(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("windowclose\n");
	def_dump(evt);
#endif
	

	/* If this window is the main window */
	if(evt->context.window->info & FH_WIN_INFO_MAIN) {
		/* ...quit program */
		fh_core_quit();
	}
	/* Otherwise if it's just a normal subwindow */
	else {
		/* ...close it */
		fh_CloseWindow(evt->context.window);
	}

	return 1;
}


FH_INTERN s8 def_windowenter(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("windowenter\n");
	def_dump(evt);
#endif

	/* Cursor enters window */

	fh_core_set_active_window(evt->context.window);

	return 1;
}


FH_INTERN s8 def_windowleave(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("windowleave\n");
	def_dump(evt);
#endif

	/* Cursor leaves window */

	if(fh_core_is_active_window(evt->context.window))
		fh_core_set_active_window(NULL);

	return 1;
}


FH_INTERN s8 def_windowshown(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("windowshown\n");
	def_dump(evt);
#endif
	
	fh_Ignore(evt);

	return 1;
}


FH_INTERN s8 def_windowhidden(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("windowhidden\n");
	def_dump(evt);
#endif
	
	fh_Ignore(evt);
	
	return 1;
}


FH_INTERN s8 def_windowresized(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("windowresized\n");
	def_dump(evt);
#endif

	/* User resizes window */

	fh_Ignore(evt);

	/* Resize UI */

	return 1;
}


FH_INTERN s8 def_elementadded(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("elementadded\n");
	def_dump(evt);
#endif
	
	fh_Ignore(evt);

	return 1;
}


FH_INTERN s8 def_elementremoved(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("ellementremoved\n");
	def_dump(evt);
#endif
	
	fh_Ignore(evt);

	return 1;
}


FH_INTERN s8 def_elemententer(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("elemententer\n");
	def_dump(evt);
#endif
	
	printf("Entered %s\n", evt->context.element->name);

	return 1;
}


FH_INTERN s8 def_elementleave(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("elementleave\n");
	def_dump(evt);
#endif
	
	printf("Leave %s\n", evt->context.element->name);

	return 1;
}


FH_INTERN s8 def_elementselect(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("elementselect\n");
	def_dump(evt);
#endif

	printf("Select %s\n", evt->context.element->name);

	return 1;
}


FH_INTERN s8 def_elementunselect(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("elementunselect\n");
	def_dump(evt);
#endif

	printf("Unselect %s\n", evt->context.element->name);

	return 1;
}


FH_INTERN s8 def_mousemotion(struct fh_event *evt)
{
	struct fh_sin2 pos;

	struct fh_window *s_w = evt->context.window;
	struct fh_element *s_e = evt->context.element;


#if FH_EVTDEF_DEBUG
	printf("mousemotion\n");
	def_dump(evt);
#endif
	

	return fh_window_hover(s_w, s_e);
}


FH_INTERN s8 def_mousebuttondown(struct fh_event *evt)
{
	struct fh_window *s_w = evt->context.window;
	struct fh_element *s_e = evt->context.element;

#if FH_EVTDEF_DEBUG
	printf("mousebuttondown\n");
	def_dump(evt);
#endif


	return fh_window_select(s_w, s_e);
}


FH_INTERN s8 def_mousebuttonup(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("mousebuttonup\n");
	def_dump(evt);
#endif

	fh_Ignore(evt);

	return 1;
}


FH_INTERN s8 def_mousewheel(struct fh_event *evt)
{
	s8 ret = 0;
	struct fh_element *ele = evt->context.element;


#if FH_EVTDEF_DEBUG
	printf("mousehweel\n");
	def_dump(evt);
#endif

	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_V) {
		ele->content_offset.y += evt->raw.wheel.y;
		ret = 1;
	}

	if(ele->scrollbar_flags & FH_RESTYLE_SCROLL_H) {
		ele->content_offset.x += evt->raw.wheel.x;
		ret = 1;
	}

	return ret;
}


FH_INTERN s8 def_keydown(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("keydown\n");
	def_dump(evt);
#endif
	
	fh_Ignore(evt);
	
	return 1;
}


FH_INTERN s8 def_keyup(struct fh_event *evt)
{
#if FH_EVTDEF_DEBUG
	printf("keyup\n");
	def_dump(evt);
#endif
	
	fh_Ignore(evt);

	return 1;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_XMOD s8 fh_evtdef_rundown(struct fh_event *evt)
{
	switch(evt->type) {
		/*
		 * WINDOW-EVENT
		 */
		case FH_EVT_WINDOWCLOSE:	return def_windowclose(evt);

		case FH_EVT_WINDOWENTER:	return def_windowenter(evt);
		case FH_EVT_WINDOWLEAVE:	return def_windowleave(evt);

		case FH_EVT_WINDOWSHOWN:	return def_windowshown(evt);
		case FH_EVT_WINDOWHIDDEN:	return def_windowhidden(evt);

		case FH_EVT_WINDOWRESIZED:	return def_windowresized(evt);

		case FH_EVT_ELEMENTADDED:	return def_elementadded(evt);

		case FH_EVT_ELEMENTREMOVED:	return def_elementremoved(evt);

		case FH_EVT_ELEMENTENTER:	return def_elemententer(evt);
		case FH_EVT_ELEMENTLEAVE:	return def_elementleave(evt);
		case FH_EVT_ELEMENTSELECT:	return def_elementselect(evt);
		case FH_EVT_ELEMENTUNSELECT:	return def_elementunselect(evt);

		case FH_EVT_MOUSEMOTION:	return def_mousemotion(evt);
		case FH_EVT_MOUSEBUTTONDOWN:	return def_mousebuttondown(evt);
		case FH_EVT_MOUSEBUTTONUP:	return def_mousebuttonup(evt);
		case FH_EVT_MOUSEWHEEL:		return def_mousewheel(evt);

		case FH_EVT_KEYDOWN: 		return def_keydown(evt);
		case FH_EVT_KEYUP: 		return def_keyup(evt);

		default: return 1;
	}
}
