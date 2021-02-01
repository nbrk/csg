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
#include <csg/wavefront.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>

#include "internal_tinyobj_loader_c.h"

static void loadFile(const char* filename, const int is_mtl,
                     const char* obj_filename, char** buffer, size_t* len) {
  long string_size = 0, read_size = 0;
  FILE* handler = fopen(filename, "r");

  if (handler) {
    fseek(handler, 0, SEEK_END);
    string_size = ftell(handler);
    rewind(handler);
    *buffer = (char*)csg_malloc(sizeof(char) * (string_size + 1));
    read_size = fread(*buffer, sizeof(char), (size_t)string_size, handler);
    (*buffer)[string_size] = '\0';
    if (string_size != read_size) {
      csg_free(buffer);
    }
    fclose(handler);
  }

  *len = read_size;
}

csg_geometry_t csg_geometry_create_from_wavefront(const char* path) {
  FILE* fp = fopen(path, "r");
  assert(fp != NULL);

  tinyobj_shape_t* shape = NULL;
  tinyobj_material_t* material = NULL;
  tinyobj_attrib_t attrib;

  unsigned long num_shapes;
  unsigned long num_materials;

  tinyobj_attrib_init(&attrib);

  int result =
      tinyobj_parse_obj(&attrib, &shape, &num_shapes, &material, &num_materials,
                        path, loadFile, TINYOBJ_FLAG_TRIANGULATE);
  fclose(fp);
  assert(result == TINYOBJ_SUCCESS);

  csg_geometry_t geom = csg_geometry_none();
  geom.flags = CSG_FLAG_ENABLED;
  geom.gl.draw_mode = GL_TRIANGLES;
  geom.gl.polygon_mode = GL_FILL;
  geom.material = csg_material_none();
  geom.num_indices = attrib.num_faces;

  // VBO
  glGenBuffers(1, &geom.gl.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geom.gl.vbo);
  glBufferData(GL_ARRAY_BUFFER, attrib.num_vertices * sizeof(GLfloat) * 3,
               attrib.vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // IBO
  GLuint* ibuffer = csg_malloc(sizeof(GLuint) * attrib.num_faces);
  for (unsigned int i = 0; i < attrib.num_faces; i++) {
    ibuffer[i] = attrib.faces[i].v_idx;
  }
  glGenBuffers(1, &geom.gl.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.gl.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, attrib.num_faces * sizeof(GLuint),
               ibuffer, GL_STATIC_DRAW);
  csg_free(ibuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // free tiny obj memory
  tinyobj_attrib_free(&attrib);
  if (shape) tinyobj_shapes_free(shape, num_shapes);
  if (material) tinyobj_materials_free(material, num_materials);

  return geom;
}
