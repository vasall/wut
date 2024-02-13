#include "graphic/inc/camera.h"

#include "utility/inc/alarm.h"

#include "core/inc/core.h"

#include "system/inc/system.h"

#include "widget/inc/view.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



FH_INTERN fh_vec3_t FH_CAM_RIGHT =    {1.0, 0.0, 0.0};
FH_INTERN fh_vec3_t FH_CAM_FORWARD =  {0.0, 1.0, 0.0};
FH_INTERN fh_vec3_t FH_CAM_UP =       {0.0, 0.0, 1.0};



FH_INTERN void cam_update_proj(struct fh_camera *cam)
{
	f32 aov;
	f32 asp;
	f32 near;
	f32 far;
	f32 bottom;
	f32 top;
	f32 left;
	f32 right;
	f32 tangent;

	aov = cam->info.area_of_view;
	asp = cam->info.aspect_ratio;
	near = cam->info.near;
	far = cam->info.far;

	tangent = near * tan(aov * 0.5 * M_PI / 180);

	top = tangent;
	bottom = -top;
	right = top * asp;
	left = -right; 

	fh_mat4_idt(cam->projection_m);

	cam->projection_m[0x0] = (2 * near) / (right - left);
	cam->projection_m[0x5] = (2 * near) / (top - bottom); 	
	cam->projection_m[0x8] = (right + left) / (right - left); 
	cam->projection_m[0x9] = (top + bottom) / (top - bottom); 
	cam->projection_m[0xa] = -(far + near) / (far - near); 
	cam->projection_m[0xb] = -1; 
	cam->projection_m[0xe] = (-2 * far * near) / (far - near); 
	cam->projection_m[0xf] = 0;
}


FH_INTERN void cam_update_view(struct fh_camera *cam)
{
	fh_vec3_t f;
	fh_vec3_t r;
	fh_vec3_t u;
	fh_vec3_t p;	

	fh_mat4_t m;

	fh_mat4_t conv = {
		1.0,  0.0,  0.0,  0.0,
   		0.0,  1.0,  0.0,  0.0,
   		0.0,  0.0, -1.0,  0.0,
		0.0,  0.0,  0.0,  1.0
	};

	/* Copy the current position of the camera */
	fh_vec3_cpy(p, cam->pos);

	/* Calculate the forward, right and up fh_vector for the camera */
	if(cam->mode == FH_CAM_FOCUS) {
		fh_vec4_t tmp = {0, 1, 0};
		fh_vec4_trans(tmp, cam->forw_m, tmp);

		fh_vec3_cpy(f, tmp);
		fh_vec3_nrm(f, f);

		fh_vec3_cross(f, FH_CAM_UP, r);
		fh_vec3_nrm(r, r);
	}
	else {
		fh_vec3_cpy(f, cam->v_forward);
		fh_vec3_cpy(r, cam->v_right);
	}

	fh_vec3_cross(r, f, u);
	fh_vec3_nrm(u, u);


	/*
	 * Calculate the view-matrix.
	 * See the link for how this works:
	 * https:/gamedev.stackexchange.com/a/181826
	 */

	fh_mat4_idt(m);

	m[0x0] = r[0];
	m[0x4] = r[1];
	m[0x8] = r[2];

	m[0x1] = u[0];
	m[0x5] = u[1];
	m[0x9] = u[2];

	m[0x2] = f[0];
	m[0x6] = f[1];
	m[0xa] = f[2];

	fh_mat4_mult(conv, m, m);

	fh_mat4_idt(cam->view_m);
	cam->view_m[0xc] = -p[0];
	cam->view_m[0xd] = -p[1];
	cam->view_m[0xe] = -p[2];

	fh_mat4_mult(m, cam->view_m, cam->view_m);
}


FH_INTERN void cam_reorder_pipe(struct fh_camera *cam)
{
	if(!cam->view)
		return;

	fh_UpdateViewPipe(cam->view);
}



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_camera *fh_CreateCamera(struct fh_camera_info info,
		struct fh_view *view)
{
	struct fh_camera *cam;

