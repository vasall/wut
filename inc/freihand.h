#ifndef _FH_FREIHAND_H
#define _FH_FREIHAND_H

#include "define.h"
#include "datatype.h"
#include "table.h"
#include "import.h"
#include "system.h"
#include "core.h"
#include "sdl.h"
#include "opengl.h"
#include "camera.h"
#include "window.h"
#include "document.h"
#include "element.h"
#include "model_constructor.h"


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
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                 WINDOW
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Create a new window and attach it to the parent. If 0 is given, it will be
 * considered the main window.
 *
 * @parent: Pointer to the parent window or NULL
 * @name: The name of the window
 * @width: The initial width of the window
 * @height: The intial height of the window
 *
 * Returns: Either a pointer to the created window or NULL if an error occurred
 */
FH_API struct fh_window *fh_add_window(struct fh_window *parent, char *name,
		s32 width, s32 height);


/*
 * Activate a window so it can be rendered on.
 *
 * @win: Pointer to the window
 */
FH_API void fh_activate_window(struct fh_window *win);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                 DOCUMENT
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


/*
 * Add a new element to the document of window.
 *
 * @win: Pointer to the window
 * @parent: A pointer to the parent element
 * @name: The name of the element
 * @type: The type of the new element
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API struct fh_element *fh_add(struct fh_window *win,
		struct fh_element *parent, char *name,
		enum fh_element_type type);


/*
 * Get an element from a window by searching for the given name.
 *
 * @win: The window descriptor
 * @name: The name of the element to look for
 *
 * Returns: Either a pointer to the element or NULL if an error occurred
 */
FH_API struct fh_element *fh_get(struct fh_window *win, char *name);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                  SHADER
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * This function will take the given source code for the vertex- and
 * fragment-shader and create a new OpenGL shader program and add it to the
 * shader table.
 *
 * @win: Pointer to the window
 * @name: The name of the shader
 * @v_src: The source code for the vertex-shader
 * @f_src: The source code for the fragment-shader
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_create_shader(struct fh_window *win, char *name,
		char *v_src, char *f_src);


/*
 * Load the given vertex- and fragment-shader-files and create an OpenGL shader
 * program and add it to the shader table.
 *
 * @win: Pointer to the window
 * @name: The name of the shader
 * @v_pth: The path to the vertex shader file
 * @f_pth: The path to the fragment shader file
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_load_shader(struct fh_window *win, char *name,
		char *v_pth, char *f_pth);


/*
 * Remove a shader from the global shader list.
 *
 * @win: Pointer to the window
 * @name: The name of the shader
 */
FH_API void fh_remove_shader(struct fh_window *win, char *name);


/*
 * Get a pointer to a shader from the global shader list.
 *
 * @win: Pointer to the window
 * @name: The name of the shader
 *
 * Returns: Either a pointer to the shader or NULL if an error occurred
 */
FH_API struct fh_shader *fh_get_shader(struct fh_window *win, char *name);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                 TEXTURE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Create a new texture from raw pixel data.
 * Especially for the format, see:
 * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
 *
 * @win: Pointer to the window
 * @name: The name of the new texture
 * @w: The width of the new texture in pixels
 * @h: The height of the nex texture in pixels
 * @format: The format used for the pixel data
 * @px: The raw pixel data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_create_texture(struct fh_window *win, char *name, u16 w, u16 h,
		GLenum format, u8 *px);


/*
 * Load a texture from a file.
 *
 * @win: Pointer to the window
 * @name: The name of the file
 * @pth: The path to the file
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_load_texture(struct fh_window *win, char *name, char *pth);


/*
 * Remove and destroy a loaded texture.
 *
 * @win: Pointer to the window
 * @name: The name of the texture
 */
FH_API void fh_remove_texture(struct fh_window *win, char *name);


/*
 * Get a pointer to a texture.
 *
 * @win: Pointer to the window
 * @name: The name of the texture
 *
 * Returns: Either a pointer to the texture or NULL if the texture was not found
 * 	    or an error occurred
 */
FH_API struct fh_texture *fh_get_texture(struct fh_window *win, char *name);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                 CAMERA
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

/*
 * Create a new camera and add it to the camera table.
 *
 * @win: Pointer to the window
 * @name: The name of the camera
 * @info: A buffer containing the essential data for the camera
 *
 * Returns: Either a pointer to the camera or NULL if an error occurred
 */
FH_API struct fh_camera* fh_create_camera(struct fh_window *win, char *name,
		struct fh_camera_info info);


/*
 * Remove and destroy a camera.
 *
 * @win: Pointer to the window
 * @name: The name of the camera
 */
FH_API void fh_remove_camera(struct fh_window *win, char *name);


/*
 * Get the current view matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @win: Pointer to the window
 * @name: The name of the camera
 * @out: A matrix to write the view matrix to
 */
FH_API void fh_get_view(struct fh_window *win, char *name, mat4_t out);


/*
 * Get the current projection matrix for a camera.
 * If the camera does not exist or an error occured, an identity-matrix will be
 * returned.
 *
 * @win: Pointer to the window
 * @name: The name of the camera
 * @out: A matrix to write the projection matrix to
 */
FH_API void fh_get_projection(struct fh_window *win, char *name, mat4_t out);


/*
 * Set the position of a camera.
 *
 * @win: Pointer to the window
 * @name: The name of the camera
 * @pos: The new position of the camera
 */
FH_API void fh_set_camera_position(struct fh_window *win, char *name,
		vec3_t pos);


/*
 * Move the camera by a given delta.
 *
 * @win: Pointer to the window
 * @name: The name of the camera
 * @del: The position difference to move the camera by
 */
FH_API void fh_move_camera(struct fh_window *win, char *name, vec3_t del);


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *                                MODEL
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

#endif /* _FH_FREIHAND_H */
