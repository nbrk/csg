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
#include <csg/core.h>
#include <stdio.h>

static void do_render_node(csg_node_t* node, csg_mat4_t projection,
                           csg_mat4_t view, csg_mat4_t model) {
  //  printf("Render node %p, string cookie: %s\n", node, node->cookie);
  assert(node->geometry != NULL);

  // query the uniforms' locations
  GLuint program = node->geometry->material.gl_program;
  GLuint u_diffuse_color = glGetUniformLocation(program, "u_diffuse_color");
  GLuint u_model = glGetUniformLocation(program, "u_model");
  GLuint u_view = glGetUniformLocation(program, "u_view");
  GLuint u_projection = glGetUniformLocation(program, "u_projection");

  // set up the uniforms
  glUseProgram(program);
  glUniform4fv(u_diffuse_color, 1,
               (GLfloat*)&node->geometry->material.diffuse_color);
  glUniformMatrix4fv(u_model, 1, GL_FALSE, (GLfloat*)&model);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, (GLfloat*)&view);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, (GLfloat*)&projection);

  // draw arrays / elements
  glBindVertexArray(node->geometry->gl_vao);
  if (node->geometry->gl_indexed_drawing == true)
    glDrawElements(node->geometry->gl_draw_mode, node->geometry->num_vertices,
                   GL_UNSIGNED_INT, NULL);
  else
    glDrawArrays(node->geometry->gl_draw_mode, 0, node->geometry->num_vertices);

  glBindVertexArray(0);
}

static void render_node(csg_node_t* node, csg_mat4_t projection,
                        csg_mat4_t view, csg_mat4_t model) {
  // update the given model matrix with our transform
  model =
      csg_mat4_mul(csg_transform_calc_model_matrix(&node->transform), model);

  // do the actual rendering if the node has geometry
  if (node->geometry != NULL) do_render_node(node, projection, view, model);

  // propagate to children
  for (size_t i = 0; i < node->num_children; i++)
    render_node(node->children[i], projection, view, model);
}

void csg_render(csg_node_t* root, csg_camera_t* camera,
                csg_vec4_t clear_color) {
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (camera != NULL && root != NULL) {
    csg_mat4_t projection = csg_camera_calc_projection_matrix(camera);
    csg_mat4_t view = csg_camera_calc_view_matrix(camera);
    csg_mat4_t model = csg_mat4_identity();
    render_node(root, projection, view, model);
  }
}