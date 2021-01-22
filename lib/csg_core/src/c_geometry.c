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
#include <csg/core.h>

csg_geometry_t* csg_geometry_create_triangle(void) {
  csg_geometry_t* geom = csg_malloc(sizeof(*geom));

  geom->num_vertices = 3;
  geom->gl.draw_mode = GL_TRIANGLES;
  geom->indexed_drawing = false;

  glGenBuffers(1, &geom->gl.position_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geom->gl.position_vbo);
  glBufferData(
      GL_ARRAY_BUFFER, 9 * sizeof(GLfloat),
      (float[9]){-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f},
      GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return geom;
}

csg_geometry_t* csg_geometry_create_cube(void) {
  csg_geometry_t* geom = csg_malloc(sizeof(*geom));

  geom->num_vertices = 36;  // XXX
  geom->gl.draw_mode = GL_TRIANGLES;
  geom->indexed_drawing = true;

  glGenBuffers(1, &geom->gl.position_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geom->gl.position_vbo);
  glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat),
               (float[24]){
                   // bottom half, cw from near-left, [0]
                   -0.5f,
                   -0.5f,
                   0.5f,
                   -0.5f,
                   -0.5f,
                   -0.5f,
                   0.5f,
                   -0.5f,
                   -0.5f,
                   0.5f,
                   -0.5f,
                   0.5f,
                   // top half, cw from near-left, [4]
                   -0.5f,
                   0.5f,
                   0.5f,
                   -0.5f,
                   0.5f,
                   -0.5f,
                   0.5f,
                   0.5f,
                   -0.5f,
                   0.5f,
                   0.5f,
                   0.5f,
               },
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &geom->gl.position_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->gl.position_ibo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint),
      (GLuint[36]){0, 1, 2, 2, 3, 1, 4, 5, 6, 6, 7, 4, 0, 4, 7, 7, 3, 0,
                   1, 5, 6, 6, 3, 1, 1, 5, 4, 4, 0, 1, 3, 7, 6, 6, 2, 3},
      GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  //  glEnableVertexAttribArray(0);
  //  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  return geom;
}
