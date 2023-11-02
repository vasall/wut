#include "graphics/inc/flex.h"

#include "system/inc/system.h"

#include <stdlib.h>



FH_API struct fh_flex fh_flex_set(u8 mode, u32 value)
{
	struct  fh_flex flex;

	/* Limit the relative value */
	if(mode == FH_FLEX_RELATIVE && value > 10000) {
		value = 10000;
	}

	flex.mode = mode;
	flex.value = value;

	return flex;
}


FH_API u32 fh_flex_comp(struct fh_flex *flex, u32 ref)
{
	if(!flex) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return 0;
	}


	switch(flex->mode) {
		case(FH_FLEX_ABSOLUTE):
			return flex->value;
			break;

		case(FH_FLEX_RELATIVE):
			return (ref * ((float)flex->value / 10000.00));
			break;

		case(FH_FLEX_INHERIT):
			return ref;
			break;

		case(FH_FLEX_AUTO):
			return (ref / 2); 
			break;
	}

	return 0;
}


FH_API u32 fh_flex_comp_lim(struct fh_flex *flex, struct fh_flex *min,
		struct fh_flex *max, u32 ref)
{
	u32 value;
	u32 minv;
	u32 maxv;

	value = fh_flex_comp(flex, ref);
	minv = fh_flex_comp(min, ref);
	maxv = fh_flex_comp(max, ref);

	if(value > maxv)
		return maxv;

	if(value < minv)
		return minv;

	return value;
}


FH_API void fh_flex_show(struct fh_flex *flex)
{
	if(!flex)
		return;


}
