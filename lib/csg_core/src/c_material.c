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
#include <stdio.h>
#include <string.h>

static size_t file_get_size(FILE* fp) {
  size_t curpos = ftell(fp);
  fseek(fp, 0, SEEK_END);
  size_t endpos = ftell(fp);
  fseek(fp, curpos, SEEK_SET);
  return endpos;
}

static char* file_get_contents(FILE* fp) {
  size_t sourcelen = file_get_size(fp);
  char* source = csg_malloc(sourcelen + 1);
  size_t nobjs = fread(source, sourcelen, 1, fp);

  if (nobjs != 1) {
    csg_free(source);
    return NULL;
  } else
    return source;
}

static const char* default_vertex_src =
    "#version 330\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "layout(location = 2) in vec2 a_texcoord;\n"
    "layout(location = 3) in vec2 a_color;\n"
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
static const char* default_fragment_src =
    "#version 330\n"
    "in vec2 o_texcoord;\n"
    "uniform vec4 u_diffuse_color;\n"
    "uniform bool u_use_textures;\n"
    "uniform sampler2D u_texture_unit;\n"
    "void main()\n"
    "{\n"
    "if (u_use_textures == true)\n"
    "gl_FragColor = texture(u_texture_unit, o_texcoord) * "
    "vec4(u_diffuse_color);\n"
    "else\n"
    "gl_FragColor = u_diffuse_color;\n"
    "}\n"
    "\n";

static int program_add_shader(GLuint program, GLenum shader_type,
                              const char* shader_src) {
  GLuint shader = glCreateShader(shader_type);
  const GLchar* source[1] = {shader_src};
  const GLint length[1] = {(GLint)strlen(shader_src)};

  glShaderSource(shader, 1, source, length);
  glCompileShader(shader);

  GLint param_val;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &param_val);
  if (param_val != GL_TRUE) {
    // fetch and print the info log
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &param_val);
    GLchar* buffer = csg_malloc((size_t)param_val);
    glGetShaderInfoLog(shader, param_val, NULL, buffer);
    glDeleteShader(shader);
    printf("Shader type %d compile error: %s\n", shader_type, buffer);
    csg_free(buffer);
    return -1;
  }
  glAttachShader(program, shader);
  glDeleteShader(shader);  // marks the shader for deletion upon deletion of the
                           // program object
  return 0;
}

static GLuint program_create(const char* vertex_src, const char* fragment_src) {
  GLuint program;
  program = glCreateProgram();

  if (program_add_shader(program, GL_VERTEX_SHADER, vertex_src) != 0) {
    glDeleteProgram(program);
    return 0;
  }
  if (program_add_shader(program, GL_FRAGMENT_SHADER, fragment_src) != 0) {
    glDeleteProgram(program);
    return 0;
  }
  GLint param_val;
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &param_val);
  if (param_val != GL_TRUE) {
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &param_val);
    GLchar* buffer = csg_malloc((size_t)param_val);
    glGetProgramInfoLog(program, param_val, NULL, buffer);
    glDeleteProgram(program);
    printf("Program link error: %s\n", buffer);
    csg_free(buffer);
    return 0;
  }
  glValidateProgram(program);
  glGetProgramiv(program, GL_VALIDATE_STATUS, &param_val);
  if (param_val != GL_TRUE) {
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &param_val);
    GLchar* buffer = csg_malloc((size_t)param_val);
    glGetProgramInfoLog(program, param_val, NULL, buffer);
    glDeleteProgram(program);
    printf("Program validation error: %s\n", buffer);
    csg_free(buffer);
    return 0;
  }

  return program;
}

csg_material_t csg_material_create(void) {
  GLuint program = program_create(default_vertex_src, default_fragment_src);
  assert(program != 0);

  csg_material_t mtrl;

  mtrl.flags = CSG_FLAG_ENABLED;
  mtrl.diffuse_color = (csg_vec4_t){1.0f, 1.0f, 1.0f, 1.0f};
  mtrl.texture = csg_texture_none();
  mtrl.gl_program = program;

  return mtrl;
}

csg_material_t csg_material_none(void) {
  csg_material_t mtrl;

  mtrl.flags = 0;
  mtrl.diffuse_color = (csg_vec4_t){0.0f, 0.0f, 0.0f, 0.0f};
  mtrl.texture = csg_texture_none();
  mtrl.gl_program = 0;

  return mtrl;
}
