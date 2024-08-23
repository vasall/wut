#include "graphic/inc/camera.h"

#include "utility/inc/alarm.h"

#include "core/inc/core.h"

#include "system/inc/system.h"

#include "widget/inc/view.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



WUT_INTERN wut_Vec3 WUT_CAM_RIGHT =    {1.0, 0.0, 0.0};
WUT_INTERN wut_Vec3 WUT_CAM_FORWARD =  {0.0, 1.0, 0.0};
WUT_INTERN wut_Vec3 WUT_CAM_UP =       {0.0, 0.0, 1.0};



WUT_INTERN void cam_update_proj(struct wut_Camera *cam)
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

	wut_mat4_idt(cam->projection_m);

	cam->projection_m[0x0] = (2 * near) / (right - left);
	cam->projection_m[0x5] = (2 * near) / (top - bottom); 	
	cam->projection_m[0x8] = (right + left) / (right - left); 
	cam->projection_m[0x9] = (top + bottom) / (top - bottom); 
	cam->projection_m[0xa] = -(far + near) / (far - near); 
	cam->projection_m[0xb] = -1; 
	cam->projection_m[0xe] = (-2 * far * near) / (far - near); 
	cam->projection_m[0xf] = 0;
}


WUT_INTERN void cam_update_view(struct wut_Camera *cam)
{
	wut_Vec3 f;
	wut_Vec3 r;
	wut_Vec3 u;
	wut_Vec3 p;	

	wut_mat4_t m;

	wut_mat4_t conv = {
		1.0,  0.0,  0.0,  0.0,
   		0.0,  1.0,  0.0,  0.0,
   		0.0,  0.0, -1.0,  0.0,
		0.0,  0.0,  0.0,  1.0
	};

	/* Copy the current position of the camera */
	wut_vec3_cpy(p, cam->pos);

	/* Calculate the forward, right and up wut_vector for the camera */
	if(cam->mode == WUT_CAM_FOCUS) {
		wut_vec4_t tmp = {0, 1, 0};
		wut_vec4_trans(tmp, cam->forw_m, tmp);

		wut_vec3_cpy(f, tmp);
		wut_vec3_nrm(f, f);

		wut_vec3_cross(f, WUT_CAM_UP, r);
		wut_vec3_nrm(r, r);
	}
	else {
		wut_vec3_cpy(f, cam->v_forward);
		wut_vec3_cpy(r, cam->v_right);
	}

	wut_vec3_cross(r, f, u);
	wut_vec3_nrm(u, u);


	/*
	 * Calculate the view-matrix.
	 * See the link for how this works:
	 * https:/gamedev.stackexchange.com/a/181826
	 */

	wut_mat4_idt(m);

	m[0x0] = r[0];
	m[0x4] = r[1];
	m[0x8] = r[2];

	m[0x1] = u[0];
	m[0x5] = u[1];
	m[0x9] = u[2];

	m[0x2] = f[0];
	m[0x6] = f[1];
	m[0xa] = f[2];

	wut_mat4_mult(conv, m, m);

	wut_mat4_idt(cam->view_m);
	cam->view_m[0xc] = -p[0];
	cam->view_m[0xd] = -p[1];
	cam->view_m[0xe] = -p[2];

	wut_mat4_mult(m, cam->view_m, cam->view_m);
}


WUT_INTERN void cam_reorder_pipe(struct wut_Camera *cam)
{
	if(!cam->view)
		return;

	wut_UpdateViewPipe(cam->view);
}



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

WUT_API struct wut_Camera *wut_CreateCamera(struct wut_CameraInfo info,
		struct wut_view *view)
{
	struct wut_Camera *cam;

	if(!(cam = wut_malloc(sizeof(struct wut_camera)))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for camera");
		goto err_return;
	}

	cam->view = view;
	cam->mode = WUT_CAM_FREE;
	cam->info = info;

	/* Set the default position of the camera */
	wut_vec3_clr(cam->pos);

	/* Set the direction-wut_vector for the camera */	
	wut_vec3_cpy(cam->v_forward, WUT_CAM_FORWARD);
	wut_vec3_cpy(cam->v_right,   WUT_CAM_RIGHT);

	/* Set the sensitivity of the mouse */
	cam->sens = 0.01;

	/* Calculate the initial distance */
	cam->dist = wut_vec3_len(cam->pos);

	wut_mat4_idt(cam->forw_m);

	/* Calculate the projection- and view-matrix */
	cam_update_proj(cam);
	cam_update_view(cam);

	return cam;

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to create new camera");
	return NULL;
}


WUT_API void wut_DestroyCamera(struct wut_Camera *cam)
{	
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_free(cam);
}


WUT_API void wut_GetViewMat(struct wut_Camera *cam, wut_mat4_t out)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		wut_mat4_idt(out);
		return;
	}

	wut_mat4_cpy(out, cam->view_m);
}


WUT_API void wut_GetProjectionMat(struct wut_Camera *cam, wut_mat4_t out)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		wut_mat4_idt(out);
		return;
	}

	wut_mat4_cpy(out, cam->projection_m);
}


WUT_API void wut_GetCameraPosition(struct wut_Camera *cam, wut_Vec3 out)
{
	if(!cam) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		wut_vec3_clr(out);
		return;
	}

	wut_vec3_cpy(out, cam->pos);
}


WUT_API void wut_SetCameraPosition(struct wut_Camera *cam, wut_Vec3 pos)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_vec3_cpy(cam->pos, pos);

	cam_update_view(cam);

	cam_reorder_pipe(cam);
}


