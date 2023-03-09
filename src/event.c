#include "event.h"

#include "core.h"



XWIN_API void xwin_evt_pipe_clear(void)
{
	g_xwin_core.event_pipe.off = 0;
	g_xwin_core.event_pipe.num = 0;
}


XWIN_API s8 xwin_evt_pipe_append(struct xwin_event evt)
{
	s16 cnum = g_xwin_core.event_pipe.num;

	if(cnum + 1 > XWIN_EVT_PIPE_LIM) {
		ALARM(ALARM_ERR, "Event pipe is full");
		return -1;
	}


	g_xwin_core.event_pipe.evts[cnum] = evt;
	g_xwin_core.event_pipe.num++;

	return cnum + 1;
}


XWIN_API s8 xwin_evt_pipe_push(u8 type, struct xwin_window *win,
		void *data, u8 len)
{
	struct xwin_event evt;
	s8 ret;	

	if(len > XWIN_EVT_DATA_LIM || (!!data && len < 1)) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	evt.type = type;
	evt.window = win;

	if(data && len > 0) {
		memcpy(evt.data, data, len);
	}

	if((ret = xwin_evt_pipe_append(evt)) < 0) {
		ALARM(ALARM_ERR, "Failed to append new event");
		goto err_return;
	}

	return ret;

err_return:
	ALARM(ALARM_ERR, "Failed to push new event");
	return -1;
}


XWIN_API s8 xwin_evt_pipe_pull(struct xwin_event *evt)
{
	s16 cnum;

	if(!evt) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	
	cnum = g_xwin_core.event_pipe.num;

	if(cnum < 1) {
		return 0;
	}

	printf("Length: %d\n", g_xwin_core.event_pipe.off);
	*evt = g_xwin_core.event_pipe.evts[g_xwin_core.event_pipe.off];
	
	g_xwin_core.event_pipe.num--;
	g_xwin_core.event_pipe.off++;

	return 1;

err_return:
	ALARM(ALARM_ERR, "Failed to pull event from event pipe");
	return -1;
}


XWIN_INTERN void xwin_evt_hdl_windowevent(struct xwin_event evt)
{
	if(!evt.window) {
		ALARM(ALARM_WARN, "Window event does not belong to a window");
		goto err_return;
	}

	printf("ARRRR\n");

	switch(evt.event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			printf("Info: %d\n", evt.window->info);

			/* If this window is the main window */
			if(evt.window->info & XWIN_WIN_INFO_MAIN) {
				printf("Close main window\n");

				/* ...quit program */
				xwin_evt_pipe_push(XWIN_EVT_QUIT, NULL, NULL, 0);
			}
			/* Otherwise if it's just a normal subwindow */
			else {
				/* ...close it */
				printf("Close window: %s\n", evt.window->name);

				xwin_win_close(evt.window);
			}

			break;
	}

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to handle window event");
}



XWIN_API void xwin_evt_process(void)
{
	struct xwin_event evt;

	szeros(&evt, sizeof(struct xwin_event));

	while(SDL_PollEvent(&evt.event)) {	

		/* Get a pointer to the window */
		evt.window = xwin_win_get(evt.event.window.windowID);	
		printf("%d, %p\n", evt.event.window.windowID, evt.window);


		switch(evt.event.type) {
			case SDL_QUIT:
				printf("Quit program\n");
				xwin_evt_pipe_push(XWIN_EVT_QUIT, NULL, NULL, 0);
				break;

			case SDL_WINDOWEVENT:
				printf("Handle window event\n");
				xwin_evt_hdl_windowevent(evt);
				break;
		}

	}

	szeros(&evt, sizeof(struct xwin_event));
}
