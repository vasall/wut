#include "event/inc/event_default.h"

#include "core/inc/core.h"

#include "document/inc/document.h"

#define WUT_EVD_DEBUG		0

#define WUT_EVD_SCROLL_FACTOR   20

WUT_INTERN s8 evd_dump(struct wut_Event *evt)
{
	if(evt->context.window)
		printf("window: %s   ", evt->context.window->name);

	if(evt->context.element)
		printf("element: %s   ", evt->context.element->name);
        else
                printf("element: <unknown>   ");

	printf("\n");

	return 0;
}


WUT_INTERN s8 evd_windowclose(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("windowclose\n");
	evd_dump(evt);
#endif
	

	/* If this window is the main window */
	if(evt->context.window->info & WUT_WIN_INFO_MAIN) {
		/* ...quit program */
		wut_cor_quit();
	}
	/* Otherwise if it's just a normal subwindow */
	else {
		/* ...close it */
		wut_CloseWindow(evt->context.window);
	}

	return 1;
}


WUT_INTERN s8 evd_windowenter(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("windowenter\n");
	evd_dump(evt);
#endif

	/* Cursor enters window */

	wut_cor_set_active_window(evt->context.window);

	return 1;
}


WUT_INTERN s8 evd_windowleave(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("windowleave\n");
	evd_dump(evt);
#endif

	/* Cursor leaves window */

	if(wut_cor_is_active_window(evt->context.window))
		wut_cor_set_active_window(NULL);

	return 1;
}


WUT_INTERN s8 evd_windowshown(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("windowshown\n");
	evd_dump(evt);
#endif
	
	WUT_IGNORE(evt);

	return 1;
}


WUT_INTERN s8 evd_windowhidden(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("windowhidden\n");
	evd_dump(evt);
#endif
	
	WUT_IGNORE(evt);
	
	return 1;
}


WUT_INTERN s8 evd_windowresized(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("windowresized\n");
	evd_dump(evt);
#endif

	/* User resizes window */

	WUT_IGNORE(evt);

	/* Resize UI */

	return 1;
}


WUT_INTERN s8 evd_elementadded(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("elementadded\n");
	evd_dump(evt);
#endif
	
	WUT_IGNORE(evt);

	return 1;
}


WUT_INTERN s8 evd_elementremoved(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("elementremoved\n");
	evd_dump(evt);
#endif
	
	WUT_IGNORE(evt);

	return 1;
}


WUT_INTERN s8 evd_elemententer(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
        printf("Event: %p\n", (void *)evt);
        printf("Element: %p\n", (void *)evt->context.element);

        /* 
	printf("elemententer (%s)\n", evt->context.element->name);
	evd_dump(evt);
        */
#endif

	/* TODO: This is only a Hotfix*/
	if(!evt->context.element) return 1;
	
        return 1;
}


WUT_INTERN s8 evd_elementleave(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("elementleave (%s)\n", evt->context.element->name);
	evd_dump(evt);
#endif

	return 1;
}


WUT_INTERN s8 evd_elementselect(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("elementselect (%s)\n", evt->context.element->name);
	evd_dump(evt);
#endif

	return 1;
}


WUT_INTERN s8 evd_elementunselect(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("elementunselect (%s)\n", evt->context.element->name);
	evd_dump(evt);
#endif

	return 1;
}


WUT_INTERN s8 evd_mousemotion(struct wut_Event *evt)
{
	struct wut_Window *s_w = evt->context.window;
	struct wut_Element *s_e = evt->context.element;


#if WUT_EVD_DEBUG
	printf("mousemotion\n");
	evd_dump(evt);
#endif

        wut_doc_track_scrollbar(s_w->document, s_e, evt->context.position);

	return wut_win_hover(s_w, s_e);
}


WUT_INTERN s8 evd_mousebuttondown(struct wut_Event *evt)
{
	struct wut_Window *s_w = evt->context.window;
	struct wut_Element *s_e = evt->context.element;

#if WUT_EVD_DEBUG
	printf("mousebuttondown\n");
	evd_dump(evt);
#endif


	return wut_win_select(s_w, s_e);
}


WUT_INTERN s8 evd_mousebuttonup(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("mousebuttonup\n");
	evd_dump(evt);
#endif

	WUT_IGNORE(evt);

	return 1;
}


WUT_INTERN s8 evd_mousewheel(struct wut_Event *evt)
{
	struct wut_Element *ele = evt->context.element;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

        s32 value[2];

#if WUT_EVD_DEBUG
	printf("mousehweel\n");
	evd_dump(evt);
#endif

        
        if(keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT]) {
        	value[0] = evt->raw.wheel.y * WUT_EVD_SCROLL_FACTOR;
                value[1] = 0;
        }
        else {
                
                value[0] = 0;
        	value[1] = evt->raw.wheel.y * WUT_EVD_SCROLL_FACTOR;
        }

	return wut_ele_scroll(ele, value);
}


WUT_INTERN s8 evd_keydown(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("keydown\n");
	evd_dump(evt);
#endif
	
	WUT_IGNORE(evt);
	
	return 1;
}


WUT_INTERN s8 evd_keyup(struct wut_Event *evt)
{
#if WUT_EVD_DEBUG
	printf("keyup\n");
	evd_dump(evt);
#endif
	
	WUT_IGNORE(evt);

	return 1;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				CROSS-MODULE-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_XMOD s8 wut_evd_rundown(struct wut_Event *evt)
{
	switch(evt->type) {
		/*
		 * WINDOW-EVENT
		 */
		case WUT_EVT_WINDOWCLOSE:	return evd_windowclose(evt);

		case WUT_EVT_WINDOWENTER:	return evd_windowenter(evt);
		case WUT_EVT_WINDOWLEAVE:	return evd_windowleave(evt);

		case WUT_EVT_WINDOWSHOWN:	return evd_windowshown(evt);
		case WUT_EVT_WINDOWHIDDEN:	return evd_windowhidden(evt);

		case WUT_EVT_WINDOWRESIZED:	return evd_windowresized(evt);

		case WUT_EVT_ELEMENTADDED:	return evd_elementadded(evt);

		case WUT_EVT_ELEMENTREMOVED:	return evd_elementremoved(evt);

		case WUT_EVT_ELEMENTENTER:	return evd_elemententer(evt);
		case WUT_EVT_ELEMENTLEAVE:	return evd_elementleave(evt);
		case WUT_EVT_ELEMENTSELECT:	return evd_elementselect(evt);
		case WUT_EVT_ELEMENTUNSELECT:	return evd_elementunselect(evt);

		case WUT_EVT_MOUSEMOTION:	return evd_mousemotion(evt);
		case WUT_EVT_MOUSEBUTTONDOWN:	return evd_mousebuttondown(evt);
		case WUT_EVT_MOUSEBUTTONUP:	return evd_mousebuttonup(evt);
		case WUT_EVT_MOUSEWHEEL:		return evd_mousewheel(evt);

		case WUT_EVT_KEYDOWN: 		return evd_keydown(evt);
		case WUT_EVT_KEYUP: 		return evd_keyup(evt);

		default: return 1;
	}
}
