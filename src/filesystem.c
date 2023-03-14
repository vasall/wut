#include "filesystem.h"

#include <stdlib.h>


FH_API s8 fh_fs_load(const char *pth, char **out)
{
	FILE *fd;
	char *buf;
	u64 file_size;
	u64 bytes_read;

	if(!pth) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(fd = fopen(pth, "rb"))) {
		ALARM(ALARM_ERR, "Failed to open file");
		goto err_return;
	}

	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);


	if(!(buf = smalloc(file_size + 1))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for file buffer");
		goto err_close_fd;
	}

	if((bytes_read = fread(buf, 1, file_size, fd)) != file_size) {
		ALARM(ALARM_ERR, "Failed to read file");
		goto err_free_buf;
	}

	buf[file_size] = '\0';


	*out = buf;
	return 0;

err_free_buf:
	sfree(buf);

err_close_fd:
	fclose(fd);

err_return:
	ALARM(ALARM_ERR, "Failed to load file");
	return -1;
}
