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
#include <stdio.h>

#include "internal.h"

csg_texture_t csg_texture_none(void) {
  csg_texture_t texture;

  texture.flags = 0;
  texture.gl.texo = 0;

  return texture;
}

// csg_texture_t csg_texture_create_from_bmp(const char* path) {
//  FILE* fp = fopen(path, "rb");
//  assert(fp != NULL);

//  unsigned char header_data[54];
//  size_t retval;
//  retval = fread(header_data, 54, 1, fp);
//  if (retval != 1) {
//    return csg_texture_none();
//  }

//  int width = *(int*)&header_data[0x12];
//  int height = *(int*)&header_data[0x16];
//  int datastart = *(int*)&header_data[0x0a];

//  printf("BMP file %s: width %d, height %d, data starts at off %d\n", path,
//         width, height, datastart);

//  int datasize = width * height * 3 /* RGB */;
//  fseek(fp, datastart, SEEK_SET);

//  struct {
//    unsigned char blue;
//    unsigned char green;
//    unsigned char red;
//  }* bgr_data = csg_malloc(datasize);

//  retval = fread(bgr_data, datasize, 1, fp);
//  if (retval != 1) {
//    csg_free(bgr_data);
//    fclose(fp);
//    return csg_texture_none();
//  }
//  fclose(fp);
//  printf("BMP read %d bytes of BGR data\n", datasize);

//  return csg_texture_none();
//}
