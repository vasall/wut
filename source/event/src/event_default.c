#include "event/inc/event_default.h"

#include "core/inc/core.h"

#include "document/inc/document.h"

#define WT_EVTDEF_DEBUG		0


WT_INTERN s8 def_dump(struct wt_event *evt)
{
	if(evt->context.window)
		printf("window: %s   ", evt->context.window->name);

	if(evt->context.element)
		printf("element: %s   ", evt->context.element->name);

	printf("\n");

	return 0;
}


WT_INTERN s8 def_windowclose(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("windowclose\n");
	def_dump(evt);
#endif
	

	/* If this window is the main window */
	if(evt->context.window->info & WT_WIN_INFO_MAIN) {
		/* ...quit program */
		wt_core_quit();
	}
	/* Otherwise if it's just a normal subwindow */
	else {
		/* ...close it */
		wt_CloseWindow(evt->context.window);
	}

	return 1;
}


WT_INTERN s8 def_windowenter(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("windowenter\n");
	def_dump(evt);
#endif

	/* Cursor enters window */

	wt_core_set_active_window(evt->context.window);

	return 1;
}


WT_INTERN s8 def_windowleave(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("windowleave\n");
	def_dump(evt);
#endif

	/* Cursor leaves window */

	if(wt_core_is_active_window(evt->context.window))
		wt_core_set_active_window(NULL);

	return 1;
}


WT_INTERN s8 def_windowshown(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("windowshown\n");
	def_dump(evt);
#endif
	
	wt_Ignore(evt);

	return 1;
}


WT_INTERN s8 def_windowhidden(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("windowhidden\n");
	def_dump(evt);
#endif
	
	wt_Ignore(evt);
	
	return 1;
}


WT_INTERN s8 def_windowresized(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("windowresized\n");
	def_dump(evt);
#endif

	/* User resizes window */

	wt_Ignore(evt);

	/* Resize UI */

	return 1;
}


WT_INTERN s8 def_elementadded(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("elementadded\n");
	def_dump(evt);
#endif
	
	wt_Ignore(evt);

	return 1;
}


WT_INTERN s8 def_elementremoved(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("elementremoved\n");
	def_dump(evt);
#endif
	
	wt_Ignore(evt);

	return 1;
}


WT_INTERN s8 def_elemententer(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("elemententer\n");
	def_dump(evt);
#endif

	/* TODO: This is only a Hotfix*/
	if(!evt->context.element) return 1;

	printf("Entered %s\n", evt->context.element->name);

	return 1;
}


WT_INTERN s8 def_elementleave(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("elementleave\n");
	def_dump(evt);
#endif
	
	printf("Leave %s\n", evt->context.element->name);

	return 1;
}


WT_INTERN s8 def_elementselect(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("elementselect\n");
	def_dump(evt);
#endif

	printf("Select %s\n", evt->context.element->name);

	return 1;
}


WT_INTERN s8 def_elementunselect(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("elementunselect\n");
	def_dump(evt);
#endif

	printf("Unselect %s\n", evt->context.element->name);

	return 1;
}


WT_INTERN s8 def_mousemotion(struct wt_event *evt)
{
	struct wt_window *s_w = evt->context.window;
	struct wt_element *s_e = evt->context.element;


#if WT_EVTDEF_DEBUG
	printf("mousemotion\n");
	def_dump(evt);
#endif
	

	return wt_window_hover(s_w, s_e);
}


WT_INTERN s8 def_mousebuttondown(struct wt_event *evt)
{
	struct wt_window *s_w = evt->context.window;
	struct wt_element *s_e = evt->context.element;

#if WT_EVTDEF_DEBUG
	printf("mousebuttondown\n");
	def_dump(evt);
#endif


	return wt_window_select(s_w, s_e);
}


WT_INTERN s8 def_mousebuttonup(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("mousebuttonup\n");
	def_dump(evt);
#endif

	wt_Ignore(evt);

	return 1;
}


WT_INTERN s8 def_mousewheel(struct wt_event *evt)
{
	struct wt_element *ele = evt->context.element;
	s32 value[2];
	s8 f = 8;

#if WT_EVTDEF_DEBUG
	printf("mousehweel\n");
	def_dump(evt);
#endif

	value[0] = evt->raw.wheel.x * f;
	value[1] = evt->raw.wheel.y * f;

	return wt_element_scroll(ele, value);
}


WT_INTERN s8 def_keydown(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("keydown\n");
	def_dump(evt);
#endif
	
	wt_Ignore(evt);
	
	return 1;
}


WT_INTERN s8 def_keyup(struct wt_event *evt)
{
#if WT_EVTDEF_DEBUG
	printf("keyup\n");
	def_dump(evt);
#endif
	
	wt_Ignore(evt);

	return 1;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_XMOD s8 wt_evtdef_rundown(struct wt_event *evt)
{
	switch(evt->type) {
		/*
		 * WINDOW-EVENT
		 */
		case WT_EVT_WINDOWCLOSE:	return def_windowclose(evt);

		case WT_EVT_WINDOWENTER:	return def_windowenter(evt);
		case WT_EVT_WINDOWLEAVE:	return def_windowleave(evt);

		case WT_EVT_WINDOWSHOWN:	return def_windowshown(evt);
		case WT_EVT_WINDOWHIDDEN:	return def_windowhidden(evt);

		case WT_EVT_WINDOWRESIZED:	return def_windowresized(evt);

		case WT_EVT_ELEMENTADDED:	return def_elementadded(evt);

		case WT_EVT_ELEMENTREMOVED:	return def_elementremoved(evt);

		case WT_EVT_ELEMENTENTER:	return def_elemententer(evt);
		case WT_EVT_ELEMENTLEAVE:	return def_elementleave(evt);
		case WT_EVT_ELEMENTSELECT:	return def_elementselect(evt);
		case WT_EVT_ELEMENTUNSELECT:	return def_elementunselect(evt);

		case WT_EVT_MOUSEMOTION:	return def_mousemotion(evt);
		case WT_EVT_MOUSEBUTTONDOWN:	return def_mousebuttondown(evt);
		case WT_EVT_MOUSEBUTTONUP:	return def_mousebuttonup(evt);
		case WT_EVT_MOUSEWHEEL:		return def_mousewheel(evt);

		case WT_EVT_KEYDOWN: 		return def_keydown(evt);
		case WT_EVT_KEYUP: 		return def_keyup(evt);

		default: return 1;
	}
}
