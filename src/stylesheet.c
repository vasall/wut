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
	 * REFERENCE
	 */
	sheet->reference_mode = FH_REFERENCE_RELATIVE;

	/*
	 * SIZE
	 */
	sheet->vsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);
	sheet->hsize = fh_flex_set(FH_FLEX_RELATIVE, 10000);

	/*
	 * SPACING
	 */
	sheet->spacing_top = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_right = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_bottom = fh_flex_set(FH_FLEX_ABSOLUTE, 0);
	sheet->spacing_left = fh_flex_set(FH_FLEX_ABSOLUTE, 0);

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
	 * LAYOUT
	 */
	sheet->layout_mode = FH_LAYOUT_BLOCKS;

	/*
	 * SCROLLBAR
	 */
	sheet->scrollbar_mode = FH_SCROLLBAR_VERTICAL;
}
