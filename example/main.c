#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../wut.h"
#include "../source/component/inc/string_object.h"


#define STYLE_FILES     1
char *style_paths[STYLE_FILES] = {
        "style.wts"
};



int main(void)
{
        int i;

        struct wut_Window *win;
        struct wut_Document *doc;
        struct wut_Context *ctx;

        struct wut_Element *ele;

        char buf[512];

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

        for(i = 0; i < STYLE_FILES; i++) {
                printf("Load style class from file \"%s\"...\n", style_paths[i]);
                if(wut_LoadClasses(doc, style_paths[i]) < 0) {
                        printf("Failed\n");
                        goto err_close_window;
                }
                printf("Done\n");
        }

        /* wut_PrintClasses(doc->classes); */
        printf("Load the elements from file \"%s\"...\n", "elements.wtd");

        if(wut_LoadElements(doc, "elements.wtd", NULL) < 0) {
                printf("Failed to load elements from file\n");
                goto err_close_window;
        }
        printf("Done\n");

        printf("----------------------------------------------------------\n");
        printf("DOCUMENT TREE:\n");
        wut_ShowDocumentTree(doc, NULL);
        printf("----------------------------------------------------------\n");
        wut_PrintClasses(doc->class_table);
        printf("----------------------------------------------------------\n");
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
