#ifndef _FH_FILE_H
#define _FH_FILE_H

#include "define.h"
#include "datatype.h"
#include "import.h"


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
FH_API s8 fh_fs_raw(const char *pth, u64 *size, u8 **out);


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
FH_API s8 fh_fs_text(const char *pth, u64 *len, char **out);


/*
 * Load the pixel data from a PNG file.
 * Note that a buffer will be allocated in this function, and the returned
 * buffer has to be freed after use.
 * The pointers must not be NULL.
 *
 * @pth: The path to the file
 * @w: A pointer to write the width of the image to
 * @h: A pointer to write the height of the image to
 * @out: A pointer to write the pixel data to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_fs_png(const char *pth, u32 *w, u32 *h, u8 **out);


#endif /* _FH_FILE_H */
