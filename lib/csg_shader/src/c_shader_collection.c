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
#include <csg/shader.h>
#include <string.h>

static const char* vertex_0 =
    "#version 330\n"
    "layout(location = 0) in vec3 a_position;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "void main()\n"
    "{\n"
    "gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);\n"
    "}\n"
    "\n";
static const char* fragment_0 =
    "#version 330\n"
    "uniform vec4 u_diffuse_color;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = u_diffuse_color;\n"
    "}\n"
    "\n";

// normals, textured (no other colors mixed)
static const char* vertex_1 =
    "#version 330\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "layout(location = 2) in vec2 a_texcoord;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "out vec2 o_texcoord;\n"
    "void main()\n"
    "{\n"
    "gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);\n"
    "o_texcoord = a_texcoord;\n"
    "}\n"
    "\n";
static const char* fragment_1 =
    "#version 330\n"
    "in vec2 o_texcoord;\n"
    "uniform sampler2D u_texture_unit;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = texture(u_texture_unit, o_texcoord);\n"
    "}\n"
    "\n";

// normals, textured, the texture blended with diffuse color
static const char* vertex_2 =
    "#version 330\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "layout(location = 2) in vec2 a_texcoord;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "out vec2 o_texcoord;\n"
    "void main()\n"
    "{\n"
    "gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);\n"
    "o_texcoord = a_texcoord;\n"
    "}\n"
    "\n";
static const char* fragment_2 =
    "#version 330\n"
    "in vec2 o_texcoord;\n"
    "uniform vec4 u_diffuse_color;\n"
    "uniform sampler2D u_texture_unit;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = texture(u_texture_unit, o_texcoord) * "
    "vec4(u_diffuse_color);\n"
    "}\n"
    "\n";
// normals, cube-map textured
static const char* vertex_3 =
    "#version 330\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "out vec3 o_texcoord;\n"
    "void main()\n"
    "{\n"
    "gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);\n"
    "o_texcoord = a_position;\n"
    "}\n"
    "\n";
static const char* fragment_3 =
    "#version 330\n"
    "in vec3 o_texcoord;\n"
    "uniform samplerCube u_texture_unit;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = texture(u_texture_unit, o_texcoord);\n"
    "}\n"
    "\n";

const char* csg_shader_collection_get(const char* name, GLenum type) {
  // TODO: proper switching

  // simple: uses only position attrib and diffuse_color uniform
  if (strncmp(name, "simple", strlen(name)) == 0) {
    if (type == GL_VERTEX_SHADER) return vertex_0;
    if (type == GL_FRAGMENT_SHADER) return fragment_0;
  }
  if (strncmp(name, "textured", strlen(name)) == 0) {
    if (type == GL_VERTEX_SHADER) return vertex_1;
    if (type == GL_FRAGMENT_SHADER) return fragment_1;
  }
  if (strncmp(name, "textured-color", strlen(name)) == 0) {
    if (type == GL_VERTEX_SHADER) return vertex_2;
    if (type == GL_FRAGMENT_SHADER) return fragment_2;
  }
  if (strncmp(name, "cubemap", strlen(name)) == 0) {
    if (type == GL_VERTEX_SHADER) return vertex_3;
    if (type == GL_FRAGMENT_SHADER) return fragment_3;
  }

  return NULL;
}
