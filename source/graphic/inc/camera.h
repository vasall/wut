#ifndef _WT_GRAPHIC_CAMERA_H
#define _WT_GRAPHIC_CAMERA_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "utility/inc/extended_math.h"

#define WT_CAM_NAME_LIM		128
#define WT_CAM_PITCH_LIM	50.0

enum wt_cam_mode {
	WT_CAM_FREE,
	WT_CAM_FOCUS
};


struct wt_camera_info {
	f32 area_of_view;
	f32 aspect_ratio;
	f32 near;
	f32 far;	
};

struct wt_camera {
	struct wt_view *view;

	enum wt_cam_mode mode;

	wt_vec3_t pos;

	wt_vec3_t v_forward;
	wt_vec3_t v_right;

	struct wt_camera_info info;

	wt_vec3_t aim;
	f32 dist;
	wt_mat4_t forw_m;

	wt_mat4_t view_m;
	wt_mat4_t projection_m;

	f32 sens;

	/*
	 * FOCUS-MODE
	 * 
	 * With this mode, the camera will focus on a given target.
	 */

	wt_vec3_t target;		/* The point to focus on */
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
WT_API struct wt_camera *wt_CreateCamera(struct wt_camera_info info,
		struct wt_view *view);


/*
 * Destroy a camera and free the allocated memory.
 *
 * @cam: Pointer to the camera
 */
WT_API void wt_DestroyCamera(struct wt_camera *cam);


/*
 * Get the current view matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @cam: Pointer to the camera
 * @out: A matrix to write the view matrix to
 */
WT_API void wt_GetViewMat(struct wt_camera *cam, wt_mat4_t out);


/*
 * Get the current projection matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @cam: Pointer to the camera
 * @out: A matrix to write the projection matrix to
 */
WT_API void wt_GetProjectionMat(struct wt_camera *cam, wt_mat4_t out);


/*
 * Get the current position of a camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @out: A wt_vector to write the position to
 */
WT_API void wt_GetCameraPosition(struct wt_camera *cam, wt_vec3_t out);

/*
 * Set the position of a camera.
 *
 * @cam: Pointer to the camera
 * @pos: The new position of the camera
 */
WT_API void wt_SetCameraPosition(struct wt_camera *cam, wt_vec3_t pos);


/*
 * Move the camera by a given delta.
 *
 * @cam: Pointer to the camera
 * @del: The position difference to move the camera by
 */
WT_API void wt_MoveCamera(struct wt_camera *cam, wt_vec3_t del);


/*
 * Get the current direction of a camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @out: A wt_vector to write the direction to
 */
WT_API void wt_GetCameraDirection(struct wt_camera *cam, wt_vec3_t out);


/*
 * Set the direction of the camera. This function will then also update the
 * view-matrix.
 *
 * @cam: Pointer to the camera
 * @dir: The new direction of the camera
 */
WT_API void wt_SetCameraDirection(struct wt_camera *cam, wt_vec3_t dir);


/*
 * Get the current mode of the camera.
 *
 * @cam: Pointer to the camera
 *
 * Returns: The current mode of the camera
 */
WT_API enum wt_cam_mode wt_GetCameraMode(struct wt_camera *cam); 


/*
 * Set the mode of the camera.
 *
 * @cam: Pointer to the camera
 * @mode: The new mode of the camera
 */
WT_API void wt_SetCameraMode(struct wt_camera *cam, enum wt_cam_mode mode);


/*
 * Toggle the mode the camera.
 *
 * @cam: Pointer to the camera
 */
WT_API void wt_ToggleCameraMode(struct wt_camera *cam);


/*
 * Change the direction of the camera to look at the given point.
 * This will only work if the camera is in the Free-Mode.
 *
 * @cam: Pointer to the camera
 * @pnt: The point to look at
 */
WT_API void wt_CameraLookAt(struct wt_camera *cam, wt_vec3_t pnt);


/*
 * Switch the camera to focus mode and target a focus point.
 *
 * @cam: Pointer to the camera
 * @trg: The target point to focus on
 */
WT_API void wt_FocusCamera(struct wt_camera *cam, wt_vec3_t trg);


/*
 * Zoom by a given value.
 *
 * @cam: Pointer to the camera
 * @f: The value to zoom by
 */
WT_API void wt_CameraZoom(struct wt_camera *cam, f32 f);



WT_API void wt_CameraRotate(struct wt_camera *cam, f32 d_yaw, f32 d_pitch);


/*
 * Get the information about the camera.
 *
 * @cam: Pointer to the camera
 *
 * Returns: The info struct
 */
WT_API struct wt_camera_info wt_GetCameraInfo(struct wt_camera *cam);


/*
 * Set the information of the camera and recalculate the projection matrix.
 *
 * @cam: Pointer to the camera
 * @info: The information struct
 */
WT_API void wt_SetCameraInfo(struct wt_camera *cam, struct wt_camera_info info);

/*
 * Update the camera and recalculate the view matrix.
 *
 * @cam: Pointer to the camera
 */
WT_API void wt_UpdateCamera(struct wt_camera *cam);

#endif /* _WT_GRAPHIC_CAMERA_H */
