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

#include "internal.h"

csg_camera_t csg_camera_create(csg_projection_mode_e projection,
                               csg_vec3_t position, csg_vec3_t target,
                               csg_vec3_t up, float aspect) {
  //  csg_camera_t* camera = csg_malloc(sizeof(*camera));
  csg_camera_t camera;
  camera.projection = projection;
  camera.position = position;
  camera.target = target;
  camera.up = up;
  camera.aspect = aspect;

  return camera;
}

csg_mat4_t csg_camera_calc_projection_matrix(csg_camera_t camera) {
  mat4s m;
  if (camera.projection == CSG_PROJECTION_PERSPECTIVE)
    m = glms_perspective_default(camera.aspect);
  if (camera.projection == CSG_PROJECTION_ORTHOGRAPHIC)
    m = glms_ortho_default(camera.aspect);
  return *(csg_mat4_t*)&m;
}

csg_mat4_t csg_camera_calc_view_matrix(csg_camera_t camera) {
  mat4s m;
  m = glms_lookat(*(vec3s*)&camera.position, *(vec3s*)&camera.target,
                  *(vec3s*)&camera.up);
  return *(csg_mat4_t*)&m;
}

csg_camera_t csg_camera_default(void) {
  return csg_camera_create(
      CSG_PROJECTION_PERSPECTIVE, (csg_vec3_t){0.f, 0.f, 10.f},
      (csg_vec3_t){0.f, 0.f, 0.f}, (csg_vec3_t){0.f, 1.f, 0.f}, 4.0f / 3.0f);
}

csg_camera_t csg_camera_default_ortho(void) {
  return csg_camera_create(
      CSG_PROJECTION_ORTHOGRAPHIC, (csg_vec3_t){0.f, 0.f, 10.f},
      (csg_vec3_t){0.f, 0.f, 0.f}, (csg_vec3_t){0.f, 1.f, 0.f}, 4.0f / 3.0f);
}
