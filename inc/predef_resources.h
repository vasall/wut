#ifndef _FH_PREDEF_RESOURCES_H
#define _FH_PREDEF_RESOURCES_H

#include "define.h"



FH_INTERN const u8 fh_ps_num = 3;
FH_INTERN char *fh_ps_lst[3][3] = {
	{"flat", "res/predef/shaders/flat.vert", "res/predef/shaders/flat.frag"},
	{"ui", "res/predef/shaders/ui.vert", "res/predef/shaders/ui.frag"},
	{"model", "res/predef/shaders/model.vert", "res/predef/shaders/model.frag"}
};





#endif /* _FH_PREDEF_RESOURCES_H */
