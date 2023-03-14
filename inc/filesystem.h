#ifndef _FH_FILESYSTEM_H
#define _FH_FILESYSTEM_H

#include "define.h"
#include "import.h"


/*
 * Read a file.
 *
 * @pth: The path to the file
 * @out: A pointer to write the file contents to
 *
 * Returns: 0 on success or -1 if an error occurred
 */
FH_API s8 fh_fs_load(const char *pth, char **out);


#endif /* _FH_FILESYSTEM_H */
