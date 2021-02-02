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

static void set_uniforms(csg_node_t* node, csg_mat4_t* projection,
                         csg_mat4_t* view, csg_mat4_t* model) {
  // query the uniforms' locations
  GLuint program = node->geometry.material.gl_program;
  GLuint u_diffuse_color = glGetUniformLocation(program, "u_diffuse_color");
  GLuint u_model = glGetUniformLocation(program, "u_model");
  GLuint u_view = glGetUniformLocation(program, "u_view");
  GLuint u_projection = glGetUniformLocation(program, "u_projection");
  GLint u_use_textures = glGetUniformLocation(program, "u_use_textures");

  glUniform4fv(u_diffuse_color, 1,
               (GLfloat*)&node->geometry.material.diffuse_color);
  glUniformMatrix4fv(u_model, 1, GL_FALSE, (GLfloat*)model);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, (GLfloat*)view);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, (GLfloat*)projection);
  glUniform1i(u_use_textures,
              (node->geometry.material.texture.flags & CSG_FLAG_ENABLED) != 0);
}

static void do_render_node(csg_node_t* node, csg_mat4_t projection,
                           csg_mat4_t view, csg_mat4_t model) {
  assert(node->geometry.flags & CSG_FLAG_ENABLED);
  assert(node->geometry.material.flags & CSG_FLAG_ENABLED);

  glUseProgram(node->geometry.material.gl_program);

  /*
   * Set uniforms
   */
  set_uniforms(node, &projection, &view, &model);

  /*
   * Setup vertex attributes streaming (via either elements or verbatim). We use
   * separate, optional data buffers. Attrib layout.locations in the shaders
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
   * Bind texture if the material has one
   */
  if (node->geometry.material.flags & CSG_FLAG_ENABLED) {
    glBindTexture(GL_TEXTURE_2D, node->geometry.material.texture.gl.texo);
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

static void render_node(csg_node_t* node, csg_mat4_t projection,
                        csg_mat4_t view, csg_mat4_t model) {
  // update the given model matrix with our transform
  model = from_glm_mat4s(glms_mat4_mul(
      from_csg_mat4(csg_transform_calc_model_matrix(&node->transform)),
      from_csg_mat4(model)));

  // do the actual rendering if the node has geometry
  if ((node->geometry.flags & CSG_FLAG_ENABLED) != 0 &&
      (node->geometry.material.flags & CSG_FLAG_ENABLED) != 0)
    do_render_node(node, projection, view, model);

  // propagate to children
  for (size_t i = 0; i < node->num_children; i++)
    render_node(node->children[i], projection, view, model);
}

void csg_render(csg_node_t* root, csg_camera_t camera, csg_vec4_t clear_color) {
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  csg_mat4_t projection = csg_camera_calc_projection_matrix(camera);
  csg_mat4_t view = csg_camera_calc_view_matrix(camera);
  csg_mat4_t model = from_glm_mat4s(glms_mat4_identity());

  if (root != NULL) render_node(root, projection, view, model);
}
