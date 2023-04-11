#ifndef _FH_CAMERA_H
#define _FH_CAMERA_H

#include "define.h"
#include "import.h"
#include "datatype.h"

#define FH_CAM_NAME_LIM		128
#define FH_CAM_PITCH_LIM	50.0

enum fh_cam_mode {
	FH_CAM_FOCUS,
	FH_CAM_WIDE
};


struct fh_camera {
	char name[FH_CAM_NAME_LIM];

	enum fh_cam_mode mode;

	vec3_t pos;

	vec3_t v_forward;
	vec3_t v_right;

	f32 aov;
	f32 asp;
	f32 near;
	f32 far;

	vec3_t aim;
	f32 dist;
	mat4_t forw_m;

	mat4_t proj_m;
	mat4_t view_m;

	f32 sens;
};


/*
 * Initialize the camera table.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_cam_init(void);


/*
 * Close the camera table and free the allocated memory.
 */
FH_API void fh_cam_close(void);


/*
 * Create a new camera.
 *
 * @name: The name of the camera
 * @aov: The angle-of-view
 * @asp: The aspect-ratio of the view
 * @near: The near-limit
 * @far: The far-limit
 *
 * Returns: Either a pointer to the newly created camera or NULL if an error
 * 	    occurred
 */
FH_API struct fh_camera *fh_cam_create(char *name, f32 aov, f32 asp, f32 near,
		f32 far);


/*
 * Destroy a camera.
 * 
 * @cam: Pointer to the camera
 */
FH_API void fh_cam_destroy(struct fh_camera *cam);


/*
 * Insert a newly created camera into the camera table.
 *
 * @cam: Pointer to the camera
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_cam_insert(struct fh_camera *cam);


/*
 * Remove a camera from the camera table.
 *
 * @cam: Pointer to the camera
 */
FH_API void fh_cam_remove(struct fh_camera *cam);


/*
 * Get the current projection-matrix of a camera.
 * If the camera if NULL, the matrix will become an Identity-Matrix.
 *
 * @cam: Pointer to the camera
 * @mat: A matrix to write the projection-matrix to
 */
FH_API void fh_cam_get_proj(struct fh_camera *cam, mat4_t mat);


/*
 * Get the current view-matrix of a camera.
 * If the camera if NULL, the matrix will become an Identity-Matrix.
 *
 * @cam: Pointer to the camera
 * @mat: A matrix to write the view-matrix to
 */
FH_API void fh_cam_get_view(struct fh_camera *cam, mat4_t mat);


/*
 * Set the position of the camera. This function will then also update the
 * view-matrix.
 *
 * @cam: Pointer to the camera
 * @pos: The new position of the fh_camera
 */
FH_API void fh_cam_set_pos(struct fh_camera *cam, vec3_t pos);


/*
 * Get the current position of the fh_camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @pos: A vector to write the current position to
 */
FH_API void fh_cam_get_pos(struct fh_camera *cam, vec3_t pos);


/*
 * Set the direction of the camera. This function will then also update the
 * view-matrix.
 *
 * @cam: Pointer to the camera
 * @dir: the new direction of the fh_camera
 */
FH_API void fh_cam_set_dir(struct fh_camera *cam, vec3_t dir);


/*
 * Get the current direction of the fh_camera.
 * If the camera is NULL, a Null-Vector will be returned.
 *
 * @cam: Pointer to the camera
 * @dir: A vector to write the current direction to
 */
FH_API void fh_cam_get_dir(struct fh_camera *cam, vec3_t dir);


/*
 * Change the distance of the fh_camera to the focused object. Note that this
 * function will only work, if the camera is focusing on a point.
 *
 * @cam: Pointer to the camera
 * @val: The cange in zoom
 */
FH_API void fh_cam_zoom(struct fh_camera *cam, s16 val);


/*
 * Change the rotation of the camera. This function will then update the
 * view-matrix of the camera.
 *
 * @cam: Pointer to the camera
 * @d_yaw: The delta-yaw value (up and down)
 * @d_pitch: The delta pitch value (left and right)
 */
FH_API void fh_cam_rot(struct fh_camera *cam, f32 d_yaw, f32 d_pitch);


/*
 * Move the _camera with a certain vector. This function will then update the
 * view-matrix.
 *
 * @cam: Pointer to the camera
 * @mov: The movement-vector the vector is going to be moved by
 */
FH_API void fh_cam_mov(struct fh_camera *cam, vec3_t mov);


/*
 * Focus the camera on a given position and recalculate the view-matrix.
 *
 * @cam: Pointer to the camera
 * @trg: The position to focus on
 */
FH_API void fh_cam_look_at(struct fh_camera *cam, vec3_t trg);


/*
 * Change the projection matrix and recalculate the projection-matrix with new
 * values.
 *
 * @cam: Pointer to the camera
 * @aov: The new angle-of-view
 * @asp: The new aspect-ation
 * @near: The new near-limit
 * @far: The new far-limit
 */
FH_API void fh_cam_proj_mat(struct fh_camera *cam, f32 aov, f32 asp, f32 near,
		f32 far);


/*
 * This function will recalculate the view-matrix of the camera.
 *
 * @cam: Pointer to the camera
 */
FH_API void fh_cam_update_view(struct fh_camera *cam);


/*
 * Set the camera-position and the position to focus the camera on.
 *
 * @cam: Pointer to the camera
 * @pos: The new position of the camera
 * @trg: The position to focus the camera on
 */
FH_API void fh_cam_set(struct fh_camera *cam, vec3_t pos, vec3_t trg);


/*
 * Get the current mode of the camera.
 *
 * @cam: Pointer to the camera
 *
 * Returns: The current mode of the camera
 */
FH_API enum fh_cam_mode fh_cam_get_mode(struct fh_camera *cam); 


/*
 * Set the mode of the camera.
 *
 * @cam: Pointer to the camera
 * @mode: The new mode of the camera
 */
FH_API void fh_cam_set_mode(struct fh_camera *cam, enum fh_cam_mode mode);


/*
 * Toggle the mode the camera.
 *
 * @cam: Pointer to the camera
 */
FH_API void fh_cam_tgl_mode(struct fh_camera *cam);


/*
 * Update the camera and recalculate the view matrix.
 *
 * @cam: Pointer to the camera
 */
FH_API void fh_cam_update(struct fh_camera *cam);


/* The callback function to call when removing an entry from the camera table */
FH_API void fh_cam_rmv_fnc(u32 size, void *ptr);

#endif /* _FH_CAMERA_H */
