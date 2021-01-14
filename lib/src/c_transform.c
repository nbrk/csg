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

#include "csg_internal.h"

void calc_matrix(csg_transform_t* trans) {
  //  glm_mat4_mul(trans->translation_matrix, trans->rotation_matrix,
  //               trans->matrix);
  glm_mat4_mulN((mat4*[]){&trans->translation_matrix, &trans->rotation_matrix,
                          &trans->scale_matrix},
                3, trans->matrix);
}

csg_transform_t* csg_transform_create(void) {
  csg_transform_t* trans = csg_malloc(sizeof(*trans));

  trans->translation_animation = NULL;
  trans->rotation_animation = NULL;
  trans->scale_animation = NULL;
  glm_mat4_identity(trans->matrix);
  glm_translate_make(trans->translation_matrix, (float[]){0.f, 0.f, 0.f, 1.0f});
  glm_rotate_make(trans->rotation_matrix, 0.0f, (float[]){0.f, 0.f, 0.f});
  glm_scale_make(trans->scale_matrix, (float[]){1.0f, 1.0f, 1.0f});

  trans->position[0] = 0.f;
  trans->position[1] = 0.f;
  trans->position[2] = 0.f;
  trans->position[3] = 1.f;

  return trans;
}

csg_transform_t* csg_transform_create_translated(float orig_x, float orig_y,
                                                 float orig_z) {
  csg_transform_t* trans = csg_transform_create();
  csg_transform_translate(trans, orig_x, orig_y, orig_z);

  return trans;
}

void csg_transform_translate(csg_transform_t* trans, float dx, float dy,
                             float dz) {
  vec4 dv;
  dv[0] = dx;
  dv[1] = dy;
  dv[2] = dz;
  dv[3] = 1.0f;
  glm_vec4_add(trans->position, dv, trans->position);
  glm_translate(trans->translation_matrix, (float[]){dx, dy, dz});
  calc_matrix(trans);
}

void csg_transform_set_translation_animation(csg_transform_t* trans,
                                             csg_animation_t* anim) {
  trans->translation_animation = anim;
}

void csg_transform_update_translation_animation(csg_transform_t* trans,
                                                float delta) {
  if (trans->translation_animation == NULL) return;

  csg_animation_update(trans->translation_animation, delta);

  // XXX
  glm_translate_make(trans->translation_matrix,
                     trans->translation_animation->current_value);
  glm_translate(trans->translation_matrix, trans->position);

  calc_matrix(trans);
}

void csg_transform_get_position(csg_transform_t* trans, float* x, float* y,
                                float* z) {
  *x = trans->position[0];
  *y = trans->position[1];
  *z = trans->position[2];
}

void csg_transform_rotate(csg_transform_t* trans, float angle_rad, float x,
                          float y, float z) {
  vec3 axis;
  axis[0] = x;
  axis[1] = y;
  axis[2] = z;
  //    glm_rotate(trans->combined_matrix, angle_rad, axis);
  glm_rotate(trans->rotation_matrix, angle_rad, axis);
  calc_matrix(trans);
}

void csg_transform_scale(csg_transform_t* trans, float sx, float sy, float sz) {
  glm_scale(trans->scale_matrix, (float[]){sx, sy, sz});
  calc_matrix(trans);
}