	if(!(cam = fh_malloc(sizeof(struct fh_camera)))) {
		FH_ALARM(FH_ERROR, "Failed to allocate memory for camera");
		goto err_return;
	}

	cam->view = view;
	cam->mode = FH_CAM_FREE;
	cam->info = info;

	/* Set the default position of the camera */
	fh_vec3_clr(cam->pos);

	/* Set the direction-fh_vector for the camera */	
	fh_vec3_cpy(cam->v_forward, FH_CAM_FORWARD);
	fh_vec3_cpy(cam->v_right,   FH_CAM_RIGHT);

	/* Set the sensitivity of the mouse */
	cam->sens = 0.01;

	/* Calculate the initial distance */
	cam->dist = fh_vec3_len(cam->pos);

	fh_mat4_idt(cam->forw_m);

	/* Calculate the projection- and view-matrix */
	cam_update_proj(cam);
	cam_update_view(cam);

	return cam;

err_return:
	FH_ALARM(FH_ERROR, "Failed to create new camera");
	return NULL;
}


FH_API void fh_DestroyCamera(struct fh_camera *cam)
{	
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_free(cam);
}


FH_API void fh_GetViewMat(struct fh_camera *cam, fh_mat4_t out)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		fh_mat4_idt(out);
		return;
	}

	fh_mat4_cpy(out, cam->view_m);
}


FH_API void fh_GetProjectionMat(struct fh_camera *cam, fh_mat4_t out)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		fh_mat4_idt(out);
		return;
	}

	fh_mat4_cpy(out, cam->projection_m);
}


FH_API void fh_GetCameraPosition(struct fh_camera *cam, fh_vec3_t out)
{
	if(!cam) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		fh_vec3_clr(out);
		return;
	}

	fh_vec3_cpy(out, cam->pos);
}


FH_API void fh_SetCameraPosition(struct fh_camera *cam, fh_vec3_t pos)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_vec3_cpy(cam->pos, pos);

	cam_update_view(cam);

	cam_reorder_pipe(cam);
}


FH_API void fh_MoveCamera(struct fh_camera *cam, fh_vec3_t del)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_vec3_add(cam->pos, del, cam->pos);

	cam_update_view(cam);

	cam_reorder_pipe(cam);
}


FH_API void fh_GetCameraDirection(struct fh_camera *cam, fh_vec3_t out)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_vec3_cpy(out, cam->v_forward);
}


FH_API void fh_SetCameraDirection(struct fh_camera *cam, fh_vec3_t dir)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}
	
	fh_vec3_cpy(cam->v_forward, dir);
	fh_vec3_nrm(cam->v_forward, cam->v_forward);

	fh_vec3_cross(cam->v_forward, FH_CAM_UP, cam->v_right);

	cam_update_view(cam);
}


FH_API enum fh_cam_mode fh_GetCameraMode(struct fh_camera *cam)
{
	if(!cam) {
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return 0;
	}

	return cam->mode;
}


FH_API void fh_SetCameraMode(struct fh_camera *cam, enum fh_cam_mode mode)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = mode;
}


FH_API void fh_ToggleCameraMode(struct fh_camera *cam)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = cam->mode == FH_CAM_FOCUS ? FH_CAM_FREE : FH_CAM_FOCUS;
}


FH_API void fh_CameraLookAt(struct fh_camera *cam, fh_vec3_t pnt)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	fh_vec3_sub(pnt, cam->pos, cam->v_forward);
	fh_vec3_nrm(cam->v_forward, cam->v_forward);

	fh_vec3_cross(cam->v_forward, FH_CAM_UP, cam->v_right);
	fh_vec3_nrm(cam->v_right, cam->v_right);
}


