#include "freihand.h"

#include "alarm.h"
#include "system.h"
#include "document.h"
#include "shader.h"

#include <stdlib.h>


FH_API s8 fh_init(void)
{
	/* Reset the core */
	fh_core_reset();

	/* Then initialize the SDL-frameworks */
	if(fh_sdl_init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize the XWIN-SDL module");
		goto err_return;
	}

	/* Initialize OpenGL */
	if(fh_gl_init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize the SDL GL module");
		goto err_quit_sdl;
	}

	return 0;

err_quit_sdl:
	fh_sdl_quit();

err_return:
	/* Reset te core */
	fh_core_reset();

	ALARM(ALARM_ERR, "Failed to initialize FH framework");
	return -1;
}


FH_API void fh_quit(void)
{
	/* Close all windows */
	fh_win_close(fh_core_get_main_window());
	fh_core_set_main_window(NULL);
	
	/* Shutdown SDL */
	fh_sdl_quit();

	/* Reset the core */
	fh_core_reset();
}





FH_API s8 fh_update(void)
{
	/* Check if the quit flag has been triggered */
	if(fh_core_check_quit())
		return 0;

	fh_evt_process();



	/* Redraw all visible windows */
	fh_win_redraw_all();

	return 1;
}


FH_API s8 fh_pull_event(struct fh_event *event)
{
	return fh_evt_pipe_pull(event);
}



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                  WINDOW
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_window *fh_add_window(struct fh_window *parent, char *name,
		s32 width, s32 height)
{
	struct fh_window *win;
		
	if(name == NULL || width < 0 || height < 0) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Then create a new window */
	if(!(win = fh_win_create(name, width, height)))
		goto err_return;

	/* If a parent is specified */
	if(parent != NULL) {
		fh_win_attach(parent, win);
	}
	/* Otherwise... */
	else {
		/* Mark this window as the main window */
		win->info = win->info | FH_WIN_INFO_MAIN;

		/* Set this window as the main window */
		fh_core_set_main_window(win);
	}

	return win;

err_return:
	ALARM(ALARM_ERR, "Failed to create add new window");
	return NULL;
}


FH_API void fh_activate_window(struct fh_window *win)
{
	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		return;
	}

	fh_win_activate(win);
}



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                 DOCUMENT
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_element *fh_add(struct fh_window *win,
		struct fh_element *parent, char *name,
		enum fh_element_type type)
{
	struct fh_element *ele;

	if(!win || !parent || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Add element to document */
	if(!(ele = fh_doc_add_element(win->document, parent, name, type)))
		goto err_return;
		
	return ele;

err_return:
	ALARM(ALARM_ERR, "Failed to add element");
	return NULL;
}


FH_API struct fh_element *fh_get(struct fh_window *win, char *name)
{
	if(!win || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	return fh_doc_find_element(win->document, name);

err_return:
	ALARM(ALARM_ERR, "Failed to get element");
	return NULL;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                  SHADER
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API s8 fh_create_shader(struct fh_window *win, char *name,
		char *v_src, char *f_src)
{
	struct fh_shader *shader;

	if(!win || !name || !v_src || !f_src) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shader = fh_shd_create(name, v_src, f_src)))
		goto err_return;

	if(fh_shd_insert(win, shader) < 0)
		goto err_destroy_shader;

	return 0;

err_destroy_shader:
	fh_shd_destroy(shader);

err_return:
	ALARM(ALARM_ERR, "Failed to create new shader");
	return -1;
}


FH_API s8 fh_load_shader(struct fh_window *win, char *name,
		char *v_pth, char *f_pth)
{
	struct fh_shader *shader;

	if(!win || !name || !v_pth || !f_pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shader = fh_shd_load(name, v_pth, f_pth)))
		goto err_return;

	if(fh_shd_insert(win, shader) < 0)
		goto err_destroy_shader;

	return 0;

err_destroy_shader:
	fh_shd_destroy(shader);

err_return:
	ALARM(ALARM_ERR, "Failed to load shader");
	return -1;
}


