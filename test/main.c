#include "../wut.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
        struct wut_Window *win;

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

        while(wut_Update()) {

        }


err_shutdown:
        printf("Cleanup WUT framework...\n");
        wut_Quit();
        printf("Done\n");

err_quit:
        return 0;
}
