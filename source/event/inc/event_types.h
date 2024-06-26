#ifndef _WT_EVENT_TYPES_H
#define _WT_EVENT_TYPES_H


enum wt_event_type {

	WT_EVT_UNKNOWN		= 0,

	/*
	 * WINDOW-EVENT
	 */

	WT_EVT_WINDOWCLOSE	= 1,
	
	WT_EVT_WINDOWENTER	= 2,
	WT_EVT_WINDOWLEAVE	= 3,

	WT_EVT_WINDOWSHOWN	= 4,
	WT_EVT_WINDOWHIDDEN	= 5,

	WT_EVT_WINDOWRESIZED	= 6,

	WT_EVT_ELEMENTADDED	= 7,

	/*
	 * ELEMENT-EVENT
	 */

	WT_EVT_ELEMENTREMOVED	= 8,
	WT_EVT_ELEMENTENTER	= 9,
	WT_EVT_ELEMENTLEAVE	= 10,
	WT_EVT_ELEMENTSELECT	= 11,
	WT_EVT_ELEMENTUNSELECT	= 12,

	/* MOUSE */
	WT_EVT_MOUSEMOTION	= 13,
	WT_EVT_MOUSEBUTTONDOWN	= 14,
	WT_EVT_MOUSEBUTTONUP	= 15,
	WT_EVT_MOUSEWHEEL	= 16,

	/* KEYBOARD */
	WT_EVT_KEYDOWN		= 17,
	WT_EVT_KEYUP		= 18

};


#endif /* _WT_EVENT_TYPES_H */
