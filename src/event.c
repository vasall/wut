#include "event.h"

#include "system.h"

#include "core.h"



FH_API s8 fh_evt_pipe_append(struct fh_event evt)
{
	s16 cnum = g_fh_core.event_pipe.num;

	if(cnum + 1 > FH_EVT_PIPE_LIM) {
		ALARM(ALARM_ERR, "Event pipe is full");
		return -1;
	}


	g_fh_core.event_pipe.evts[cnum] = evt;
	g_fh_core.event_pipe.num++;

	return cnum + 1;
}


FH_API s8 fh_evt_pipe_push(u8 type, struct fh_window *win,
		void *data, u8 len)
{
	struct fh_event evt;
	s8 ret;	

	if(len > FH_EVT_DATA_LIM || (!!data && len < 1)) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	evt.type = type;
	evt.window = win;

	if(data && len > 0) {
		memcpy(evt.data, data, len);
	}

	if((ret = fh_evt_pipe_append(evt)) < 0) {
		ALARM(ALARM_ERR, "Failed to append new event");
		goto err_return;
	}

	return ret;

err_return:
	ALARM(ALARM_ERR, "Failed to push new event");
	return -1;
}


FH_API s8 fh_evt_pipe_pull(struct fh_event *evt)
{
	s16 cnum;

	if(!evt) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	
	cnum = g_fh_core.event_pipe.num;

	if(cnum < 1) {
		return 0;
	}

	printf("Length: %d\n", g_fh_core.event_pipe.off);
	*evt = g_fh_core.event_pipe.evts[g_fh_core.event_pipe.off];
	
	g_fh_core.event_pipe.num--;
	g_fh_core.event_pipe.off++;

	return 1;

err_return:
	ALARM(ALARM_ERR, "Failed to pull event from event pipe");
	return -1;
}


FH_INTERN void fh_evt_hdl_windowevent(struct fh_event evt)
{
	if(!evt.window) {
		ALARM(ALARM_WARN, "Window event does not belong to a window");
		goto err_return;
	}

	switch(evt.event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			/* User requests to close window */

			/* If this window is the main window */
			if(evt.window->info & FH_WIN_INFO_MAIN) {
				/* ...quit program */
				fh_core_quit();
			}
			/* Otherwise if it's just a normal subwindow */
			else {
				/* ...close it */
				fh_CloseWindow(evt.window);
			}

			break;

		case SDL_WINDOWEVENT_ENTER:
			/* Cursor enters window */

			fh_core_set_active_window(evt.window);

			break;

		case SDL_WINDOWEVENT_LEAVE:
			/* Cursor leaves window */

			fh_core_set_active_window(NULL);
			
			break;
	}

	return;

err_return:
	ALARM(ALARM_WARN, "Failed to handle window event");
}



FH_API void fh_evt_process(void)
{
	struct fh_event evt;

	fh_zeros(&evt, sizeof(struct fh_event));

	while(SDL_PollEvent(&evt.event)) {	


		switch(evt.event.type) {
			case SDL_QUIT:
				fh_core_quit();
				break;

			case SDL_WINDOWEVENT:
				/* Get a pointer to the window */	
				evt.window =
					fh_GetWindow(evt.event.window.windowID);

				fh_evt_hdl_windowevent(evt);
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

FH_API s8 fh_pull_event(struct fh_event *event)
{
	return fh_evt_pipe_pull(event);
}
