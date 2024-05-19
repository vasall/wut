#include "system/inc/file.h"

#include "utility/inc/alarm.h"

#include "system/inc/system.h"

#include <stdlib.h>


#define WT_FS_DEBUG 1


WT_API s8 wt_fs_raw(const char *pth, u64 *size, u8 **out)
{
	FILE *fd;
	char *buf;
	u64 file_size;
	u64 bytes_read;

	if(!pth) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(fd = fopen(pth, "rb"))) {
		WT_ALARM(WT_ERROR, "Failed to open file");
		goto err_return;
	}

	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);


	if(!(buf = wt_malloc(file_size + 1))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for file buffer");
		goto err_close_fd;
	}

	if((bytes_read = fread(buf, file_size, 1, fd)) == 0) {
		WT_ALARM(WT_ERROR, "Failed to read file");
		goto err_free_buf;
	}

	if(size) *size = file_size;
	*out = (u8 *)buf;
	return 0;

err_free_buf:
	wt_free(buf);

err_close_fd:
	fclose(fd);

err_return:
	WT_ALARM(WT_ERROR, "Failed to load file");
	return -1;
}


WT_API s8 wt_fs_text(const char *pth, u64 *size, char **out)
{
	FILE *fd;
	char *buf;
	u64 file_size;
	u64 bytes_read;

	if(!pth) {
		WT_ALARM(WT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(fd = fopen(pth, "rb"))) {
		WT_ALARM(WT_ERROR, "Failed to open file");
		goto err_return;
	}

	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);


	if(!(buf = wt_malloc(file_size + 1))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for file buffer");
		goto err_close_fd;
	}

	if((bytes_read = fread(buf, file_size, 1, fd)) == 0) {
		WT_ALARM(WT_ERROR, "Failed to read file");
		goto err_free_buf;
	}

	buf[file_size] = '\0';

	if(size) *size = file_size;
	*out = buf;
	return 0;

err_free_buf:
	wt_free(buf);

err_close_fd:
	fclose(fd);

err_return:
	WT_ALARM(WT_ERROR, "Failed to load file");
	return -1;
}

#if 0
/*
 * This function will map the SDL_PixelFormatEnum to the OpenGL pixel format
 * enum.
 */
WT_INTERN GLenum wt_fs_map_format(SDL_PixelFormatEnum pixelFormat)
{
    switch (pixelFormat)
    {
        case SDL_PIXELFORMAT_RGB332:
            return GL_RGB;
        case SDL_PIXELFORMAT_RGB444:
            return GL_RGB;
        case SDL_PIXELFORMAT_RGB555:
            return GL_RGB;
        case SDL_PIXELFORMAT_BGR555:
            return GL_BGR;
        case SDL_PIXELFORMAT_ARGB4444:
            return GL_RGBA;
        case SDL_PIXELFORMAT_RGBA4444:
            return GL_RGBA;
        case SDL_PIXELFORMAT_ABGR4444:
            return GL_BGRA;
        case SDL_PIXELFORMAT_BGRA4444:
            return GL_BGRA;
        case SDL_PIXELFORMAT_ARGB1555:
            return GL_RGBA;
        case SDL_PIXELFORMAT_RGBA5551:
            return GL_RGBA;
        case SDL_PIXELFORMAT_ABGR1555:
            return GL_BGRA;
        case SDL_PIXELFORMAT_BGRA5551:
            return GL_BGRA;
        case SDL_PIXELFORMAT_RGB565:
            return GL_RGB;
        case SDL_PIXELFORMAT_BGR565:
            return GL_BGR;
        case SDL_PIXELFORMAT_RGB24:
            return GL_RGB;
        case SDL_PIXELFORMAT_BGR24:
            return GL_BGR;
        case SDL_PIXELFORMAT_RGB888:
            return GL_RGB;
        case SDL_PIXELFORMAT_RGBX8888:
            return GL_RGBA;
        case SDL_PIXELFORMAT_BGR888:
            return GL_BGR;
        case SDL_PIXELFORMAT_BGRX8888:
            return GL_BGRA;
        case SDL_PIXELFORMAT_ARGB8888:
            return GL_RGBA;
        case SDL_PIXELFORMAT_RGBA8888:
            return GL_RGBA;
        case SDL_PIXELFORMAT_ABGR8888:
            return GL_BGRA;
        case SDL_PIXELFORMAT_BGRA8888:
            return GL_BGRA;
        case SDL_PIXELFORMAT_ARGB2101010:
            return GL_RGBA;
        case SDL_PIXELFORMAT_YV12:
            return GL_RED;
        case SDL_PIXELFORMAT_IYUV:
            return GL_RED;
        case SDL_PIXELFORMAT_YUY2:
            return GL_RED;
        case SDL_PIXELFORMAT_UYVY:
            return GL_RED;
        case SDL_PIXELFORMAT_YVYU:
            return GL_RED;
        default:
            return 0; /* Invalid pixel format */
    }

    return 0;
}
#endif


WT_API s8 wt_fs_image(const char *pth, struct wt_fs_r_image *out)
{
	SDL_Surface *surf;
	u8 *buf;
	u64 size;
	
	if(!pth || !out) {
		WT_ALARM(WT_WARNING, "Input parameters invalid");
		goto err_return;
	}


	if(!(surf = IMG_Load(pth))) {
		WT_ALARM(WT_ERROR, "Failed to open and read file");
		goto err_return;
	}


	size = surf->pitch * surf->h;
	if(!(buf = wt_malloc(size))) {
		WT_ALARM(WT_ERROR, "Failed to allocate memory for image");
		goto err_free_surface;
	}

	memcpy(buf, surf->pixels, size);

	out->w = surf->w;
	out->h = surf->h;
	out->format = GL_RGBA;
	out->data = buf;

	SDL_FreeSurface(surf);

	return 0;

err_free_surface:
	SDL_FreeSurface(surf);

err_return:
	WT_ALARM(WT_ERROR, "Failed to load PNG file");
	return -1;
}


WT_API void wt_fs_image_cleanup(struct wt_fs_r_image *img)
{
	if(!img)
		return;

	wt_free(img->data);
}
