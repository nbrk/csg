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
#include <csg/image.h>
#include <stdio.h>

#include "internal_stb_image.h"

csg_texture_t csg_texture_create_2d_from_image(const char* path,
                                               bool vert_flip) {
  int width, height, nchans;
  unsigned char* data = NULL;

  stbi_set_flip_vertically_on_load(vert_flip);
  data = stbi_load(path, &width, &height, &nchans, 4);
  assert(data != NULL);

  printf("Loaded image: %s, size %d x %d (%f MB)\n", path, width, height,
         (float)width * height * nchans / 1024 / 1024);

  csg_texture_t texture;
  texture.flags = CSG_FLAG_ENABLED;
  texture.gl.type = GL_TEXTURE_2D;

  glGenTextures(1, &texture.gl.texo);
  //  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.gl.texo);
  // feed it to the hardware
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered
  // LINEARLY too
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);
  return texture;
}

static void unpack_cubemap_part(const char* path, GLenum target,
                                bool vert_flip) {
  int width, height, nchans;
  unsigned char* data = NULL;

  stbi_set_flip_vertically_on_load(vert_flip);

  data = stbi_load(path, &width, &height, &nchans, 4);
  assert(data != NULL);
  printf("Loaded image: %s, size %d x %d (%f MB), cubemap part %d\n", path,
         width, height, (float)width * height * nchans / 1024 / 1024, target);
  glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               data);
  stbi_image_free(data);
}

csg_texture_t csg_texture_create_cubemap_from_image(
    const char* right, const char* left, const char* top, const char* bottom,
    const char* front, const char* back, bool vert_flip) {
  csg_texture_t texture;

  texture.flags = CSG_FLAG_ENABLED;
  texture.gl.type = GL_TEXTURE_CUBE_MAP;

  glGenTextures(1, &texture.gl.texo);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture.gl.texo);

  // load and unpack the images
  unpack_cubemap_part(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X, vert_flip);
  unpack_cubemap_part(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, vert_flip);
  unpack_cubemap_part(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, vert_flip);
  unpack_cubemap_part(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, vert_flip);
  unpack_cubemap_part(front, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, vert_flip);
  unpack_cubemap_part(back, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, vert_flip);

  // generate mipmaps
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  // filters
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return texture;
}
