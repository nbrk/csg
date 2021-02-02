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

#include <GL/glew.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 *
 * C Scene-Graph (core functionality)
 *
 */

/******************************************************************************
 *
 * CONSTANTS/ENUMS
 */
#define CSG_FLAG_ENABLED 1

#define CSG_GEOMETRY_FLAG_HAS_POSITION_DATA 2
#define CSG_GEOMETRY_FLAG_HAS_NORMAL_DATA 4
#define CSG_GEOMETRY_FLAG_HAS_TEXCOORD_DATA 8
#define CSG_GEOMETRY_FLAG_HAS_COLOR_DATA 16
#define CSG_GEOMETRY_FLAG_HAS_INDICES 32

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
  float horizontal_angle;  // yaw
  float vertical_angle;    // pitch
  float fov;
  float aspect;
  float near_plane;
  float far_plane;
} csg_camera_t;

// TODO: textures
typedef struct {
  int flags;
  struct {
    unsigned texo;  // texture object (texture ID)
    // TODO
  } gl;
} csg_texture_t;

// TODO: material system
typedef struct {
  int flags;
  csg_texture_t texture;
  csg_vec4_t diffuse_color;
  unsigned gl_program;
} csg_material_t;

typedef struct {
  int flags;
  int num_indices;
  struct {
    GLenum draw_mode;
    GLenum polygon_mode;
    GLuint ibo;
    GLuint position_vbo;
    GLuint normal_vbo;
    GLuint texcoord_vbo;
    GLuint color_vbo;
  } gl;
  csg_material_t material;
} csg_geometry_t;

typedef struct csg_node_t csg_node_t;
struct csg_node_t {
  csg_node_t* parent;
  csg_node_t** children;
  size_t num_children;
  void* cookie;
  csg_transform_t transform;
  csg_geometry_t geometry;
};

/******************************************************************************
 *
 * FUNCTIONS
 */
extern void* csg_calloc_dbg(size_t number, size_t size, const char* file,
                            int line);
extern void* csg_malloc_dbg(size_t size, const char* file, int line);
extern void* csg_realloc_dbg(void* mem, size_t size, const char* file,
                             int line);
extern void csg_free_dbg(void* mem, const char* file, int line);
#define csg_malloc(x) csg_malloc_dbg(x, __FILE__, __LINE__)
#define csg_calloc(x, y) csg_calloc_dbg(x, y, __FILE__, __LINE__)
#define csg_realloc(x, y) csg_realloc_dbg(x, y, __FILE__, __LINE__)
#define csg_free(x) csg_free_dbg(x, __FILE__, __LINE__)

extern void csg_malloc_set_debug(bool value);
extern void csg_malloc_print_stat(void);

extern csg_node_t* csg_node_create(csg_node_t* parent, void* cookie);
extern void csg_node_traverse(csg_node_t* node, csg_traverse_mode_e type,
                              void (*func)(csg_node_t*, void*),
                              void* func_cookie);
extern csg_mat4_t csg_transform_calc_model_matrix(csg_transform_t* transform);

csg_camera_t csg_camera_create(csg_projection_mode_e projection,
                               csg_vec3_t position, float horiz_angle,
                               float vert_angle, float fov, float aspect);
/**
 * @brief Create the default camera: positioned at +10 (Z), looking at the
 * origin, perspective, aspect 4:3
 * @return
 */
extern csg_camera_t csg_camera_default(void);
extern csg_camera_t csg_camera_default_ortho(void);
extern csg_vec3_t csg_camera_get_direction(csg_camera_t camera);
extern csg_vec3_t csg_camera_get_right(csg_camera_t camera);
extern void csg_camera_move_by(csg_camera_t* camera, float forward,
                               float strafe, float jump);

extern void csg_render(csg_node_t* root, csg_camera_t camera,
                       csg_vec4_t clear_color);

extern csg_geometry_t csg_geometry_none(void);
extern csg_geometry_t csg_geometry_create_triangle(void);
extern csg_geometry_t csg_geometry_create_cube(void);
extern csg_geometry_t csg_geometry_create_octahedron(void);
extern csg_geometry_t csg_geometry_create_sphere(int gradation);
extern csg_geometry_t csg_geometry_create_quad(void);

extern csg_texture_t csg_texture_none(void);

extern csg_material_t csg_material_create(void);
extern csg_material_t csg_material_none(void);
