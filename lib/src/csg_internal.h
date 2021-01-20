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
#include <cglm/cglm.h>
#include <csg/csg.h>

struct csg_viewport_t {
  GLint x;
  GLint y;
  GLsizei width;
  GLsizei height;
  float clear_color[4];
  csg_camera_t* camera;
};

struct csg_camera_t {
  csg_camera_projection_e projection;
  float aspect;

  vec3 position;
  //  vec3 direction;
  vec3 center;
  vec3 up;

  mat4 view_matrix;
  mat4 projection_matrix;
};

struct csg_node_t {
  csg_node_t* parent;
  csg_node_t** children;
  size_t num_children;

  void* cookie;
  csg_transform_t* transform;
  csg_drawable_t* drawable;
};

struct csg_transform_t {
  csg_animation_t* translation_animation;
  csg_animation_t* rotation_animation;
  csg_animation_t* scale_animation;

  mat4 matrix;
  mat4 translation_matrix;
  mat4 rotation_matrix;
  mat4 scale_matrix;

  vec4 position;
};

struct csg_geometry_t {
  int num_vertices;
  bool indexed_drawing;
  GLenum draw_mode;
  GLuint vao;
};

struct csg_material_t {
  GLuint program;
  float diffuse_color[4];
};

struct csg_drawable_t {
  csg_geometry_t* geometry;
  csg_material_t* material;
};

struct csg_animation_t {
  vec4 current_value;
  vec4* waypoints;
  int num_waypoints;
  int current_segment;
  int total_segments;
  float interpolant;

  csg_animation_mode_e mode;
  int flags;
};

extern void drawable_draw(csg_drawable_t* draw,
                          mat4 model_matrix,
                          mat4 view_matrix,
                          mat4 projection_matrix);
