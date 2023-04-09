#include "camera.h"

#include "alarm.h"
#include "system.h"
#include "core.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



FH_INTERN vec3_t FH_CAM_RIGHT =    {1.0, 0.0, 0.0};
FH_INTERN vec3_t FH_CAM_FORWARD =  {0.0, 1.0, 0.0};
FH_INTERN vec3_t FH_CAM_UP =       {0.0, 0.0, 1.0};


FH_API s8 fh_cam_init(void)
{
	struct fh_table *tbl;

	if(!(tbl = fh_tbl_create(&fh_cam_rmv_fnc))) {
		ALARM(ALARM_ERR, "Failed to create camera table");
		goto err_return;
	}

	g_fh_core.cameras = tbl;
	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to initialize camera table");
	return -1;
}


FH_API void fh_cam_close(void)
{
	fh_tbl_destroy(g_fh_core.cameras);
	g_fh_core.cameras = NULL;
}


FH_API struct fh_camera *fh_cam_create(char *name, f32 aov, f32 asp, f32 near,
		f32 far)
{
	struct fh_camera *cam;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(cam = fh_malloc(sizeof(struct fh_camera)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for camera");
		goto err_return;
	}

	strcpy(cam->name, name);

	/* Set the default position of the camera */
	vec3_clr(cam->pos);

	/* Set the direction-vector for the camera */	
	vec3_cpy(cam->v_forward, FH_CAM_FORWARD);
	vec3_cpy(cam->v_right,   FH_CAM_RIGHT);

	/* Set the sensitivity of the mouse */
	cam->sens = 0.01;

	/* Calculate the initial distance */
	cam->dist = vec3_len(cam->pos);

	mat4_idt(cam->forw_m);

	/* Create the proj_mection matrix */
	mat4_idt(cam->proj_m);
	fh_cam_proj_mat(cam, aov, asp, near, far);

	/* Create the view-matrix */
	mat4_idt(cam->view_m);
	fh_cam_update_view(cam);

	return cam;

err_return:
	ALARM(ALARM_ERR, "Failed to create a new camera");
	return NULL;
}


FH_API void fh_cam_destroy(struct fh_camera *cam)
{
	if(!cam)
		return;

	fh_free(cam);
}


FH_API s8 fh_cam_insert(struct fh_camera *cam)
{
	u32 size;
	void **p;

	if(!cam) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	size = sizeof(struct fh_camera);
	p = (void **)&cam;
	if(fh_tbl_add(g_fh_core.cameras, cam->name, size, p) < 0) {
		ALARM(ALARM_ERR, "Failed to insert into fh_table");
		goto err_return;
	}

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to insert texture into texture table");
	return -1;
}


FH_API void fh_cam_remove(struct fh_camera *cam)
{
	if(!cam)
		return;

	fh_tbl_rmv(g_fh_core.cameras, cam->name);
}


FH_API void fh_cam_get_proj(struct fh_camera *cam, mat4_t mat)
{
	if(!cam) {
		mat4_idt(mat);
		return;
	}

	mat4_cpy(mat, cam->proj_m);
}


FH_API void fh_cam_get_view(struct fh_camera *cam, mat4_t mat)
{
	if(!cam) {
		mat4_idt(mat);
		return;
	}

	mat4_cpy(mat, cam->view_m);
}


FH_API void fh_cam_set_pos(struct fh_camera *cam, vec3_t pos)
{
	if(!cam)
		return;

	vec3_cpy(cam->pos, pos);
}


FH_API void fh_cam_get_pos(struct fh_camera *cam, vec3_t pos)
{
	if(!cam) {
		vec3_clr(pos);
		return;
	}

	vec3_cpy(pos, cam->pos);
}


FH_API void fh_cam_set_dir(struct fh_camera *cam, vec3_t dir)
{
	vec3_cpy(cam->v_forward, dir);
	vec3_nrm(cam->v_forward, cam->v_forward);

	vec3_cross(cam->v_forward, FH_CAM_UP, cam->v_right);

	fh_cam_update_view(cam);
}


FH_API void fh_cam_get_dir(struct fh_camera *cam, vec3_t dir)
{
	if(!cam) {
		vec3_clr(dir);
		return;
	}

	vec3_cpy(dir, cam->v_forward);
}


FH_API void fh_cam_zoom(struct fh_camera *cam, s16 val)
{
	cam->dist += val;
	if(cam->dist < 0.5)
		cam->dist = 0.5;

	fh_cam_update(cam);
}


FH_API void fh_cam_rot(struct fh_camera *cam, f32 d_yaw, f32 d_pitch)
{
	d_yaw *= cam->sens;
	d_pitch *= cam->sens;

	if(d_yaw != 0.0) {
		vec3_rot_z(cam->v_forward, d_yaw, cam->v_forward);
	}

	if(d_pitch != 0.0) {
		vec3_rot_axes(cam->v_forward, d_pitch,
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

	vec3_nrm(cam->v_forward, cam->v_forward);

	vec3_cross(cam->v_forward, FH_CAM_UP, cam->v_right);
	vec3_nrm(cam->v_right, cam->v_right);

	/*
	 * Calculate the forward-rotation-matrix.
	 */
	if(cam->mode == FH_CAM_FOCUS) {
		f32 yaw;
		f32 pitch;
		vec3_t f;

		vec3_cpy(f, cam->v_forward);

		yaw = atan2(f[0], f[1]);
		pitch = -asin(f[2]);

		yaw = RAD_TO_DEG(yaw);
		pitch = RAD_TO_DEG(pitch);

		mat4_rfagl_s(cam->forw_m, pitch, 0, yaw);
	}
}


FH_API void fh_cam_mov(struct fh_camera *cam, vec3_t mov)
{
	vec3_add(cam->pos, mov, cam->pos);
	fh_cam_update_view(cam);
}


FH_API void fh_cam_look_at(struct fh_camera *cam, vec3_t trg)
{
	cam->mode = FH_CAM_FOCUS;

	vec3_cpy(cam->aim, trg);

	vec3_sub(trg, cam->pos, cam->v_forward);
	vec3_nrm(cam->v_forward, cam->v_forward);

	vec3_cross(cam->v_forward, FH_CAM_UP, cam->v_right);
	vec3_nrm(cam->v_right, cam->v_right);
}


FH_API void fh_cam_proj_mat(struct fh_camera *cam, f32 aov, f32 asp, f32 near,
		f32 far)
{
	f32 bottom;
	f32 top;
	f32 left;
	f32 right;
	f32 tangent;

	cam->aov = aov;
	cam->asp = asp;
	cam->near = near;
	cam->far = far;

	tangent = near * tan(aov * 0.5 * M_PI / 180);

	top = tangent;
	bottom = -top;
	right = top * asp;
	left = -right; 

	cam->proj_m[0x0] = (2 * near) / (right - left);
	cam->proj_m[0x5] = (2 * near) / (top - bottom); 	
	cam->proj_m[0x8] = (right + left) / (right - left); 
	cam->proj_m[0x9] = (top + bottom) / (top - bottom); 
	cam->proj_m[0xa] = -(far + near) / (far - near); 
	cam->proj_m[0xb] = -1; 
	cam->proj_m[0xe] = (-2 * far * near) / (far - near); 
	cam->proj_m[0xf] = 0;
}


FH_API void fh_cam_update_view(struct fh_camera *cam)
{
	vec3_t f;
	vec3_t r;
	vec3_t u;
	vec3_t p;	

	mat4_t m;

	mat4_t conv = {
		1.0,  0.0,  0.0,  0.0,
   		0.0,  1.0,  0.0,  0.0,
   		0.0,  0.0, -1.0,  0.0,
		0.0,  0.0,  0.0,  1.0
	};

	/* Copy the current position of the camera */
	vec3_cpy(p, cam->pos);

	/* Calculate the forward, right and up vector for the camera */
	if(cam->mode == FH_CAM_FOCUS) {
		vec4_t tmp = {0, 1, 0};
		vec4_trans(tmp, cam->forw_m, tmp);

		vec3_cpy(f, tmp);
		vec3_nrm(f, f);

		vec3_cross(f, FH_CAM_UP, r);
		vec3_nrm(r, r);
	}
	else {
		vec3_cpy(f, cam->v_forward);
		vec3_cpy(r, cam->v_right);
	}

	vec3_cross(r, f, u);
	vec3_nrm(u, u);


#if 0
	printf("VIEW\n");
	printf("forward: "); vec3_print(f); printf("\n");
	printf("right: ");   vec3_print(r); printf("\n");
	printf("up: ");      vec3_print(u); printf("\n");
	printf("pos: ");     vec3_print(p); printf("\n");
#endif

	/*
	 * Calculate the view-matrix.
	 * See the link for how this works:
	 * https://gamedev.stackexchange.com/a/181826
	 */

	mat4_idt(m);

	m[0x0] = r[0];
	m[0x4] = r[1];
	m[0x8] = r[2];

	m[0x1] = u[0];
	m[0x5] = u[1];
	m[0x9] = u[2];

	m[0x2] = f[0];
	m[0x6] = f[1];
	m[0xa] = f[2];

	mat4_mult(conv, m, m);

	mat4_idt(cam->view_m);
	cam->view_m[0xc] = -p[0];
	cam->view_m[0xd] = -p[1];
	cam->view_m[0xe] = -p[2];

	mat4_mult(m, cam->view_m, cam->view_m);
}


FH_API void fh_cam_set(struct fh_camera *cam, vec3_t pos, vec3_t trg)
{	
	vec3_cpy(cam->pos, pos);

	vec3_sub(pos, trg, cam->v_forward);
	vec3_nrm(cam->v_forward, cam->v_forward);

	vec3_cross(FH_CAM_UP, cam->v_forward, cam->v_right);
	vec3_nrm(cam->v_right, cam->v_right);

	fh_cam_update_view(cam);
}


FH_API enum fh_cam_mode fh_cam_get_mode(struct fh_camera *cam)
{
	if(!cam)
		return 0;

	return cam->mode;
}


FH_API void fh_cam_set_mode(struct fh_camera *cam, enum fh_cam_mode mode)
{
	if(!cam)
		return;

	cam->mode = mode;
}


FH_API void fh_cam_tgl_mode(struct fh_camera *cam)
{
	cam->mode = cam->mode == FH_CAM_FOCUS ? FH_CAM_WIDE : FH_CAM_FOCUS;
}


FH_API void fh_cam_update(struct fh_camera *cam)
{

	fh_cam_update_view(cam);
}


FH_API void fh_cam_rmv_fnc(u32 size, void *ptr)
{
	struct fh_camera *cam;

	/* SILENCIO! */
	if(size) {}

	if(!ptr)
		return;

	cam = (struct fh_camera *)ptr;

	fh_cam_destroy(cam);
}
