#ifndef _XWIN_GLOBAL_H
#define _XWIN_GLOBAL_H


#include "define.h"
#include "imports.h"


struct xwin_container {
	s8 ready;

	struct xwin_window *main_window;	
};



XWIN_API struct xwin_container g_cnt;






#endif /* _XWIN_GLOBAL_H */
