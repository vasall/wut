#ifndef _FH_OBJECT_H
#define _FH_OBJECT_H

#include "define.h"
#include "import.h"
#include "model.h"


struct fh_object {
	/* 
	 * The current position of the object with a x-, y- and z-position.
	 */
	vec3_t position;

	/*
	 * The rotation of the object in radians around the x-, y- and z-axis.
	 */
	vec3_t rotation;
	
	/*
	 * A reference to the model used.
	 */
	struct fh_model *model;
};







#endif /* _FH_OBJECT_H */
