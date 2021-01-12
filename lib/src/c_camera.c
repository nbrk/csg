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

  cam->transform = NULL;

  //  // initial view matrix
  //  glm_lookat(cam->position, cam->direction, cam->up, cam->view_matrix);
  glm_mat4_identity(cam->view_matrix);

  // initial proj matrix
  if (proj == CSG_CAMERA_PROJECTION_ORTHOGRAPHIC)
    glm_ortho_default(aspect, cam->projection_matrix);
  if (proj == CSG_CAMERA_PROJECTION_PERSPECTIVE)
    glm_perspective_default(aspect, cam->projection_matrix);

  return cam;
}

void csg_camera_set_transform(csg_camera_t* cam, csg_transform_t* trans) {
  cam->transform = trans;
}
