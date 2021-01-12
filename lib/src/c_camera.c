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

csg_camera_t* csg_camera_create(csg_camera_projection_e proj, float aspect) {
  csg_camera_t* cam = csg_malloc(sizeof(*cam));

  cam->aspect = aspect;
  cam->position[0] = 0.0f;
  cam->position[1] = 0.0f;
  cam->position[2] = 0.0f;
  cam->direction[0] = 0.0f;
  cam->direction[1] = 0.0f;
  cam->direction[2] = -1.0f;
  cam->up[0] = 0.0f;
  cam->up[1] = 1.0f;
  cam->up[2] = 0.0f;

  // initial view matrix
  glm_lookat(cam->position, cam->direction, cam->up, cam->view_matrix);
  //  glm_mat4_identity(cam->view_matrix);

  // initial proj matrix
  if (cam->projection_type == CSG_CAMERA_PROJECTION_ORTHOGRAPHIC)
    glm_ortho_default(cam->aspect, cam->projection_matrix);
  if (cam->projection_type == CSG_CAMERA_PROJECTION_PERSPECTIVE)
    glm_perspective_default(cam->aspect, cam->projection_matrix);

  return cam;
}

void csg_camera_translate(csg_camera_t* cam, float dx, float dy, float dz) {
  glm_vec3_add(cam->position, (float[]){dx, dy, dz}, cam->position);  // XXX
  glm_translate(cam->view_matrix, (float[]){dx, dy, -dz});
}
