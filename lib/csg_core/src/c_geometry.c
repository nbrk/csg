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
#include <csg/core.h>
#include <math.h>
#include <stdio.h>

csg_geometry_t csg_geometry_none(void) {
  csg_geometry_t geom;

  geom.flags = 0;
  geom.num_to_draw = 0;
  geom.gl.draw_mode = 0;
  geom.gl.vertices_vbo = 0;
  geom.gl.vertices_ibo = 0;
  geom.gl.texcoords_vbo = 0;
  geom.gl.texcoords_ibo = 0;
  geom.gl.normals_vbo = 0;
  geom.gl.normals_ibo = 0;
  geom.material = csg_material_none();

  return geom;
}

csg_geometry_t csg_geometry_create_triangle(void) {
  csg_geometry_t geom = csg_geometry_none();

  geom.flags = CSG_GEOMETRY_FLAG_ENABLED;
  geom.num_to_draw = 3;
  geom.gl.draw_mode = GL_TRIANGLES;
  geom.gl.polygon_mode = GL_FILL;
  geom.material = csg_material_none();

  glGenBuffers(1, &geom.gl.vertices_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geom.gl.vertices_vbo);
  glBufferData(
      GL_ARRAY_BUFFER, 9 * sizeof(GLfloat),
      (float[9]){-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f},
      GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return geom;
}

csg_geometry_t csg_geometry_create_cube(void) {
  csg_geometry_t geom = csg_geometry_none();

  geom.flags = CSG_GEOMETRY_FLAG_ENABLED | CSG_GEOMETRY_FLAG_INDEXED_DRAW;
  geom.num_to_draw = 36;  // XXX
  geom.gl.draw_mode = GL_TRIANGLES;
  geom.gl.polygon_mode = GL_FILL;
  geom.material = csg_material_none();

  glGenBuffers(1, &geom.gl.vertices_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geom.gl.vertices_vbo);
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

  glGenBuffers(1, &geom.gl.vertices_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.gl.vertices_ibo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint),
      (GLuint[36]){0, 1, 2, 2, 3, 1, 4, 5, 6, 6, 7, 4, 0, 4, 7, 7, 3, 0,
                   1, 5, 6, 6, 3, 1, 1, 5, 4, 4, 0, 1, 3, 7, 6, 6, 2, 3},
      GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return geom;
}

csg_geometry_t csg_geometry_create_sphere(int gradation) {
  csg_geometry_t geom = csg_geometry_none();

  geom.flags = CSG_GEOMETRY_FLAG_ENABLED;
  geom.gl.draw_mode = GL_TRIANGLE_STRIP;
  geom.gl.polygon_mode = GL_FILL;
  geom.material = csg_material_none();

  // client data buffer
  const float PI = 3.141592f;
  GLfloat x, y, z, alpha, beta;  // Storage for coordinates and angles
  GLfloat radius = 1.0f;
  gradation = 50;

  float* vertex_data = NULL;
  size_t num_vertices = 0;

  for (alpha = 0.0; alpha < M_PI; alpha += PI / gradation) {
    for (beta = 0.0; beta < 2.01 * M_PI; beta += PI / gradation) {
      vertex_data = csg_realloc(
          vertex_data, num_vertices * sizeof(float) * 3 + sizeof(float) * 3);

      x = radius * cos(beta) * sin(alpha);
      y = radius * sin(beta) * sin(alpha);
      z = radius * cos(alpha);
      vertex_data[num_vertices * 3 + 0] = x;
      vertex_data[num_vertices * 3 + 1] = y;
      vertex_data[num_vertices * 3 + 2] = z;
      num_vertices++;

      x = radius * cos(beta) * sin(alpha + PI / gradation);
      y = radius * sin(beta) * sin(alpha + PI / gradation);
      z = radius * cos(alpha + PI / gradation);
      vertex_data = csg_realloc(
          vertex_data, num_vertices * sizeof(float) * 3 + sizeof(float) * 3);
      vertex_data[num_vertices * 3 + 0] = x;
      vertex_data[num_vertices * 3 + 1] = y;
      vertex_data[num_vertices * 3 + 2] = z;
      num_vertices++;
    }
  }

  // hardware
  geom.num_to_draw = num_vertices;

  glGenBuffers(1, &geom.gl.vertices_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geom.gl.vertices_vbo);
  glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(GLfloat), vertex_data,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return geom;
}

csg_geometry_t csg_geometry_create_octahedron(void) {
  return csg_geometry_create_sphere(2);
}
