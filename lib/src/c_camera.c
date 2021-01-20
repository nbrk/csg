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

csg_camera_t* csg_camera_create(float aspect) {
  csg_camera_t* cam = csg_malloc(sizeof(*cam));

  glm_mat4_identity(cam->view_matrix);
  glm_mat4_identity(cam->projection_matrix);

  // initial camera position/orientation
  cam->position[0] = 0.f;
  cam->position[1] = 0.f;
  cam->position[2] = 10.f;
  cam->center[0] = 0.f;
  cam->center[1] = 0.f;
  cam->center[2] = 0.f;
  cam->up[0] = 0.f;
  cam->up[1] = 1.f;
  cam->up[2] = 0.f;

  cam->projection = CSG_CAMERA_PROJECTION_PERSPECTIVE;
  cam->aspect = aspect;

  // initial view and proj matrices
  glm_lookat(cam->position, cam->center, cam->up, cam->view_matrix);
  glm_perspective_default(cam->aspect, cam->projection_matrix);

  //  glm_mat4_identity(cam->view_matrix);  // XXX

  return cam;
}

void csg_camera_set_projection(csg_camera_t* cam,
                               csg_camera_projection_e proj) {
  cam->projection = proj;

  // recompute the projection matrix
  if (cam->projection == CSG_CAMERA_PROJECTION_ORTHOGRAPHIC)
    glm_ortho_default(cam->aspect, cam->projection_matrix);
  if (cam->projection == CSG_CAMERA_PROJECTION_PERSPECTIVE)
    glm_perspective_default(cam->aspect, cam->projection_matrix);
}

void csg_camera_set_position(csg_camera_t* cam,
                             float x,
                             float y,
                             float z,
                             bool keep_center) {
  cam->position[0] = x;
  cam->position[1] = y;
  cam->position[2] = z;

  // keep the center, or just move the camera around it
  if (keep_center == false)
    glm_vec3_add(cam->position, (float[]){0.f, 0.f, -1.f}, cam->center);
  // XXX TODO:
  //    glm_vec3_add(cam->position, , cam->center);

  // recompute the view matrix
  glm_lookat(cam->position, cam->center, cam->up, cam->view_matrix);
}

void csg_camera_set_center(csg_camera_t* cam, float x, float y, float z) {
  cam->center[0] = x;
  cam->center[1] = y;
  cam->center[2] = z;

  // recompute the view matrix
  glm_lookat(cam->position, cam->center, cam->up, cam->view_matrix);
}

void csg_camera_get_position(csg_camera_t* cam, float* x, float* y, float* z) {
  *x = cam->position[0];
  *y = cam->position[1];
  *z = cam->position[2];
}

void csg_camera_get_center(csg_camera_t* cam, float* x, float* y, float* z) {
  *x = cam->center[0];
  *y = cam->center[1];
  *z = cam->center[2];
}
