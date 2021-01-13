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

csg_transform_t* csg_transform_create(float orig_x, float orig_y,
                                      float orig_z) {
  csg_transform_t* trans = csg_malloc(sizeof(*trans));

  trans->translation_animation = NULL;
  trans->rotation_animation = NULL;
  trans->scaling_animation = NULL;
  glm_mat4_identity(trans->matrix);
  glm_translate(trans->matrix, (float[]){orig_x, orig_y, orig_z});  // XXX

  // XXX
  trans->position[0] = orig_x;
  trans->position[1] = orig_y;
  trans->position[2] = orig_z;
  trans->position[3] = 1.0f;

  return trans;
}

void csg_transform_translate(csg_transform_t* trans, float dx, float dy,
                             float dz) {
  glm_translate(trans->matrix, (float[]){dx, dy, dz});

  trans->position[0] += dx;
  trans->position[1] += dy;
  trans->position[2] += dz;
}

void csg_transform_set_translation_animation(csg_transform_t* trans,
                                             csg_animation_t* anim) {
  trans->translation_animation = anim;
}

void csg_transform_translation_animation_update(csg_transform_t* trans,
                                                float delta) {
  if (trans->translation_animation == NULL) return;

  csg_animation_update(trans->translation_animation, delta);

  //  glm_translate(trans->matrix, trans->translation_animation->current_value);
  glm_translate_make(trans->matrix,
                     trans->translation_animation->current_value);
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
  //  glm_rotate(trans->matrix, angle_rad, axis);
  glm_rotate_make(trans->matrix, angle_rad, axis);
}
