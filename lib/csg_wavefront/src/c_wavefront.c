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

#include <csg/wavefront.h>

#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>

#include "internal_tinyobj_loader_c.h"

// static void calc_totals(FILE* fp,
//                        size_t* v_total,
//                        size_t* vt_total,
//                        size_t* vn_total,
//                        size_t* f_total) {
//  *v_total = 0;
//  *vt_total = 0;
//  *vn_total = 0;
//  *f_total = 0;

//  char* line = csg_malloc(256);
//  ssize_t linelen = 0;
//  size_t linecap = sizeof(line);

//  long fpos = ftell(fp);
//  fseek(fp, 0, SEEK_SET);
//  while ((linelen = getline(&line, &linecap, fp)) > 0) {
//    if (line[0] == 'v' && line[1] == ' ')
//      *v_total = *v_total + 1;
//    if (line[0] == 'v' && line[1] == 't')
//      *vt_total = *vt_total + 1;
//    if (line[0] == 'v' && line[1] == 'n')
//      *vn_total = *vn_total + 1;
//    if (line[0] == 'f' && line[1] == ' ')
//      *f_total = *f_total + 1;
//  }
//  csg_free(line);
//  fseek(fp, fpos, SEEK_SET);
//}

// static void read_data(FILE* fp,
//                      float* v_buffer,
//                      float* vt_buffer,
//                      float* vn_buffer,
//                      float* fv_buffer,
//                      float* fvt_buffer,
//                      float* fvn_buffer) {
//  size_t v_done = 0;
//  size_t vt_done = 0;
//  size_t vn_done = 0;
//  size_t f_done = 0;

//  char* line = csg_malloc(256);
//  ssize_t linelen = 0;
//  size_t linecap = sizeof(line);

//  long fpos = ftell(fp);
//  fseek(fp, 0, SEEK_SET);
//  while ((linelen = getline(&line, &linecap, fp)) > 0) {
//    if (line[0] == 'v' && line[1] == ' ') {
//      float x, y, z;
//      sscanf(line, "v  %f %f %f", &x, &y, &z);
//      v_buffer[v_done * 3 + 0] = x;
//      v_buffer[v_done * 3 + 1] = y;
//      v_buffer[v_done * 3 + 2] = z;
//      v_done++;
//    }
//    if (line[0] == 'v' && line[1] == 't') {
//      float x, y, z;
//      sscanf(line, "vt %f %f %f", &x, &y, &z);
//      vt_buffer[vt_done * 3 + 0] = x;
//      vt_buffer[vt_done * 3 + 1] = y;
//      vt_buffer[vt_done * 3 + 2] = z;
//      vt_done++;
//    }
//    if (line[0] == 'v' && line[1] == 'n') {
//      float x, y, z;
//      sscanf(line, "vn %f %f %f", &x, &y, &z);
//      vn_buffer[vn_done * 3 + 0] = x;
//      vn_buffer[vn_done * 3 + 1] = y;
//      vn_buffer[vn_done * 3 + 2] = z;
//      vn_done++;
//    }
//    if (line[0] == 'f' && line[1] == ' ') {
//      int v1, v2, v3;
//      int vt1, vt2, vt3;
//      int vn1, vn2, vn3;
//      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &v2, &v3, &vt1, &vt2,
//             &vt3, &vn1, &vn2, &vn3);
//      fv_buffer[f_done * 3 + 0] = v1;
//      fv_buffer[f_done * 3 + 1] = v2;
//      fv_buffer[f_done * 3 + 2] = v3;
//      fvt_buffer[f_done * 3 + 0] = vt1;
//      fvt_buffer[f_done * 3 + 1] = vt2;
//      fvt_buffer[f_done * 3 + 2] = vt3;
//      fvn_buffer[f_done * 3 + 0] = vn1;
//      fvn_buffer[f_done * 3 + 1] = vn2;
//      fvn_buffer[f_done * 3 + 2] = vn3;
//      f_done++;
//    }
//  }
//  csg_free(line);
//  fseek(fp, fpos, SEEK_SET);
//}

// csg_geometry_t csg_geometry_create_from_wavefront(const char* path) {
//  FILE* fp = fopen(path, "r");
//  assert(fp != NULL);

//  size_t v_total = 0;
//  size_t vt_total = 0;
//  size_t vn_total = 0;
//  size_t f_total = 0;

//  calc_totals(fp, &v_total, &vt_total, &vn_total, &f_total);
//  printf("File totals: v=%zu, vt=%zu, vn=%zu, f=%zu\n", v_total, vt_total,
//         vn_total, f_total);

//  assert(v_total > 0);
//  float* v_buffer = csg_malloc(v_total * sizeof(GLfloat) * 3 /* x y z */);
//  float* vt_buffer = csg_malloc(vt_total * sizeof(GLfloat) * 3);
//  float* vn_buffer = csg_malloc(vn_total * sizeof(GLfloat) * 3);
//  float* fv_buffer = csg_malloc(f_total * sizeof(GLint) * 3);
//  float* fvt_buffer = csg_malloc(f_total * sizeof(GLint) * 3);
//  float* fvn_buffer = csg_malloc(f_total * sizeof(GLint) * 3);

