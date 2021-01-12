/*
 * Copyright 2021 Nikolay Burkov <nbrk@linklevel.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#pragma once

#include <stdlib.h>

/*
 * C Scene Graph library.
 */

/*
 * DATA.
 */
/**
 * @brief Opaque handle to the scenegraph node tree. A node contains optional
 * transformation and drawable components, as well as pointer to a user data.
 */
typedef struct csg_node_t csg_node_t;
/**
 * @brief Opaque handle to the subtree's model-space transformation component
 */
typedef struct csg_transform_t csg_transform_t;
/**
 * @brief Opaque handle to the node's drawable component (which is optional)
 */
typedef struct csg_drawable_t csg_drawable_t;
/**
 * @brief Opaque handle to the drawable's geometry data
 */
typedef struct csg_geometry_t csg_geometry_t;
/**
 * @brief Opaque handle to the drawable's material abstraction
 */
typedef struct csg_material_t csg_material_t;
/**
 * @brief Opaque handle to the viewport
 */
typedef struct csg_viewport_t csg_viewport_t;
/**
 * @brief Opaque handle to the camera
 */
typedef struct csg_camera_t csg_camera_t;

/*
 * ENUMS
 */
typedef enum {
  CSG_MATERIAL_COLOR_DIFFUSE,
} csg_material_color_e;

typedef enum {
  CSG_CAMERA_PROJECTION_ORTHOGRAPHIC,
  CSG_CAMERA_PROJECTION_PERSPECTIVE,
} csg_camera_projection_e;

/*
 * FUNCTIONS.
 */
/*
 * Tools
 */
extern void csg_init(void);
extern void* csg_malloc(size_t size);
extern void csg_free(void* mem);

/*
 * Viewport
 */
extern csg_viewport_t* csg_viewport_create(int x, int y, int width, int height,
                                           csg_camera_t* cam);
extern void csg_viewport_reset(csg_viewport_t* view, int x, int y, int width,
                               int height);
extern void csg_viewport_set_clear_color(csg_viewport_t* view, float r, float g,
                                         float b, float a);
extern void csg_viewport_render(csg_viewport_t* view, csg_node_t* root);

/*
 * Camera
 */
extern csg_camera_t* csg_camera_create(csg_camera_projection_e proj,
                                       float aspect);
extern void csg_camera_set_transform(csg_camera_t* cam, csg_transform_t* trans);

/*
 * Nodes
 */
/**
 * @brief Create a new root node or a child node
 * @param parent pointer to a parent node or NULL if the node is a root
 * @return
 */
extern csg_node_t* csg_node_create(csg_node_t* parent, void* cookie);
extern void csg_node_destroy_recursively(csg_node_t* node);
extern void csg_node_apply_depth_first(csg_node_t* node,
                                       void (*func)(csg_node_t*, void*),
                                       void* func_cookie);
extern void* csg_node_get_cookie(csg_node_t* node);
extern csg_node_t** csg_node_get_children(csg_node_t* node,
                                          size_t* num_children);
extern csg_node_t* csg_node_get_parent(csg_node_t* node);
extern void csg_node_set_transform(csg_node_t* node, csg_transform_t* trans);
extern csg_transform_t* csg_node_get_transform(csg_node_t* node);
extern void csg_node_set_drawable(csg_node_t* node, csg_drawable_t* draw);

/*
 * Transforms
 */
extern csg_transform_t* csg_transform_create(float orig_x, float orig_y,
                                             float orig_z);
extern void csg_transform_translate(csg_transform_t* trans, float dx, float dy,
                                    float dz);

/*
 * Geometries
 */
extern csg_geometry_t* csg_geometry_create_triangle(void);

/*
 * Materials
 */
extern csg_material_t* csg_material_create(void);
extern void csg_material_set_color(csg_material_t* mtrl,
                                   csg_material_color_e type, float r, float g,
                                   float b, float a);

/*
 * Drawables
 */
extern csg_drawable_t* csg_drawable_create(csg_geometry_t* geom,
                                           csg_material_t* mtrl);
