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
#include <assert.h>
#include <string.h>

#include "csg_internal.h"

csg_viewport_t* csg_viewport_create(int x, int y, int width, int height,
                                    csg_camera_t* cam) {
  if (cam == NULL) return NULL;

  csg_viewport_t* view = csg_malloc(sizeof(*view));

  csg_viewport_reset(view, x, y, width, height);
  view->camera = cam;
  view->clear_color[0] = 0.0f;
  view->clear_color[1] = 0.0f;
  view->clear_color[2] = 0.0f;
  view->clear_color[3] = 1.0f;

  return view;
}

void csg_viewport_set_clear_color(csg_viewport_t* view, float r, float g,
                                  float b, float a) {
  view->clear_color[0] = r;
  view->clear_color[1] = g;
  view->clear_color[2] = b;
  view->clear_color[3] = a;
}

static void render_forest(csg_node_t* node, mat4 view_matrix,
                          mat4 projection_matrix) {
  // if no transform is set, consider the identity matrix for the node
  mat4 model_matrix;
  if (node->transform != NULL)
    glm_mat4_copy(node->transform->matrix, model_matrix);
  else
    glm_mat4_identity(model_matrix);

  if (node->parent != NULL && node->parent->transform != NULL)
    // incorporate parent's model matrix
    glm_mat4_mul(node->parent->transform->matrix, model_matrix, model_matrix);

  if (node->drawable != NULL)
    // if we have the drawable then draw it using the combined model matrix
    drawable_draw(node->drawable, model_matrix, view_matrix, projection_matrix);

  for (size_t i = 0; i < node->num_children; i++) {
    render_forest(node->children[i], view_matrix, projection_matrix);
    // restore model matrix
    if (node->transform != NULL)
      glm_mat4_copy(node->transform->matrix, model_matrix);
  }
}

void csg_viewport_render(csg_viewport_t* view, csg_node_t* root) {
  glClearColor(view->clear_color[0], view->clear_color[1], view->clear_color[2],
               view->clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (root == NULL) return;

  // apply the camera's model matrix, if any
  mat4 camera_model_matrix = GLM_MAT4_IDENTITY_INIT;
  if (view->camera->transform != NULL)
    glm_mat4_mul(view->camera->transform->matrix, view->camera->view_matrix,
                 camera_model_matrix);

  render_forest(root, camera_model_matrix, view->camera->projection_matrix);
}

void csg_viewport_reset(csg_viewport_t* view, int x, int y, int width,
                        int height) {
  glViewport(x, y, width, height);
  view->x = x;
  view->y = y;
  view->width = width;
  view->height = height;
}