//  read_data(fp, v_buffer, vt_buffer, vn_buffer, fv_buffer, fvt_buffer,
//            fvn_buffer);
//  fclose(fp);

//  csg_geometry_t geom = csg_geometry_none();
//  geom.flags = CSG_GEOMETRY_FLAG_ENABLED;
//  geom.gl.draw_mode = GL_TRIANGLES;
//  geom.gl.polygon_mode = GL_FILL;
//  geom.material = csg_material_none();
//  geom.num_to_draw = f_total;

//  glGenBuffers(1, &geom.gl.position_vbo);
//  glBindBuffer(GL_ARRAY_BUFFER, geom.gl.position_vbo);
//  glBufferData(GL_ARRAY_BUFFER, v_total * sizeof(GLfloat) * 3, v_buffer,
//               GL_STATIC_DRAW);

//  glBindBuffer(GL_ARRAY_BUFFER, 0);

//  return geom;
//}

static void loadFile(const char* filename,
                     const int is_mtl,
                     const char* obj_filename,
                     char** buffer,
                     size_t* len) {
  long string_size = 0, read_size = 0;
  FILE* handler = fopen(filename, "r");

  if (handler) {
    fseek(handler, 0, SEEK_END);
    string_size = ftell(handler);
    rewind(handler);
    *buffer = (char*)malloc(sizeof(char) * (string_size + 1));
    read_size = fread(*buffer, sizeof(char), (size_t)string_size, handler);
    (*buffer)[string_size] = '\0';
    if (string_size != read_size) {
      free(buffer);
    }
    fclose(handler);
  }

  *len = read_size;
}

// static void get_file_data(const char* filename, const int is_mtl, const char
// *obj_filename, char** data, size_t* len) {
//  if (!filename) {
//    fprintf(stderr, "null filename\n");
//    (*data) = NULL;
//    (*len) = 0;
//  }

//  const char* ext = strrchr(filename, '.');

//  size_t data_len = 0;

//  if (strcmp(ext, ".gz") == 0) {
//    assert(0); /* todo */

//  } else {

//    char *basedirname = NULL;

//    char tmp[1024];
//    tmp[0] = '\0';

//    /* For .mtl, extract base directory path from .obj filename and append
//    .mtl filename */ if (is_mtl && obj_filename) {
//      /* my_strdup is from tinyobjloader-c.h implementation(since strdup is
//      not a C standard library function */ basedirname = strndup(obj_filename,
//      strlen(obj_filename)); basedirname = dirname(basedirname);
//      printf("basedirname = %s\n", basedirname);
//    }

//    if (basedirname) {
//      strncpy(tmp, basedirname, strlen(basedirname));

//#if defined(_WIN32)
//      strncat(tmp, "/", 1023 - strlen(tmp));
//#else
//      strncat(tmp, "/", 1023 - strlen(tmp));
//#endif
//      strncat(tmp, filename, 1023 - strlen(tmp));
//      tmp[strlen(tmp)] = '\0';
//    } else {
//      strncpy(tmp, filename, strlen(filename));
//      tmp[strlen(tmp)] = '\0';
//    }

//    printf("tmp = %s\n", tmp);

//    if (basedirname) {
//      free(basedirname);
//    }

//    *data = mmap_file(&data_len, tmp);
//  }

//  (*len) = data_len;
//}

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

  tinyobj_attrib_free(&attrib);
  if (shape)
    tinyobj_shapes_free(shape, num_shapes);
  if (material)
    tinyobj_materials_free(material, num_materials);

  csg_geometry_t geom = csg_geometry_none();
  geom.flags = CSG_GEOMETRY_FLAG_ENABLED /* | CSG_GEOMETRY_FLAG_INDEXED_DRAW */;
  geom.gl.draw_mode = GL_TRIANGLES;
  geom.gl.polygon_mode = GL_FILL;
  geom.material = csg_material_none();
  geom.num_to_draw = attrib.num_vertices;

  glGenBuffers(1, &geom.gl.vertices_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geom.gl.vertices_vbo);
  glBufferData(GL_ARRAY_BUFFER, attrib.num_vertices * sizeof(GLfloat) * 3,
               attrib.vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  ///
  //  GLuint* ibuffer = malloc(sizeof(GLuint) * 3 * attrib.num_faces);
  //  for (unsigned int i = 0; i < attrib.num_faces; i++) {
  //    ibuffer[i] = attrib.faces[i].v_idx;
  //  }
  //  glGenBuffers(1, &geom.gl.vertices_ibo);
  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.gl.vertices_ibo);
  //  glBufferData(GL_ARRAY_BUFFER, attrib.num_faces * sizeof(GLuint) * 3,
  //  ibuffer,
  //               GL_STATIC_DRAW);
  //  free(ibuffer);
  //  geom.flags |= CSG_GEOMETRY_FLAG_INDEXED_DRAW;

  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  ///

  return geom;
}
