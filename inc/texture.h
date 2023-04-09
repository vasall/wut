#ifndef _FH_TEXTURE_H
#define _FH_TEXTURE_H

#include "define.h"
#include "datatype.h"
#include "table.h"
#include "import.h"


#define FH_TEXTURE_NAME_LIM		128


struct fh_texture {
	/* The name of the texture */
	char name[FH_TEXTURE_NAME_LIM];

	/* The texture handle used by OpenGL */
	u32 texture;

	/* The size of the texture in pixels */
	u16 width;
	u16 height;
};


/*
 * Initialize the global texture list.
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_tex_init(void);


/*
 * Close the global texture list.
 */
FH_API void fh_tex_close(void);


/*
 * Create a new texture.
 *
 * @name: The name of the texture
 * @w: The width of the texture in pixels
 * @h: The height of the texture in pixels
 * @px: The RGBA pixel data
 *
 * Returns: Either a pointer to the created texture or NULL if an error occurred
 */
FH_API struct fh_texture *fh_tex_create(char *name, u16 w, u16 h, u8 *px);


/*
 * Load an image and create a new texture using it.
 *
 * @name: The name of the texture
 * @pth: The path to the file
 *
 * Returns: Either a pointer to the texture or NULL if an error occurred
 */
FH_API struct fh_texture *fh_tex_load(char *name, char *pth);


/*
 * Destroy a texture.
 *
 * @tex: Pointer to the texture
 */
FH_API void fh_tex_destroy(struct fh_texture *tex);


/*
 * Add a new texture to the texture table.
 *
 * @tex: Pointer to a texture
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_tex_insert(struct fh_texture *tex);


/*
 * Remove a texture from the texture table.
 *
 * @name: The name of the texture
 */
FH_API void fh_tex_remove(char *name);


/*
 * Update the texture and overwrite with the new pixel data.
 * Note that for this function to work, the texture has to already exist in the
 * texture table.
 *
 * @tex: Pointer to the texture
 * @w: The width of the texture
 * @h: The height of the texture
 * @px: The RGBA pixel data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_tex_set(struct fh_texture *tex, u16 w, u16 h, u8 *px);


/*
 * Get a pointer to a texture from the texture table.
 *
 * @name: The name of the texture
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API struct fh_texture *fh_tex_get(char *name);


/*
 * Activate a texture from the texture table.
 *
 * @tex: Pointer to the texture
 */
FH_API void fh_tex_use(struct fh_texture *tex);


/*
 * Unuse the currently active texture.
 */
FH_API void fh_tex_unuse(void);


/*
 * The callback function that will be passed to the dbs_table, which will be
 * called in case any entry will be removed from the table.
 */
FH_API void fh_tex_rmv_fnc(u32 size, void *ptr);

#endif /* _FH_TEXTURE_H */