FH_API void fh_FocusCamera(struct fh_camera *cam, fh_vec3_t trg)
{
	if(cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = FH_CAM_FOCUS;

	fh_vec3_cpy(cam->target, trg);

	fh_vec3_sub(trg, cam->pos, cam->v_forward);
	fh_vec3_nrm(cam->v_forward, cam->v_forward);

	fh_vec3_cross(cam->v_forward, FH_CAM_UP, cam->v_right);
	fh_vec3_nrm(cam->v_right, cam->v_right);

	cam_update_view(cam);

}


FH_API void fh_CameraZoom(struct fh_camera *cam, f32 f)
{
	fh_vec3_t del;

	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	if(cam->mode == FH_CAM_FOCUS) {
		cam->dist += f;
		if(cam->dist < 0.5)
			cam->dist = 0.5;
	}
	else {
		fh_vec3_scl(cam->v_forward, f, del);
		fh_vec3_add(cam->pos, del, cam->pos);
	}

	fh_UpdateCamera(cam);

	cam_reorder_pipe(cam);
}


FH_API void fh_CameraRotate(struct fh_camera *cam, f32 d_yaw, f32 d_pitch)
{
	d_yaw *= cam->sens;
	d_pitch *= cam->sens;

	if(d_yaw != 0.0) {
		fh_vec3_rot_z(cam->v_forward, d_yaw, cam->v_forward);
	}

	if(d_pitch != 0.0) {
		fh_vec3_rot_axes(cam->v_forward, d_pitch,
				cam->v_right, cam->v_forward);
	}

	/*
	 * Verify the pitch of the camera is in limits in FPV.
	 */
	if(cam->mode == FH_CAM_FOCUS) {
		f32 agl;

		if(cam->v_forward[2] > 0) {	
			agl = asin(cam->v_forward[2]);
			agl = RAD_TO_DEG(agl);

			if(agl > FH_CAM_PITCH_LIM)
				agl = FH_CAM_PITCH_LIM;

			agl = DEG_TO_RAD(agl);
			cam->v_forward[2] = sin(agl);
		}
		else {
			agl = asin(cam->v_forward[2]);
			agl = RAD_TO_DEG(agl);

			if(agl < -FH_CAM_PITCH_LIM)
				agl = -FH_CAM_PITCH_LIM;

			agl = DEG_TO_RAD(agl);
			cam->v_forward[2] = sin(agl);
		}
	}

	fh_vec3_nrm(cam->v_forward, cam->v_forward);

	fh_vec3_cross(cam->v_forward, FH_CAM_UP, cam->v_right);
	fh_vec3_nrm(cam->v_right, cam->v_right);

	/*
	 * Calculate the forward-rotation-matrix.
	 */
	if(cam->mode == FH_CAM_FOCUS) {
		f32 yaw;
		f32 pitch;
		fh_vec3_t f;

		fh_vec3_cpy(f, cam->v_forward);

		yaw = atan2(f[0], f[1]);
		pitch = -asin(f[2]);

		yaw = RAD_TO_DEG(yaw);
		pitch = RAD_TO_DEG(pitch);

		fh_mat4_rfagl_s(cam->forw_m, pitch, 0, yaw);
	}
}


FH_API struct fh_camera_info fh_GetCameraInfo(struct fh_camera *cam)
{
	if(!cam) {
		struct fh_camera_info info = {0, 0, 0, 0};
		FH_ALARM(FH_ERROR, "Input parameters invalid");
		return info;
	}

	return cam->info;
}


FH_API void fh_SetCameraInfo(struct fh_camera *cam, struct fh_camera_info info)
{
	if(!cam) {
		FH_ALARM(FH_WARNING, "Input parameters invalid");
		return;
	}

	/* Overwrite the info struct */
	cam->info = info;

	/* And recalculate the projection matrix */
	cam_update_proj(cam);
}


FH_API void fh_UpdateCamera(struct fh_camera *cam)
{
	if(!cam)
		return;

	cam_update_view(cam);
}
