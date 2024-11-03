#ifndef _WUT_GRAPHICTEXTURE_H
#define _WUT_GRAPHICTEXTURE_H

#include "core/inc/define.h"
#include "core/inc/import.h"

#include "component/inc/table.h"

#include "math/inc/polygon.h"

#include "graphic/inc/context.h"

#define WUT_TEXTURE_SLOTS		16

#define WUT_TEXTURE_NAME_LIM		128


struct wut_Texture {
	/* The name of the texture */
	char name[WUT_TEXTURE_NAME_LIM];
	
	/* The format used for the pixel data */
	GLenum format;

	/* The size of the texture in pixels */
	u16 width;
	u16 height;

	/* The texture handle used by OpenGL */
	u32 texture;

	/* Reference to the context */
	struct wut_Context *context;

	/* The batch id */
	s16 batch_id;

	/* The slot in the texture list */
	s16 texture_slot;
};


/*
 * Create and initialize a texture table for a context.
 *
 * @ctx: Pointer to the context
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_InitTextureTable(struct wut_Context *ctx);


/*
 * Close the global texture list.
 *
 * @ctx: Pointer to the context
 */
WUT_API void wut_CloseTextureTable(struct wut_Context *ctx);


/*
 * Create a new texture from raw pixel data.
 * Especially for the format, see:
 * https:/registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
 *
 * @ctx: Pointer to the context
 * @name: The name of the new texture
 * @w: The width of the new texture in pixels
 * @h: The height of the nex texture in pixels
 * @format: The format used for the pixel data
 * @px: The raw pixel data
 *
 * Returns: Either a pointer to the texture or NULL if an error occurred
 */
WUT_API struct wut_Texture *wut_CreateTexture(struct wut_Context *ctx, char *name,
		u16 w, u16 h, GLenum format, u8 *px);


/*
 * Load a texture from a file.
 *
 * @ctx: Pointer to the texture
 * @name: The name of the file
 * @pth: The path to the file
 *
 * Returns: Either a pointer to the texture or NULL if an error occurred
 */
WUT_API struct wut_Texture *wut_LoadTexture(struct wut_Context *ctx, char *name,
		char *pth);


/*
 * Remove and destroy a loaded texture.
 *
 * @tex: Pointer to the texture
 */
WUT_API void wut_RemoveTexture(struct wut_Texture *tex);


/*
 * Resize a texture.
 *
 * @tex: Pointer to the texture
 * @w: The new width
 * @h: The new height
 * @px: The new pixel data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_ResizeTexture(struct wut_Texture *tex, u16 w, u16 h, u8 *px);


/*
 * Update the texture and overwrite with the new pixel data.
 * Note that for this function to work, the texture has to already exist in the
 * texture table.
 *
 * @tex: Pointer to the texture
 * @x: The x-offset of the rectangle
 * @y: The y-offset of the rectangle
 * @w: The width of the rectangle
 * @h: The height of the rectangle
 * @px: The RGBA pixel data
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_SetTexture(struct wut_Texture *tex, u16 x, u16 y, u16 w, u16 h,
		u8 *px);


/*
 * Get a pointer to a texture from the texture table.
 *
 * @ctx: Pointer to the context
 * @name: The name of the texture
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API struct wut_Texture *wut_GetTexture(struct wut_Context *ctx, char *name);


/*
 * Activate a texture from the texture table.
 *
 * @tex: Pointer to the texture
 */
WUT_API void wut_UseTexture(struct wut_Texture *tex);


/*
 * Unuse the currently active texture.
 */
WUT_API void wut_UnuseTexture(void);



#endif /* _WUT_GRAPHICTEXTURE_H */
