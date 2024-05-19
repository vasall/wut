#include "graphic/inc/camera.h"

#include "utility/inc/alarm.h"

#include "core/inc/core.h"

#include "system/inc/system.h"

#include "widget/inc/view.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



WT_INTERN wt_vec3_t WT_CAM_RIGHT =    {1.0, 0.0, 0.0};
WT_INTERN wt_vec3_t WT_CAM_FORWARD =  {0.0, 1.0, 0.0};
WT_INTERN wt_vec3_t WT_CAM_UP =       {0.0, 0.0, 1.0};



WT_INTERN void cam_update_proj(struct wt_camera *cam)
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

	wt_mat4_idt(cam->projection_m);

	cam->projection_m[0x0] = (2 * near) / (right - left);
	cam->projection_m[0x5] = (2 * near) / (top - bottom); 	
	cam->projection_m[0x8] = (right + left) / (right - left); 
	cam->projection_m[0x9] = (top + bottom) / (top - bottom); 
	cam->projection_m[0xa] = -(far + near) / (far - near); 
	cam->projection_m[0xb] = -1; 
	cam->projection_m[0xe] = (-2 * far * near) / (far - near); 
	cam->projection_m[0xf] = 0;
}


WT_INTERN void cam_update_view(struct wt_camera *cam)
{
	wt_vec3_t f;
	wt_vec3_t r;
	wt_vec3_t u;
	wt_vec3_t p;	

	wt_mat4_t m;

	wt_mat4_t conv = {
		1.0,  0.0,  0.0,  0.0,
   		0.0,  1.0,  0.0,  0.0,
   		0.0,  0.0, -1.0,  0.0,
		0.0,  0.0,  0.0,  1.0
	};

	/* Copy the current position of the camera */
	wt_vec3_cpy(p, cam->pos);

	/* Calculate the forward, right and up wt_vector for the camera */
	if(cam->mode == WT_CAM_FOCUS) {
		wt_vec4_t tmp = {0, 1, 0};
		wt_vec4_trans(tmp, cam->forw_m, tmp);

		wt_vec3_cpy(f, tmp);
		wt_vec3_nrm(f, f);

		wt_vec3_cross(f, WT_CAM_UP, r);
		wt_vec3_nrm(r, r);
	}
	else {
		wt_vec3_cpy(f, cam->v_forward);
		wt_vec3_cpy(r, cam->v_right);
	}

	wt_vec3_cross(r, f, u);
	wt_vec3_nrm(u, u);


	/*
	 * Calculate the view-matrix.
	 * See the link for how this works:
	 * https:/gamedev.stackexchange.com/a/181826
	 */

	wt_mat4_idt(m);

	m[0x0] = r[0];
	m[0x4] = r[1];
	m[0x8] = r[2];

	m[0x1] = u[0];
	m[0x5] = u[1];
	m[0x9] = u[2];

	m[0x2] = f[0];
	m[0x6] = f[1];
	m[0xa] = f[2];

	wt_mat4_mult(conv, m, m);

	wt_mat4_idt(cam->view_m);
	cam->view_m[0xc] = -p[0];
	cam->view_m[0xd] = -p[1];
	cam->view_m[0xe] = -p[2];

	wt_mat4_mult(m, cam->view_m, cam->view_m);
}


WT_INTERN void cam_reorder_pipe(struct wt_camera *cam)
{
	if(!cam->view)
		return;

	wt_UpdateViewPipe(cam->view);
}



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WT_API struct wt_camera *wt_CreateCamera(struct wt_camera_info info,
		struct wt_view *view)
{
	struct wt_camera *cam;

	if(!(cam = wt_malloc(sizeof(struct wt_camera)))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for camera");
		goto err_return;
	}

	cam->view = view;
	cam->mode = WT_CAM_FREE;
	cam->info = info;

	/* Set the default position of the camera */
	wt_vec3_clr(cam->pos);

	/* Set the direction-wt_vector for the camera */	
	wt_vec3_cpy(cam->v_forward, WT_CAM_FORWARD);
	wt_vec3_cpy(cam->v_right,   WT_CAM_RIGHT);

	/* Set the sensitivity of the mouse */
	cam->sens = 0.01;

	/* Calculate the initial distance */
	cam->dist = wt_vec3_len(cam->pos);

	wt_mat4_idt(cam->forw_m);

	/* Calculate the projection- and view-matrix */
	cam_update_proj(cam);
	cam_update_view(cam);

	return cam;

err_return:
	WT_ALARM(WT_ERROR, "Failed to create new camera");
	return NULL;
}


WT_API void wt_DestroyCamera(struct wt_camera *cam)
{	
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_free(cam);
}


WT_API void wt_GetViewMat(struct wt_camera *cam, wt_mat4_t out)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		wt_mat4_idt(out);
		return;
	}

	wt_mat4_cpy(out, cam->view_m);
}


WT_API void wt_GetProjectionMat(struct wt_camera *cam, wt_mat4_t out)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		wt_mat4_idt(out);
		return;
	}

	wt_mat4_cpy(out, cam->projection_m);
}


WT_API void wt_GetCameraPosition(struct wt_camera *cam, wt_vec3_t out)
{
	if(!cam) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		wt_vec3_clr(out);
		return;
	}

	wt_vec3_cpy(out, cam->pos);
}


WT_API void wt_SetCameraPosition(struct wt_camera *cam, wt_vec3_t pos)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_vec3_cpy(cam->pos, pos);

	cam_update_view(cam);

	cam_reorder_pipe(cam);
}


