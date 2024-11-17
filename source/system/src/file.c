#include "source/system/inc/file.h"

#include "source/utility/inc/alarm.h"

#include "source/system/inc/system.h"

#include <stdlib.h>


#define WUT_FS_DEBUG 1


WUT_API s8 wut_fs_raw(const char *pth, u64 *size, u8 **out)
{
	FILE *fd;
	char *buf;
	u64 file_size;
	u64 bytes_read;

	if(!pth) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(fd = fopen(pth, "rb"))) {
		WUT_ALARM(WUT_ERROR, "Failed to open file");
		goto err_return;
	}

	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);


	if(!(buf = wut_malloc(file_size + 1))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for file buffer");
		goto err_close_fd;
	}

	if((bytes_read = fread(buf, file_size, 1, fd)) == 0) {
		WUT_ALARM(WUT_ERROR, "Failed to read file");
		goto err_free_buf;
	}

	if(size) *size = file_size;
	*out = (u8 *)buf;
	return 0;

err_free_buf:
	wut_free(buf);

err_close_fd:
	fclose(fd);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load file");
	return -1;
}


WUT_API s8 wut_fs_text(const char *pth, u64 *size, char **out)
{
	FILE *fd;
	char *buf;
	u64 file_size;
	u64 bytes_read;

	if(!pth) {
		WUT_ALARM(WUT_ERROR, "Input parameters invalid");
		goto err_return;
	}

	if(!(fd = fopen(pth, "rb"))) {
		WUT_ALARM(WUT_ERROR, "Failed to open file");
		goto err_return;
	}

	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);


	if(!(buf = wut_malloc(file_size + 1))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for file buffer");
		goto err_close_fd;
	}

	if((bytes_read = fread(buf, file_size, 1, fd)) == 0) {
		WUT_ALARM(WUT_ERROR, "Failed to read file");
		goto err_free_buf;
	}

	buf[file_size] = '\0';

	if(size) *size = file_size;
	*out = buf;
	return 0;

err_free_buf:
	wut_free(buf);

err_close_fd:
	fclose(fd);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load file");
	return -1;
}

#if 0
/*
 * This function will map the SDL_PixelFormatEnum to the OpenGL pixel format
 * enum.
 */
WUT_INTERN GLenum wut_fs_map_format(SDL_PixelFormatEnum pixelFormat)
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


WUT_API s8 wut_fs_image(const char *pth, struct wut_fs_r_image *out)
{
	SDL_Surface *in_surf;
	SDL_Surface *out_surf;
	u8 *buf;
	u64 size;
	
	s32 x;
	s32 y;
	u32 px;
	s8 step;


	if(!pth || !out) {
		WUT_ALARM(WUT_WARNING, "Input parameters invalid");
		goto err_return;
	}


	if(!(in_surf = IMG_Load(pth))) {
		WUT_ALARM(WUT_ERROR, "Failed to open and read file");
		goto err_return;
	}

	if(!(out_surf = SDL_ConvertSurfaceFormat(in_surf, SDL_PIXELFORMAT_RGBA32, 0))) {
		printf("Failed to convert surface\n");
		goto err_free_in_surface;
	}

	size = out_surf->pitch * out_surf->h;
	if(!(buf = wut_malloc(size))) {
		WUT_ALARM(WUT_ERROR, "Failed to allocate memory for image");
		goto err_free_out_surface;
	}

	memcpy(buf, out_surf->pixels, size);

	out->w = out_surf->w;
	out->h = out_surf->h;
	out->format = GL_RGBA;
	out->data = buf;

	printf("Old Pitch: %d\n", in_surf->pitch);
	printf("New Pitch: %d\n", out_surf->pitch);

	SDL_FreeSurface(in_surf);
	SDL_FreeSurface(out_surf);

	return 0;


err_free_out_surface:
	SDL_FreeSurface(out_surf);

err_free_in_surface:
	SDL_FreeSurface(in_surf);

err_return:
	WUT_ALARM(WUT_ERROR, "Failed to load PNG file");
	return -1;
}


WUT_API void wut_fs_image_cleanup(struct wut_fs_r_image *img)
{
	if(!img)
		return;

	wut_free(img->data);
}
