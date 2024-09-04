#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../wut.h"

#define STYLE_PATH      "style.wts"

int main(void)
{
        struct wut_Window *win;
        struct wut_Document *doc;
        struct wut_Context *ctx;

        printf("Initialize WUT framework...\n");
        if(wut_Init() < 0) {
                printf("Failed!\n");
                goto err_quit;
        }
        printf("Done\n");

        printf("Create the main window...\n");
        if(!(win = wut_CreateWindow(NULL, "main", 800, 600))) {
                printf("Failed!\n");
                goto err_shutdown;
        }
        printf("Done\n");

        printf("Get window related resources...\n");
        doc = wut_GetDocument(win);
        ctx = wut_GetContext(win);
        printf("Done\n");

        printf("Load style class from file \"%s\"...\n", STYLE_PATH);
        if(wut_LoadClasses(doc, STYLE_PATH) < 0) {
                printf("Failed\n");
                goto err_close_window;
        }
        printf("Done\n");

        wut_PrintClasses(doc->classes);

        while(wut_Update()) {

        }

err_close_window:
        wut_CloseWindow(win);

err_shutdown:
        printf("Cleanup WUT framework...\n");
        wut_Quit();
        printf("Done\n");

err_quit:
        return 0;
}
