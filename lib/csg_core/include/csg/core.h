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

#include <stdbool.h>
#include <stdlib.h>

/*
 *
 * C Scene-Graph (core functionality)
 *
 */
/******************************************************************************
 *
 * ENUMS
 */
typedef enum {
  CSG_TRAVERSE_ONLY_SELF,
  CSG_TRAVERSE_ONLY_CHILDREN,
  CSG_TRAVERSE_SELF_AND_CHILDREN,
  CSG_TRAVERSE_DEPTH_FIRST,
  CSG_TRAVERSE_BREADTH_FIRST,
  CSG_TRAVERSE_PATH_TO_ROOT,
  CSG_TRAVERSE_PATH_FROM_ROOT,
} csg_traverse_mode_e;

typedef enum {
  CSG_PROJECTION_ORTHOGRAPHIC,
  CSG_PROJECTION_PERSPECTIVE
} csg_projection_mode_e;

/******************************************************************************
 *
 * DATA
 */
typedef struct {
  float x, y, z;
} csg_vec3_t;

typedef struct {
  float x, y, z, w;
} csg_vec4_t;

typedef struct {
  csg_vec4_t col[4];
} csg_mat4_t;

typedef struct {
  csg_vec3_t translation;
  csg_vec3_t rotation;
  csg_vec3_t scale;
} csg_transform_t;

typedef struct {
  csg_projection_mode_e projection;
  csg_vec3_t position;
  csg_vec3_t target;
  csg_vec3_t up;
  float aspect;
} csg_camera_t;

// TODO: material system
typedef struct {
  csg_vec4_t diffuse_color;
  unsigned gl_program;
} csg_material_t;

// TODO: geometry system
typedef struct {
  int num_vertices;
  unsigned gl_vao;
  int gl_draw_mode;
  bool gl_indexed_drawing;
  csg_material_t material;
} csg_geometry_t;

typedef struct csg_node_t csg_node_t;
struct csg_node_t {
  csg_node_t* parent;
  csg_node_t** children;
  size_t num_children;
  void* cookie;
  csg_transform_t transform;
  csg_geometry_t* geometry;
};

/******************************************************************************
 *
 * FUNCTIONS
 */
extern void csg_free(void* mem);
extern void* csg_malloc(size_t size);
extern void csg_print_malloc_stat(void);
extern void* csg_realloc(void* mem, size_t size);
extern void csg_set_malloc_debug(bool val);

extern csg_node_t* csg_node_create(csg_node_t* parent, void* cookie);
extern void csg_node_traverse(csg_node_t* node, csg_traverse_mode_e type,
                              void (*func)(csg_node_t*, void*),
                              void* func_cookie);
extern csg_mat4_t csg_transform_calc_model_matrix(csg_transform_t* transform);

extern csg_camera_t csg_camera_create(csg_projection_mode_e projection,
                                      csg_vec3_t position, csg_vec3_t target,
                                      csg_vec3_t up, float aspect);
/**
 * @brief Create the default camera: positioned at +10 (Z), looking at the
 * origin, perspective, aspect 4:3
 * @return
 */
extern csg_camera_t csg_camera_default(void);
extern csg_mat4_t csg_camera_calc_projection_matrix(csg_camera_t camera);
extern csg_mat4_t csg_camera_calc_view_matrix(csg_camera_t camera);

extern void csg_render(csg_node_t* root, csg_camera_t camera,
                       csg_vec4_t clear_color);

extern csg_geometry_t* csg_geometry_create_triangle(void);
extern csg_geometry_t* csg_geometry_create_cube(void);

extern csg_material_t csg_material_create(void);
