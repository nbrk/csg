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
                               csg_vec3_t position, float horiz_angle,
                               float vert_angle, float fov, float aspect) {
  csg_camera_t camera;
  camera.projection = projection;
  camera.position = position;
  camera.horizontal_angle = horiz_angle;
  camera.vertical_angle = vert_angle;
  camera.aspect = aspect;
  camera.fov = fov;

  return camera;
}

csg_mat4_t csg_camera_calc_projection_matrix(csg_camera_t camera) {
  mat4s m;
  if (camera.projection == CSG_PROJECTION_PERSPECTIVE)
    m = glms_perspective(glm_rad(camera.fov), camera.aspect, 0.1f, 100.0f);
  if (camera.projection == CSG_PROJECTION_ORTHOGRAPHIC)
    m = glms_ortho_default(camera.aspect);
  return *(csg_mat4_t*)&m;
}

csg_mat4_t csg_camera_calc_view_matrix(csg_camera_t camera) {
  mat4s m;
  csg_vec3_t direction = csg_camera_get_direction(camera);
  csg_vec3_t right = csg_camera_get_right(camera);
  vec3s up = glms_cross(*(vec3s*)&right, *(vec3s*)&direction);
  m = glms_lookat(*(vec3s*)&camera.position,
                  glms_vec3_add(*(vec3s*)&camera.position, *(vec3s*)&direction),
                  up);
  return *(csg_mat4_t*)&m;
}

csg_camera_t csg_camera_default(void) {
  return csg_camera_create(CSG_PROJECTION_PERSPECTIVE,
                           (csg_vec3_t){0.f, 0.f, 10.f}, M_PI, 0.0f, 45.0f,
                           4.0f / 3.0f);
}

csg_camera_t csg_camera_default_ortho(void) {
  return csg_camera_create(CSG_PROJECTION_ORTHOGRAPHIC,
                           (csg_vec3_t){0.f, 0.f, 10.f}, M_PI, 0.0f, 45.0f,
                           4.0f / 3.0f);
}

csg_vec3_t csg_camera_get_direction(csg_camera_t camera) {
  vec3s direction =
      (vec3s){.x = cos(camera.vertical_angle) * sin(camera.horizontal_angle),
              .y = sin(camera.vertical_angle),
              .z = cos(camera.vertical_angle) * cos(camera.horizontal_angle)};
  return *(csg_vec3_t*)&direction;
}

csg_vec3_t csg_camera_get_right(csg_camera_t camera) {
  vec3s right = (vec3s){.x = sin(camera.horizontal_angle - M_PI_2),
                        .y = 0,
                        .z = cos(camera.horizontal_angle - M_PI_2)};
  return *(csg_vec3_t*)&right;
}

void csg_camera_move_by(csg_camera_t* camera, float forward, float strafe,
                        float jump) {
  csg_vec3_t direction = csg_camera_get_direction(*camera);
  csg_vec3_t right = csg_camera_get_right(*camera);
  vec3s up = glms_cross(*(vec3s*)&right, *(vec3s*)&direction);

  glm_vec3_scale(*(vec3*)&direction, forward, *(vec3*)&direction);
  glm_vec3_scale(*(vec3*)&right, strafe, *(vec3*)&right);
  glm_vec3_scale(*(vec3*)&up, jump, *(vec3*)&up);

  vec3s newposition;
  newposition = glms_vec3_add(*(vec3s*)&camera->position, *(vec3s*)&direction);
  newposition = glms_vec3_add(newposition, *(vec3s*)&right);
  newposition = glms_vec3_add(newposition, *(vec3s*)&up);

  camera->position = *(csg_vec3_t*)&newposition;
}