FH_API void fh_remove_shader(struct fh_window *win, char *name)
{
	struct fh_shader *shader;

	if(!win || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!(shader = fh_shd_get(win, name))) {
		ALARM(ALARM_WARN, "Shader not found");
		return;
	}

	fh_shd_remove(win, shader);
}


FH_API struct fh_shader *fh_get_shader(struct fh_window *win, char *name)
{
	if(!win || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	return fh_shd_get(win, name);

err_return:
	ALARM(ALARM_ERR, "Failed to get shader");
	return NULL;
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                 TEXTURE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API s8 fh_create_texture(struct fh_window *win, char *name, u16 w, u16 h,
		GLenum format, u8 *px)
{
	struct fh_texture *tex;

	if(!win || !name || w < 1 || h < 1 || !px) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = fh_tex_create(name, w, h, format, px)))
		goto err_return;

	if(fh_tex_insert(win, tex) < 0)
		goto err_destroy_tex;

	return 0;

err_destroy_tex:
	fh_tex_destroy(tex);

err_return:
	ALARM(ALARM_ERR, "Failed to create new texture");
	return -1;
}


FH_API s8 fh_load_texture(struct fh_window *win, char *name, char *pth)
{
	struct fh_texture *tex;

	if(!win || !name || !pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(tex = fh_tex_load(name, pth)))
		goto err_return;

	if(fh_tex_insert(win, tex))
		goto err_destroy_tex;

	return 0;

err_destroy_tex:
	fh_tex_destroy(tex);

err_return:
	ALARM(ALARM_ERR, "Failed to load new texture");
	return -1;
}


FH_API void fh_remove_texture(struct fh_window *win, char *name)
{
	struct fh_texture *tex;

	if(!win || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!(tex = fh_tex_get(win, name)))
		return;

	fh_tex_remove(win, tex);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                 CAMERA
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_camera *fh_create_camera(struct fh_window *win, char *name,
		struct fh_camera_info info)
{
	struct fh_camera *cam;

	if(!win || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(cam = fh_cam_create(name, info)))
		goto err_return;

	if(fh_cam_insert(win, cam) < 0)
		goto err_destroy_cam;

	return cam;

err_destroy_cam:
	fh_cam_destroy(cam);

err_return:
	ALARM(ALARM_ERR, "Failed to create new camera");
	return NULL;
}


FH_API void fh_remove_camera(struct fh_window *win, char *name)
{
	struct fh_camera *cam;
	
	if(!win || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!(cam = fh_cam_get(win, name)))
		return;

	fh_cam_remove(win, cam);
}


FH_API void fh_get_view(struct fh_window *win, char *name, mat4_t out)
{
	struct fh_camera *cam;

	if(!win || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	if(!(cam = fh_cam_get(win, name))) {
		ALARM(ALARM_ERR, "Camera not found");
		goto err_return;
	}

	fh_cam_get_view(cam, out);

	return;

err_return:
	mat4_idt(out);
}


FH_API void fh_get_projection(struct fh_window *win, char *name, mat4_t out)
{
	struct fh_camera *cam;

	if(!win || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		goto err_return;
	}

	if(!(cam = fh_cam_get(win, name))) {
		ALARM(ALARM_ERR, "Camera not found");
		goto err_return;
	}

	fh_cam_get_proj(cam, out);
	
	return;

err_return:
	mat4_idt(out);
}


FH_API void fh_set_camera_position(struct fh_window *win, char *name,
		vec3_t pos)
{
	struct fh_camera *cam;

	if(!win || !name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!(cam = fh_cam_get(win, name))) {
		ALARM(ALARM_ERR, "Camera not found");
		return;
	}

	vec3_cpy(cam->pos, pos);

	return;
}


FH_API void fh_move_camera(struct fh_window *win, char *name, vec3_t del)
{
	struct fh_camera *cam;

	if(!name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(!(cam = fh_cam_get(win, name))) {
		ALARM(ALARM_ERR, "Camera not found");
		return;
	}

	vec3_add(cam->pos, del, cam->pos);
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                MODEL
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


