#ifndef _FH_CORE_PREDEF_RESOURCES_H
#define _FH_CORE_PREDEF_RESOURCES_H

#include "./define.h"
#include "./import.h"

FH_API const u8 fh_ps_num;
FH_API char *fh_ps_lst[3][3];


#define FH_PS_BATCH_VLEN	606
FH_API u8 fh_ps_batch_vshader[FH_PS_BATCH_VLEN];
#define FH_PS_BATCH_FLEN	553
FH_API u8 fh_ps_batch_fshader[FH_PS_BATCH_FLEN];




#endif /* _FH_CORE_PREDEF_RESOURCES_H */
