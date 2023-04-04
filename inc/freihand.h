#ifndef _FH_FREIHAND_H
#define _FH_FREIHAND_H

#include "define.h"
#include "import.h"
#include "core.h"
#include "sdl.h"
#include "opengl.h"
#include "window.h"
#include "document.h"
#include "element.h"


/*
 * Initialize the FH-framework.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_init(void);


/*
 * Shutdown the FH-framework and clear the allocated memory.
 */
FH_API void fh_quit(void);


/*
 * Create a new window and attach it to the parent. If 0 is given, it will be
 * considered the main window.
 *
 * @parent: The windowId of the parent window
 * @name: The name of the window
 * @width: The initial width of the window
 * @height: The intial height of the window
 *
 * Returns: Either the window-descriptor(wd > 1) or -1 if an error occurred
 */
FH_API s32 fh_add_window(s32 parent, char *name, s32 width, s32 height);


/*
 * Handle inputs, update and redraw the window.
 *
 * Returns: 1 if everything is normal, and 0 if either a fatal error occurred or
 * 	    the user requested to close the program
 */
FH_API s8 fh_update(void);


/*
 * Pull the oldest event from the event pipe.
 *
 * @event: A pointer to write the pulled event to, if there is any
 *
 * Returns: 1 if an event has been returned, 0 if there are no more events and
 * 	    -1 if an error occurred
 */
FH_API s8 fh_pull_event(struct fh_event *event);


/*
 * Add a new element to the document of window.
 *
 * @wd: The window descriptor
 * @parent: A pointer to the parent element
 * @name: The name of the element
 * @type: The type of the new element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API struct fh_element *fh_add(s32 wd, struct fh_element *parent, char *name,
		enum fh_element_type type);


/*
 * Get an element from a window by searching for the given name.
 *
 * @win: The window descriptor
 * @name: The name of the element to look for
 *
 * Returns: Either a pointer to the element or NULL if an error occurred
 */
FH_API struct fh_element *fh_get(s32 wd, char *name);


/*
 * Load a shader.
 *
 * @name: The name of the shader
 * @v_src: The path to the vertex shader file
 * @f_src: The path to the fragment shader file
 * @var_num: The number of input variables for the shader
 * @vars: The names of the input variables
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_load_shader(char *name, const char *v_src, const char *f_src,
		u8 var_num, char **vars);


/*
 * Remove a shader from the global shader list.
 *
 * @name: The name of the shader
 */
FH_API void fh_remove_shader(char *name);


/*
 * Get a pointer to a shader from the global shader list.
 *
 * @name: The name of the shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_get_shader(char *name);


/*
 * Select a shader to be the UI shader.
 *
 * @name: The name of the shader
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_set_ui_shader(char *name);

#endif /* _FH_FREIHAND_H */
