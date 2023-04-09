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

	/*
	 * Initialize the global data tables.
	 */
	if(fh_shd_init() < 0)
		goto err_quit_sdl;

	if(fh_tex_init() < 0)
		goto err_quit_sdl;

	if(fh_mdl_init() < 0)
		goto err_quit_sdl;

	if(fh_cam_init() < 0)
		goto err_quit_sdl;

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


FH_API s32 fh_add_window(s32 parent, char *name, s32 width, s32 height)
{
	struct fh_window *win;
	struct fh_window *par;
		
	if(parent < 0 || name == NULL || width < 0 || height < 0) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* First find the parent to see if it even exists */
	if(parent != 0 && !(par = fh_win_get(parent))) {
		ALARM(ALARM_ERR, "Parent could not be found");
		goto err_return;
	}

	/* Then create a new window */
	if(!(win = fh_win_create(name, width, height)))
		goto err_return;

	/* Lastly attach it */
	if(parent == 0) {
		/* Mark this window as the main window */
		win->info = win->info | FH_WIN_INFO_MAIN;

		/* Set this window as the main window */
		fh_core_set_main_window(win);
	}
	else {
		fh_win_attach(par, win);
	}

	return win->id;
		

err_return:
	ALARM(ALARM_ERR, "Failed to create add new window");
	return -1;
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


FH_API struct fh_element *fh_add(s32 wd, struct fh_element *parent, char *name,
		enum fh_element_type type)
{
	struct fh_window *win;
	struct fh_element *ele;

	if(!parent || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Get a pointer to the window */
	if(!(win = fh_win_get(wd)))
		goto err_return;

	/* Add element to document */
	if(!(ele = fh_doc_add_element(win->document, parent, name, type)))
		goto err_return;
		
	return ele;

err_return:
	ALARM(ALARM_ERR, "Failed to add element");
	return NULL;
}


FH_API struct fh_element *fh_get(s32 wd, char *name)
{
	struct fh_window *win;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Get a pointer to the window */
	if(!(win = fh_win_get(wd)))
		goto err_return;


	return fh_doc_find_element(win->document, name);

err_return:
	ALARM(ALARM_ERR, "Failed to get element");
	return NULL;
}


FH_API s8 fh_create_shader(char *name, char *v_src, char *f_src)
{
	struct fh_shader *shader;

	if(!name || !v_src || !f_src) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shader = fh_shd_create(name, v_src, f_src)))
		goto err_return;

	if(fh_shd_insert(shader) < 0)
		goto err_destroy_shader;

	return 0;

err_destroy_shader:
	fh_shd_destroy(shader);

err_return:
	ALARM(ALARM_ERR, "Failed to create new shader");
	return -1;
}


FH_API s8 fh_load_shader(char *name, char *v_pth, char *f_pth)
{
	struct fh_shader *shader;

	if(!name || !v_pth || !f_pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shader = fh_shd_load(name, v_pth, f_pth)))
		goto err_return;

	if(fh_shd_insert(shader) < 0)
		goto err_destroy_shader;

	return 0;

err_destroy_shader:
	fh_shd_destroy(shader);

err_return:
	ALARM(ALARM_ERR, "Failed to load shader");
	return -1;
}


FH_API void fh_remove_shader(char *name)
{
	if(!name) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_shd_remove(name);
}


FH_API struct fh_shader *fh_get_shader(char *name)
{
	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	return fh_shd_get(name);

err_return:
	ALARM(ALARM_ERR, "Failed to get shader");
	return NULL;
}


FH_API s8 fh_set_ui_shader(char *name)
{
	struct fh_shader *shader;

	if(!name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(shader = fh_shd_get(name))) {
		ALARM(ALARM_ERR, "Shader not found");
		goto err_return;
	}

	fh_core_set_ui_shader(shader);

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to set UI shader");
	return -1;
}
