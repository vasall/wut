#ifndef _FH_GRAPHIC_CAMERA_H
#define _FH_GRAPHIC_CAMERA_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#define FH_CAM_NAME_LIM		128
#define FH_CAM_PITCH_LIM	50.0

enum fh_cam_mode {
	FH_CAM_FREE,
	FH_CAM_FOCUS
};


struct fh_camera_info {
	f32 area_of_view;
	f32 aspect_ratio;
	f32 near;
	f32 far;	
};

struct fh_camera {
	struct fh_view *view;

	enum fh_cam_mode mode;

	vec3_t pos;

	vec3_t v_forward;
	vec3_t v_right;

	struct fh_camera_info info;

	vec3_t aim;
	f32 dist;
	mat4_t forw_m;

	mat4_t view_m;
	mat4_t projection_m;

	f32 sens;

	/*
	 * FOCUS-MODE
	 * 
	 * With this mode, the camera will focus on a given target.
	 */

	vec3_t target;		/* The point to focus on */
	f32 distance;		/* The distance from the target point */
};


/*
 * Create and initialize a new camera.
 *
 * @info: A buffer containing the essential data for the camera
 * @[view]: A pointer to the view struct
 *
 * Returns: Either a pointer to the camera or NULL if an error occurred
 */
FH_API struct fh_camera *fh_CreateCamera(struct fh_camera_info info,
		struct fh_view *view);


/*
 * Destroy a camera and free the allocated memory.
 *
 * @cam: Pointer to the camera
 */
FH_API void fh_DestroyCamera(struct fh_camera *cam);


/*
 * Get the current view matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @cam: Pointer to the camera
 * @out: A matrix to write the view matrix to
 */
FH_API void fh_GetViewMat(struct fh_camera *cam, mat4_t out);


/*
 * Get the current projection matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @cam: Pointer to the camera
 * @out: A matrix to write the projection matrix to
 */
FH_API void fh_GetProjectionMat(struct fh_camera *cam, mat4_t out);


/*
 * Get the current position of a camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @out: A vector to write the position to
 */
FH_API void fh_GetCameraPosition(struct fh_camera *cam, vec3_t out);

/*
 * Set the position of a camera.
 *
 * @cam: Pointer to the camera
 * @pos: The new position of the camera
 */
FH_API void fh_SetCameraPosition(struct fh_camera *cam, vec3_t pos);


/*
 * Move the camera by a given delta.
 *
 * @cam: Pointer to the camera
 * @del: The position difference to move the camera by
 */
FH_API void fh_MoveCamera(struct fh_camera *cam, vec3_t del);


/*
 * Get the current direction of a camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @out: A vector to write the direction to
 */
FH_API void fh_GetCameraDirection(struct fh_camera *cam, vec3_t out);


/*
 * Set the direction of the camera. This function will then also update the
 * view-matrix.
 *
 * @cam: Pointer to the camera
 * @dir: The new direction of the camera
 */
FH_API void fh_SetCameraDirection(struct fh_camera *cam, vec3_t dir);


/*
 * Get the current mode of the camera.
 *
 * @cam: Pointer to the camera
 *
 * Returns: The current mode of the camera
 */
FH_API enum fh_cam_mode fh_GetCameraMode(struct fh_camera *cam); 


/*
 * Set the mode of the camera.
 *
 * @cam: Pointer to the camera
 * @mode: The new mode of the camera
 */
FH_API void fh_SetCameraMode(struct fh_camera *cam, enum fh_cam_mode mode);


/*
 * Toggle the mode the camera.
 *
 * @cam: Pointer to the camera
 */
FH_API void fh_ToggleCameraMode(struct fh_camera *cam);


/*
 * Change the direction of the camera to look at the given point.
 * This will only work if the camera is in the Free-Mode.
 *
 * @cam: Pointer to the camera
 * @pnt: The point to look at
 */
FH_API void fh_CameraLookAt(struct fh_camera *cam, vec3_t pnt);


/*
 * Switch the camera to focus mode and target a focus point.
 *
 * @cam: Pointer to the camera
 * @trg: The target point to focus on
 */
FH_API void fh_FocusCamera(struct fh_camera *cam, vec3_t trg);


/*
 * Zoom by a given value.
 *
 * @cam: Pointer to the camera
 * @f: The value to zoom by
 */
FH_API void fh_CameraZoom(struct fh_camera *cam, f32 f);



FH_API void fh_CameraRotate(struct fh_camera *cam, f32 d_yaw, f32 d_pitch);


/*
 * Get the information about the camera.
 *
 * @cam: Pointer to the camera
 *
 * Returns: The info struct
 */
FH_API struct fh_camera_info fh_GetCameraInfo(struct fh_camera *cam);


/*
 * Set the information of the camera and recalculate the projection matrix.
 *
 * @cam: Pointer to the camera
 * @info: The information struct
 */
FH_API void fh_SetCameraInfo(struct fh_camera *cam, struct fh_camera_info info);

/*
 * Update the camera and recalculate the view matrix.
 *
 * @cam: Pointer to the camera
 */
FH_API void fh_UpdateCamera(struct fh_camera *cam);

#endif /* _FH_GRAPHIC_CAMERA_H */
