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
#include <cglm/struct.h>
#include <csg/core.h>

static mat4s calc_translation_matrix(csg_transform_t* transform) {
  mat4s m;
  m = glms_translate_make(*(vec3s*)&transform->translation);
  return m;
}

static mat4s calc_rotation_matrix(csg_transform_t* transform) {
  mat4s m;
  m = glms_rotate_make(0.0f, (vec3s){.x = 0.0f, .y = 0.0f, .z = 0.0f});
  m = glms_rotate_x(m, transform->rotation.x);
  m = glms_rotate_y(m, transform->rotation.y);
  m = glms_rotate_z(m, transform->rotation.z);
  return m;
}

static mat4s calc_scale_matrix(csg_transform_t* transform) {
  mat4s m;
  m = glms_scale_make(*(vec3s*)&transform->scale);
  return m;
}
csg_mat4_t csg_transform_calc_model_matrix(csg_transform_t* transform) {
  mat4s m;

  mat4s matrices[3];
  matrices[0] = calc_translation_matrix(transform);
  matrices[1] = calc_rotation_matrix(transform);
  matrices[2] = calc_scale_matrix(transform);

  // M := T * R * S
  m = glms_mat4_mulN((mat4s*[]){&matrices[0], &matrices[1], &matrices[2]}, 3);
  return *(csg_mat4_t*)&m;
}
