#include "file.h"

#include "alarm.h"
#include "system.h"

#include <stdlib.h>


FH_API s8 fh_fs_raw(const char *pth, u64 *size, u8 **out)
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


	if(!(buf = fh_malloc(file_size + 1))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for file buffer");
		goto err_close_fd;
	}

	if((bytes_read = fread(buf, file_size, 1, fd)) == 0) {
		ALARM(ALARM_ERR, "Failed to read file");
		goto err_free_buf;
	}

	if(size) *size = file_size;
	*out = (u8 *)buf;
	return 0;

err_free_buf:
	fh_free(buf);

err_close_fd:
	fclose(fd);

err_return:
	ALARM(ALARM_ERR, "Failed to load file");
	return -1;
}


FH_API s8 fh_fs_text(const char *pth, u64 *size, char **out)
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


	if(!(buf = fh_malloc(file_size + 1))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for file buffer");
		goto err_close_fd;
	}

	if((bytes_read = fread(buf, file_size, 1, fd)) == 0) {
		ALARM(ALARM_ERR, "Failed to read file");
		goto err_free_buf;
	}

	buf[file_size] = '\0';

	if(size) *size = file_size;
	*out = buf;
	return 0;

err_free_buf:
	fh_free(buf);

err_close_fd:
	fclose(fd);

err_return:
	ALARM(ALARM_ERR, "Failed to load file");
	return -1;
}


FH_API s8 fh_fs_png(const char *pth, u32 *w, u32 *h, u8 **out)
{
	SDL_Surface *surf;
	u8 *buf;
	u64 size;

	if(!(surf = IMG_Load(pth))) {
		ALARM(ALARM_ERR, "Failed to open and read file");
		goto err_return;
	}

	size = surf->pitch * surf->h;

	if(!(buf = fh_malloc(size))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for image");
		goto err_free_surface;
	}

	memcpy(buf, surf->pixels, size);
	SDL_FreeSurface(surf);

	*w = surf->w;
	*h = surf->h;
	*out = buf;

	return 0;

err_free_surface:
	SDL_FreeSurface(surf);

err_return:
	ALARM(ALARM_ERR, "Failed to load PNG file");
	return -1;
}
