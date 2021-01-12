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

#include "csg_internal.h"

csg_drawable_t* csg_drawable_create(csg_geometry_t* geom,
                                    csg_material_t* mtrl) {
  if (geom == NULL || mtrl == NULL) return NULL;

  csg_drawable_t* draw = csg_malloc(sizeof(*draw));
  draw->geometry = geom;
  draw->material = mtrl;

  return draw;
}

void drawable_draw(csg_drawable_t* draw, mat4 model_matrix, mat4 view_matrix,
                   mat4 projection_matrix) {
  assert(draw->geometry != NULL);
  assert(draw->material != NULL);

  //  glm_mat4_identity(model_matrix);
  //  glm_mat4_identity(view_matrix);
  //  glm_mat4_identity(projection_matrix);

  GLuint u_diffuse_color =
      glGetUniformLocation(draw->material->program, "u_diffuse_color");
  GLuint u_model = glGetUniformLocation(draw->material->program, "u_model");
  GLuint u_view = glGetUniformLocation(draw->material->program, "u_view");
  GLuint u_projection =
      glGetUniformLocation(draw->material->program, "u_projection");

  glUseProgram(draw->material->program);

  glUniform4f(u_diffuse_color, draw->material->diffuse_color[0],
              draw->material->diffuse_color[1],
              draw->material->diffuse_color[2],
              draw->material->diffuse_color[3]);
  glUniformMatrix4fv(u_model, 1, GL_FALSE, model_matrix[0]);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, view_matrix[0]);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, projection_matrix[0]);

  glBindVertexArray(draw->geometry->vao);
  if (draw->geometry->indexed_drawing == true)
    glDrawElements(draw->geometry->draw_mode, draw->geometry->num_vertices,
                   GL_UNSIGNED_INT, NULL);
  else
    glDrawArrays(draw->geometry->draw_mode, 0, draw->geometry->num_vertices);
  glBindVertexArray(0);
  glUseProgram(0);
}
