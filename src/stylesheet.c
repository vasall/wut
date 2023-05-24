#include "stylesheet.h"


#include <stdlib.h>


FH_API void fh_ResetStylesheet(struct fh_stylesheet *sheet)
{
	if(!sheet) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* 
	 * DISPLAY
	 */
	sheet->display_mode = FH_DISPLAY_BLOCK;	

	/*
	 * SIZE
	 */
	sheet->vsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->vsize_min = fh_flex_set(FH_FLEX_RELATIVE, 0);
	sheet->vsize_max = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	sheet->hsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->hsize_min = fh_flex_set(FH_FLEX_RELATIVE, 0);
	sheet->hsize_max = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * POSITION
	 */
	sheet->vorientation = FH_ORIENTATION_TOP;
	sheet->vposition = fh_flex_set(FH_FLEX_AUTO, 0);

	sheet->horientation = FH_ORIENTATION_LEFT;
	sheet->hposition = fh_flex_set(FH_FLEX_AUTO, 0);

	/*
	 * PADDING 
	 */
	sheet->padding_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->padding_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

	/*
	 * INFILL
	 */
	sheet->infill_mode = FH_INFILL_COLOR;
	sheet->infill_color =  fh_col_set(0, 0, 0xFF, 0xff);

	/*
	 * BORDER
	 */
	sheet->border_mode = FH_BORDER_NONE;

}
