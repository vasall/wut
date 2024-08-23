#ifndef _WUT_GRAPHIC_CAMERA_H
#define _WUT_GRAPHIC_CAMERA_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "window/inc/window.h"

#include "math/inc/functions.h"

#define WUT_CAM_NAME_LIM	128
#define WUT_CAM_PITCH_LIM	50.0


enum wut_eCameraMode {
	WUT_CAM_FREE,
	WUT_CAM_FOCUS
};


struct wut_CameraInfo {
	f32 area_of_view;
	f32 aspect_ratio;
	f32 near;
	f32 far;	
};


struct wut_Camera {
	struct wut_View *view;

	enum wut_eCameraMode mode;

	wut_Vec3 pos;

	wut_Vec3 v_forward;
	wut_Vec3 v_right;

	struct wut_CameraInfo info;

	wut_Vec3 aim;
	f32 dist;
	wut_Mat4 forw_m;

	wut_Mat4 view_m;
	wut_Mat4 projection_m;

	f32 sens;

	/*
	 * FOCUS-MODE
	 * 
	 * With this mode, the camera will focus on a given target.
	 */

	wut_Vec3 target;		/* The point to focus on */
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
WUT_API struct wut_Camera *wut_CreateCamera(struct wut_CameraInfo info,
		struct wut_View *view);


/*
 * Destroy a camera and free the allocated memory.
 *
 * @cam: Pointer to the camera
 */
WUT_API void wut_DestroyCamera(struct wut_Camera *cam);


/*
 * Get the current view matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @cam: Pointer to the camera
 * @out: A matrix to write the view matrix to
 */
WUT_API void wut_GetViewMat(struct wut_Camera *cam, wut_Mat4 out);


/*
 * Get the current projection matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @cam: Pointer to the camera
 * @out: A matrix to write the projection matrix to
 */
WUT_API void wut_GetProjectionMat(struct wut_Camera *cam, wut_Mat4 out);


/*
 * Get the current position of a camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @out: A wut_vector to write the position to
 */
WUT_API void wut_GetCameraPosition(struct wut_Camera *cam, wut_Vec3 out);

/*
 * Set the position of a camera.
 *
 * @cam: Pointer to the camera
 * @pos: The new position of the camera
 */
WUT_API void wut_SetCameraPosition(struct wut_Camera *cam, wut_Vec3 pos);


/*
 * Move the camera by a given delta.
 *
 * @cam: Pointer to the camera
 * @del: The position difference to move the camera by
 */
WUT_API void wut_MoveCamera(struct wut_Camera *cam, wut_Vec3 del);


/*
 * Get the current direction of a camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @out: A wut_vector to write the direction to
 */
WUT_API void wut_GetCameraDirection(struct wut_Camera *cam, wut_Vec3 out);


/*
 * Set the direction of the camera. This function will then also update the
 * view-matrix.
 *
 * @cam: Pointer to the camera
 * @dir: The new direction of the camera
 */
WUT_API void wut_SetCameraDirection(struct wut_Camera *cam, wut_Vec3 dir);


/*
 * Get the current mode of the camera.
 *
 * @cam: Pointer to the camera
 *
 * Returns: The current mode of the camera
 */
WUT_API enum wut_eCameraMode wut_GetCameraMode(struct wut_Camera *cam); 


/*
 * Set the mode of the camera.
 *
 * @cam: Pointer to the camera
 * @mode: The new mode of the camera
 */
WUT_API void wut_SetCameraMode(struct wut_Camera *cam, enum wut_eCameraMode mode);


/*
 * Toggle the mode the camera.
 *
 * @cam: Pointer to the camera
 */
WUT_API void wut_ToggleCameraMode(struct wut_Camera *cam);


/*
 * Change the direction of the camera to look at the given point.
 * This will only work if the camera is in the Free-Mode.
 *
 * @cam: Pointer to the camera
 * @pnt: The point to look at
 */
WUT_API void wut_CameraLookAt(struct wut_Camera *cam, wut_Vec3 pnt);


/*
 * Switch the camera to focus mode and target a focus point.
 *
 * @cam: Pointer to the camera
 * @trg: The target point to focus on
 */
WUT_API void wut_FocusCamera(struct wut_Camera *cam, wut_Vec3 trg);


/*
 * Zoom by a given value.
 *
 * @cam: Pointer to the camera
 * @f: The value to zoom by
 */
WUT_API void wut_CameraZoom(struct wut_Camera *cam, f32 f);



WUT_API void wut_CameraRotate(struct wut_Camera *cam, f32 d_yaw, f32 d_pitch);


/*
 * Get the information about the camera.
 *
 * @cam: Pointer to the camera
 *
 * Returns: The info struct
 */
WUT_API struct wut_CameraInfo wut_GetCameraInfo(struct wut_Camera *cam);


/*
 * Set the information of the camera and recalculate the projection matrix.
 *
 * @cam: Pointer to the camera
 * @info: The information struct
 */
WUT_API void wut_SetCameraInfo(struct wut_Camera *cam, struct wut_CameraInfo info);

/*
 * Update the camera and recalculate the view matrix.
 *
 * @cam: Pointer to the camera
 */
WUT_API void wut_UpdateCamera(struct wut_Camera *cam);

#endif /* _WUT_GRAPHIC_CAMERA_H */