WUT_API void wut_MoveCamera(struct wut_Camera *cam, wut_Vec3 del)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_vec3_add(cam->pos, del, cam->pos);

	cam_update_view(cam);

	cam_reorder_pipe(cam);
}


WUT_API void wut_GetCameraDirection(struct wut_Camera *cam, wut_Vec3 out)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_vec3_cpy(out, cam->v_forward);
}


WUT_API void wut_SetCameraDirection(struct wut_Camera *cam, wut_Vec3 dir)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}
	
	wut_vec3_cpy(cam->v_forward, dir);
	wut_vec3_nrm(cam->v_forward, cam->v_forward);

	wut_vec3_cross(cam->v_forward, WUT_CAM_UP, cam->v_right);

	cam_update_view(cam);
}


WUT_API enum wut_cam_mode wut_GetCameraMode(struct wut_Camera *cam)
{
	if(!cam) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return 0;
	}

	return cam->mode;
}


WUT_API void wut_SetCameraMode(struct wut_Camera *cam, enum wut_cam_mode mode)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = mode;
}


WUT_API void wut_ToggleCameraMode(struct wut_Camera *cam)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = cam->mode == WUT_CAM_FOCUS ? WUT_CAM_FREE : WUT_CAM_FOCUS;
}


WUT_API void wut_CameraLookAt(struct wut_Camera *cam, wut_Vec3 pnt)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	wut_vec3_sub(pnt, cam->pos, cam->v_forward);
	wut_vec3_nrm(cam->v_forward, cam->v_forward);

	wut_vec3_cross(cam->v_forward, WUT_CAM_UP, cam->v_right);
	wut_vec3_nrm(cam->v_right, cam->v_right);
}


WUT_API void wut_FocusCamera(struct wut_Camera *cam, wut_Vec3 trg)
{
	if(cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	cam->mode = WUT_CAM_FOCUS;

	wut_vec3_cpy(cam->target, trg);

	wut_vec3_sub(trg, cam->pos, cam->v_forward);
	wut_vec3_nrm(cam->v_forward, cam->v_forward);

	wut_vec3_cross(cam->v_forward, WUT_CAM_UP, cam->v_right);
	wut_vec3_nrm(cam->v_right, cam->v_right);

	cam_update_view(cam);

}


WUT_API void wut_CameraZoom(struct wut_Camera *cam, f32 f)
{
	wut_Vec3 del;

	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	if(cam->mode == WUT_CAM_FOCUS) {
		cam->dist += f;
		if(cam->dist < 0.5)
			cam->dist = 0.5;
	}
	else {
		wut_vec3_scl(cam->v_forward, f, del);
		wut_vec3_add(cam->pos, del, cam->pos);
	}

	wut_UpdateCamera(cam);

	cam_reorder_pipe(cam);
}


WUT_API void wut_CameraRotate(struct wut_Camera *cam, f32 d_yaw, f32 d_pitch)
{
	d_yaw *= cam->sens;
	d_pitch *= cam->sens;

	if(d_yaw != 0.0) {
		wut_vec3_rot_z(cam->v_forward, d_yaw, cam->v_forward);
	}

	if(d_pitch != 0.0) {
		wut_vec3_rot_axes(cam->v_forward, d_pitch,
				cam->v_right, cam->v_forward);
	}

	/*
	 * Verify the pitch of the camera is in limits in FPV.
	 */
	if(cam->mode == WUT_CAM_FOCUS) {
		f32 agl;

		if(cam->v_forward[2] > 0) {	
			agl = asin(cam->v_forward[2]);
			agl = RAD_TO_DEG(agl);

			if(agl > WUT_CAM_PITCH_LIM)
				agl = WUT_CAM_PITCH_LIM;

			agl = DEG_TO_RAD(agl);
			cam->v_forward[2] = sin(agl);
		}
		else {
			agl = asin(cam->v_forward[2]);
			agl = RAD_TO_DEG(agl);

			if(agl < -WUT_CAM_PITCH_LIM)
				agl = -WUT_CAM_PITCH_LIM;

			agl = DEG_TO_RAD(agl);
			cam->v_forward[2] = sin(agl);
		}
	}

	wut_vec3_nrm(cam->v_forward, cam->v_forward);

	wut_vec3_cross(cam->v_forward, WUT_CAM_UP, cam->v_right);
	wut_vec3_nrm(cam->v_right, cam->v_right);

	/*
	 * Calculate the forward-rotation-matrix.
	 */
	if(cam->mode == WUT_CAM_FOCUS) {
		f32 yaw;
		f32 pitch;
		wut_Vec3 f;

		wut_vec3_cpy(f, cam->v_forward);

		yaw = atan2(f[0], f[1]);
		pitch = -asin(f[2]);

		yaw = RAD_TO_DEG(yaw);
		pitch = RAD_TO_DEG(pitch);

		wut_mat4_rfagl_s(cam->forw_m, pitch, 0, yaw);
	}
}


WUT_API struct wut_CameraInfo wut_GetCameraInfo(struct wut_Camera *cam)
{
	if(!cam) {
		struct wut_CameraInfo info = {0, 0, 0, 0};
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		return info;
	}

	return cam->info;
}


WUT_API void wut_SetCameraInfo(struct wut_Camera *cam, struct wut_CameraInfo info)
{
	if(!cam) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		return;
	}

	/* Overwrite the info struct */
	cam->info = info;

	/* And recalculate the projection matrix */
	cam_update_proj(cam);
}


WUT_API void wut_UpdateCamera(struct wut_Camera *cam)
{
	if(!cam)
		return;

	cam_update_view(cam);
}
