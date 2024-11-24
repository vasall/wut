#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../wut.h"
#include "../source/component/inc/string_object.h"


#define STYLE_FILES     1
char *style_paths[STYLE_FILES] = {
        "style.wts"
};

/* Cube vertices (positions only) */
float cube_vertices[24] = {
    /* Front face (z = 1.0) */
    -1.0f, -1.0f,  1.0f, /* Bottom-left         */
     1.0f, -1.0f,  1.0f, /* Bottom-right        */
     1.0f,  1.0f,  1.0f, /* Top-right           */
    -1.0f,  1.0f,  1.0f, /* Top-left            */

    /* Back face (z = -1.0) */
    -1.0f, -1.0f, -1.0f, /* Bottom-left         */
     1.0f, -1.0f, -1.0f, /* Bottom-right        */
     1.0f,  1.0f, -1.0f, /* Top-right           */
    -1.0f,  1.0f, -1.0f, /* Top-left            */
};

/* Cube indices (for rendering triangles) */
unsigned int cube_indices[36] = {
    /* Front face       */
    0, 1, 2,
    2, 3, 0,

    /* Back face        */
    4, 5, 6,
    6, 7, 4,

    /* Left face        */
    4, 0, 3,
    3, 7, 4,

    /* Right face       */
    1, 5, 6,
    6, 2, 1,

    /* Top face         */
    3, 2, 6,
    6, 7, 3,

    /* Bottom face      */
    4, 5, 1,
    1, 0, 4,
};


struct wut_Object *create_cube(struct wut_Context *ctx)
{
        struct wut_Object *obj;
        wut_Vec3 pos;
        wut_Vec3 rot;

        struct wut_ObjectConstr *c = wut_BeginObjectConstr("Cube1", 8, 24, cube_indices);
        
        /* Set the shader to use */
        wut_ObjectConstrShader(c, wut_GetShader(ctx, "test"));

        /* Add the vertex positions */
        wut_ObjectConstrAttrib(c, "v_pos", 3, GL_FLOAT, cube_vertices);

        wut_vec3_set(pos, 0, 0, 0);
        wut_vec3_set(rot, 0, 0, 0);

        obj = wut_EndObjectConstr(c, ctx, pos, rot);

        wut_ObjectConstrCleanup(c);

        return obj;

}


int main(void)
{
        int i;

        struct wut_Window *win;
        struct wut_Document *doc;
        struct wut_Context *ctx;

        struct wut_Element *ele;
        struct wut_View *view;


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

        wut_LoadTexture(ctx, "Missing", "Missing.png");
        wut_LoadTexture(ctx, "Blue", "Blue.png");
        wut_LoadTexture(ctx, "Gray", "Untitled.png");

        wut_LoadFont(ctx, "Here", "here.png", "here.txt");

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

#if 0
        view = wut_GetView(wut_GetElement(doc, "view"));
#endif

        printf("----------------------------------------------------------\n");
        printf("DOCUMENT TREE:\n");
        wut_ShowDocumentTree(doc, NULL);
        printf("----------------------------------------------------------\n");
        wut_PrintClasses(doc->class_table);
        printf("----------------------------------------------------------\n");


        wut_DumpStylesheet(&doc->body->style);

        while(wut_Update());

err_close_window:
        wut_CloseWindow(win);

err_shutdown:
        printf("Cleanup WUT framework...\n");
        wut_Quit();
        printf("Done\n");

err_quit:
        return 0;
}
