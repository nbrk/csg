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
#include <GL/glew.h>
#include <assert.h>
#include <cglm/struct.h>
#include <stdio.h>

#include "internal.h"

static csg_mat4_t from_glm_mat4s(mat4s m) { return *(csg_mat4_t*)&m; }

static mat4s from_csg_mat4(csg_mat4_t m) { return *(mat4s*)&m; }

static void render_node(csg_node_t* node, csg_mat4_t projection,
                        csg_mat4_t view, csg_mat4_t model) {
  assert(node->geometry.flags & CSG_FLAG_ENABLED);
  assert(node->shader.flags & CSG_FLAG_ENABLED);

  glUseProgram(node->shader.program);

  /*
   * Set uniforms
   */
  node->shader.u_projection = projection;
  node->shader.u_view = view;
  node->shader.u_model = model;
  csg_shader_set_all_uniforms(&node->shader);

  /*
   * Setup vertex attributes streaming (via either elements or verbatim). We use
   * separate, optional data buffers. Attrib layout/locations in the shaders
   * are: 0 - position data, 1 - normal data, 2 - texcoord data, 3 - vertex
   * color data
   */
  if (node->geometry.flags & CSG_GEOMETRY_FLAG_HAS_POSITION_DATA) {
    glBindBuffer(GL_ARRAY_BUFFER, node->geometry.gl.position_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  if (node->geometry.flags & CSG_GEOMETRY_FLAG_HAS_NORMAL_DATA) {
    glBindBuffer(GL_ARRAY_BUFFER, node->geometry.gl.normal_vbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  if (node->geometry.flags & CSG_GEOMETRY_FLAG_HAS_TEXCOORD_DATA) {
    glBindBuffer(GL_ARRAY_BUFFER, node->geometry.gl.texcoord_vbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  if (node->geometry.flags & CSG_GEOMETRY_FLAG_HAS_COLOR_DATA) {
    glBindBuffer(GL_ARRAY_BUFFER, node->geometry.gl.color_vbo);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  /*
   * Bind texture if we have one
   */
  if (node->texture.flags & CSG_FLAG_ENABLED) {
    //    glActiveTexture(GL_TEXTURE0);
    glBindTexture(node->texture.gl.type, node->texture.gl.texo);
  }

  /*
   * Stream the above attrib data: draw elements or arrays (verbatim data as-is)
   */
  glPolygonMode(GL_FRONT_AND_BACK, node->geometry.gl.polygon_mode);

  if (node->geometry.flags & CSG_GEOMETRY_FLAG_HAS_INDICES) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, node->geometry.gl.ibo);
    glDrawElements(node->geometry.gl.draw_mode, node->geometry.num_indices,
                   GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  } else {
    glDrawArrays(node->geometry.gl.draw_mode, 0, node->geometry.num_indices);
  }
}

static void process_node(csg_node_t* node, csg_mat4_t projection,
                         csg_mat4_t view, csg_mat4_t model) {
  /*
   * Apply our model transforms to the current matrix, even if we don't
   * have something to draw to the screen.
   */
  model = from_glm_mat4s(glms_mat4_mul(
      from_csg_mat4(csg_transform_calc_model_matrix(&node->transform)),
      from_csg_mat4(model)));

  /*
   * If we have an enabled geometry and a shader, then draw ourselfes
   */
  if ((node->geometry.flags & CSG_FLAG_ENABLED) != 0 &&
      (node->shader.flags & CSG_FLAG_ENABLED) != 0)
    render_node(node, projection, view, model);

  /*
   * Recursively process children proagating the updated model matrix further.
   */
  for (size_t i = 0; i < node->num_children; i++)
    process_node(node->children[i], projection, view, model);
}

void csg_render(csg_node_t* root, csg_camera_t camera, csg_vec4_t clear_color) {
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  csg_mat4_t projection = csg_camera_calc_projection_matrix(camera);
  csg_mat4_t view = csg_camera_calc_view_matrix(camera);
  csg_mat4_t model = from_glm_mat4s(glms_mat4_identity());

  if (root != NULL) process_node(root, projection, view, model);
}
