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

	return cnum + 1;
}


XWIN_API s8 xwin_evt_pipe_pull(struct xwin_event *evt)
{
	s16 cnum;

	if(!evt) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	
	cnum = g_xwin_core.event_pipe.num;

	if(c_num < 1) {
		return 0;
	}

	*evt = g_xwin_core.event_pipe.evts[g_xwin_core.event_pipe.off];
	
	g_xwin_core.event_pipe.num--;
	g_xwin_core.event_pipe.off++;

	return 1;

err_return:
	ALARM(ALARM_ERR, "Failed to pull event from event pipe");
	return -1;
}



XWIN_API void xwin_evt_process(void)
{
	SDL_Event evt;

	while(SDL_PollEvent(&evt)) {
		
	}
}
