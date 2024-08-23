#ifndef _WUT_SYSTEM_FILE_H
#define _WUT_SYSTEM_FILE_H

#include "core/inc/define.h"
#include "core/inc/import.h"


/*
 * Load the raw byte data from a file.
 * This function will allocate memory to store the loaded data, which has to be
 * freed after use.
 * The pointers must not be NULL.
 *
 * @pth: The path to the file
 * @size: A pointer to write the size of the buffer to
 * @out: A pointer to write the buffer to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_fs_raw(const char *pth, u64 *size, u8 **out);


/*
 * Load the data from a file and format it as text.
 * This function will allocate memory to store the loaded data and add a null
 * terminator at the end of the string. The memory has to be freed after use.
 * The pointers must not be NULL.
 *
 * @pth: The path to the file
 * @len: A pointer to write the size of the string to
 * @out: A pointer to attach the loaded string to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_fs_text(const char *pth, u64 *len, char **out);



struct wut_fs_r_image {
	u32 w;
	u32 h;

	GLenum format;

	u8 *data;
};

/*
 * Load the pixel data from an image file, preferable a PNG.
 * Note that after using the image, please call wut_hs_image_cleanup() to prevent
 * memory leakage.
 *
 * @pth: The path to the file
 * @out: An image read buffer
 *
 * Returns: 0 on success or -1 if an error occurred
 */
WUT_API s8 wut_fs_image(const char *pth, struct wut_fs_r_image *out);


/*
 * Cleanup after loading an image.
 *
 * @img: Pointer to the image read buffer
 */
WUT_API void wut_fs_image_cleanup(struct wut_fs_r_image *img);


#endif /* _WUT_SYSTEM_FILE_H */