WT_API void wt_MoveCamera(struct wt_camera *cam, wt_vec3_t del)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_vec3_add(cam->pos, del, cam->pos);

	cam_update_view(cam);

	cam_reorder_pipe(cam);
}


WT_API void wt_GetCameraDirection(struct wt_camera *cam, wt_vec3_t out)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_vec3_cpy(out, cam->v_forward);
}


WT_API void wt_SetCameraDirection(struct wt_camera *cam, wt_vec3_t dir)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}
	
	wt_vec3_cpy(cam->v_forward, dir);
	wt_vec3_nrm(cam->v_forward, cam->v_forward);

	wt_vec3_cross(cam->v_forward, WT_CAM_UP, cam->v_right);

	cam_update_view(cam);
}


WT_API enum wt_cam_mode wt_GetCameraMode(struct wt_camera *cam)
{
	if(!cam) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return 0;
	}

	return cam->mode;
}


WT_API void wt_SetCameraMode(struct wt_camera *cam, enum wt_cam_mode mode)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = mode;
}


WT_API void wt_ToggleCameraMode(struct wt_camera *cam)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = cam->mode == WT_CAM_FOCUS ? WT_CAM_FREE : WT_CAM_FOCUS;
}


WT_API void wt_CameraLookAt(struct wt_camera *cam, wt_vec3_t pnt)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	wt_vec3_sub(pnt, cam->pos, cam->v_forward);
	wt_vec3_nrm(cam->v_forward, cam->v_forward);

	wt_vec3_cross(cam->v_forward, WT_CAM_UP, cam->v_right);
	wt_vec3_nrm(cam->v_right, cam->v_right);
}


WT_API void wt_FocusCamera(struct wt_camera *cam, wt_vec3_t trg)
{
	if(cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = WT_CAM_FOCUS;

	wt_vec3_cpy(cam->target, trg);

	wt_vec3_sub(trg, cam->pos, cam->v_forward);
	wt_vec3_nrm(cam->v_forward, cam->v_forward);

	wt_vec3_cross(cam->v_forward, WT_CAM_UP, cam->v_right);
	wt_vec3_nrm(cam->v_right, cam->v_right);

	cam_update_view(cam);

}


WT_API void wt_CameraZoom(struct wt_camera *cam, f32 f)
{
	wt_vec3_t del;

	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	if(cam->mode == WT_CAM_FOCUS) {
		cam->dist += f;
		if(cam->dist < 0.5)
			cam->dist = 0.5;
	}
	else {
		wt_vec3_scl(cam->v_forward, f, del);
		wt_vec3_add(cam->pos, del, cam->pos);
	}

	wt_UpdateCamera(cam);

	cam_reorder_pipe(cam);
}


WT_API void wt_CameraRotate(struct wt_camera *cam, f32 d_yaw, f32 d_pitch)
{
	d_yaw *= cam->sens;
	d_pitch *= cam->sens;

	if(d_yaw != 0.0) {
		wt_vec3_rot_z(cam->v_forward, d_yaw, cam->v_forward);
	}

	if(d_pitch != 0.0) {
		wt_vec3_rot_axes(cam->v_forward, d_pitch,
				cam->v_right, cam->v_forward);
	}

	/*
	 * Verify the pitch of the camera is in limits in FPV.
	 */
	if(cam->mode == WT_CAM_FOCUS) {
		f32 agl;

		if(cam->v_forward[2] > 0) {	
			agl = asin(cam->v_forward[2]);
			agl = RAD_TO_DEG(agl);

			if(agl > WT_CAM_PITCH_LIM)
				agl = WT_CAM_PITCH_LIM;

			agl = DEG_TO_RAD(agl);
			cam->v_forward[2] = sin(agl);
		}
		else {
			agl = asin(cam->v_forward[2]);
			agl = RAD_TO_DEG(agl);

			if(agl < -WT_CAM_PITCH_LIM)
				agl = -WT_CAM_PITCH_LIM;

			agl = DEG_TO_RAD(agl);
			cam->v_forward[2] = sin(agl);
		}
	}

	wt_vec3_nrm(cam->v_forward, cam->v_forward);

	wt_vec3_cross(cam->v_forward, WT_CAM_UP, cam->v_right);
	wt_vec3_nrm(cam->v_right, cam->v_right);

	/*
	 * Calculate the forward-rotation-matrix.
	 */
	if(cam->mode == WT_CAM_FOCUS) {
		f32 yaw;
		f32 pitch;
		wt_vec3_t f;

		wt_vec3_cpy(f, cam->v_forward);

		yaw = atan2(f[0], f[1]);
		pitch = -asin(f[2]);

		yaw = RAD_TO_DEG(yaw);
		pitch = RAD_TO_DEG(pitch);

		wt_mat4_rfagl_s(cam->forw_m, pitch, 0, yaw);
	}
}


WT_API struct wt_camera_info wt_GetCameraInfo(struct wt_camera *cam)
{
	if(!cam) {
		struct wt_camera_info info = {0, 0, 0, 0};
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		return info;
	}

	return cam->info;
}


WT_API void wt_SetCameraInfo(struct wt_camera *cam, struct wt_camera_info info)
{
	if(!cam) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		return;
	}

	/* Overwrite the info struct */
	cam->info = info;

	/* And recalculate the projection matrix */
	cam_update_proj(cam);
}


WT_API void wt_UpdateCamera(struct wt_camera *cam)
{
	if(!cam)
		return;

	cam_update_view(cam);
}
