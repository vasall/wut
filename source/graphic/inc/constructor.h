#ifndef _WUT_GRAPHIC_CONSTRUCTOR_H
#define _WUT_GRAPHIC_CONSTRUCTOR_H

#include "source/core/inc/define.h"
#include "source/core/inc/import.h"

#include "source/graphic/inc/object.h"

#include "source/window/inc/window.h"

struct wut_ObjectConstr;


#include "source/graphic/inc/object.h"


struct wut_ObjectConstrAttrib {
        /* The name of the attribute */
        char name[128];

        /* 
         * The number of elements the attribute consists of for every vertex.
         * The total size of the attribute for each vertex is callculated as:
         * 	elements * element_size ie 3 * sizeof(f32)
         */
        u8 elements;
        u8 element_size;

        /* The type of each element */
        GLenum type;

        /* the buffer containing the attribute data for all vertices */
        u8 *data;
};

struct wut_ObjectConstrUnibuf {
        /* The name of the new uniform buffer */
        char name[128];

        /* The size of the uniform buffer in bytes */
        u32 size;
};

enum wut_eObjectConstType {
        WUT_CTR_DEFAULT,
        WUT_CTR_RIGGED,
        WUT_CTR_CUSTOM
};

struct wut_ObjectConstr {
        /* The name of the new object */
        char name[WUT_OBJECT_NAME_LIM];

        /* The construction process to utilize */
        enum wut_eObjectConstType type;

        /* The shader to use for the object */
        struct wut_Shader *shader;

        /* The texture to use for the object */
        struct wut_Texture *texture;

        /* The number of vertices */
        u32 vtx_num;

        /* The index data */
        u32 idx_num;
        u32 *idx;

        /* The attributes of the new object */
        u32 attrib_num;
        struct wut_ObjectConstrAttrib attribs[WUT_OBJECT_ATTRIB_LIM];

        /* The uniform buffers for the new object */
        u32 unibuf_num;
        struct wut_ObjectConstrUnibuf unibufs[WUT_OBJECT_UNIFORM_LIM]; 
};


/*
 * Create a new object constructor and begin creating a new object.
 *
 * @name: The name of the new object
 * @vnum: The number of vertices
 * @inum: The number of indices
 * @idx: The index data
 *
 * Returns: A new constructor or NULL if an error occurred
 */
WUT_API struct wut_ObjectConstr *wut_BeginObjectConstr(char *name,
                u32 vnum, u32 inum, u32 *idx);


/*
 * Finalize construction and get the finished object. This function will also
 * insert the object into the object table of the given context.
 *
 * @c: Pointer to the constructor
 * @ctx: Pointer to the context
 * @pos: The initial position of the object
 * @rot: The initial rotation of the object
 *
 * Returns: Either a pointer to the finished object or NULL if an error occurred
 */
WUT_API struct wut_Object *wut_EndObjectConstr(struct wut_ObjectConstr *c,
                struct wut_Context *ctx, wut_Vec3 pos, wut_Vec3 rot);


/*
 * After the object constructor has been utilized and is not need anymore, this
 * function should be called to cleanup everything and free the allocated
 * memory. The objects that have been created using the constructor are
 * unaffected.
 *
 * @c: Pointer to the constructor
 */
WUT_API void wut_ObjectConstrCleanup(struct wut_ObjectConstr *c);


/*
 * Set the texture of the object during construction.
 *
 * @c: Pointer to the object constructor
 * @tex: Pointer to the texture
 */
WUT_API void wut_ObjectConstrTexture(struct wut_ObjectConstr *c, struct wut_Texture *tex);


/*
 * Set the shader of the object during construction.
 *
 * @c: Pointer to the object constructor
 * @shd: Pointer to the shader
 */
WUT_API void wut_ObjectConstrShader(struct wut_ObjectConstr *c, struct wut_Shader *shd);


/*
 * Attach a new attribute to the object.
 *
 * Use like: wut_ObjectConstrAttrib(c, "v_pos", 3, GL_FLOAT, vtx);
 *
 * @c: Pointer to the object constructor
 * @name: The name of the new attribute(has to match the shader input)
 * @size: The size of the element in data type quantities (ie 3 * GL_FLOAT)
 * @type: The data type of an element
 * @data: The buffer containing the data
 */
WUT_API void wut_ObjectConstrAttrib(struct wut_ObjectConstr *c, char *name, u8 size,
                GLenum type, void *data);


/*
 * Add a new uniform buffer to the object.
 *
 * Use like: wut_ObjectConstrUniform(c, "camera", sizeof(struct uniform_buffer));
 *
 * @c: Pointer to the object constructor
 * @name: The name of the uniform buffer
 * @size: The size of the uniform buffer in bytes
 */
WUT_API void wut_ObjectConstrUniform(struct wut_ObjectConstr *c, char *name, u32 size);



#endif /* _WUT_GRAPHIC_CONSTRUCTOR_H */
