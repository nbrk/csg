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

unsigned int csg_shader_program_assemble_mem(const char* vertex_str,
                                             const char* fragment_str) {
  GLuint program = program_create(vertex_str, fragment_str);
  assert(program != 0);

  return program;
}

unsigned int csg_shader_program_assemble(const char* vertex_path,
                                         const char* fragment_path) {
  FILE* fp;

  fp = fopen(vertex_path, "r");
  assert(fp != NULL);
  char* vertex_str = file_get_contents(fp);
  fclose(fp);

  fp = fopen(fragment_path, "r");
  assert(fp != NULL);
  char* fragment_str = file_get_contents(fp);
  fclose(fp);

  GLuint program = csg_shader_program_assemble_mem(vertex_str, fragment_str);

  csg_free(vertex_str);
  csg_free(fragment_str);

  return program;
}

csg_shader_t csg_shader_create(unsigned int gl_program) {
  assert(gl_program != 0);
  csg_shader_t shader;

  shader.flags = CSG_FLAG_ENABLED;
  shader.program = gl_program;
  shader.u_diffuse_color = (csg_vec4_t){1.0f, 1.0f, 1.0f, 1.0f};
  bzero(&shader.u_model, sizeof(shader.u_model));
  bzero(&shader.u_view, sizeof(shader.u_view));
  bzero(&shader.u_projection, sizeof(shader.u_view));
  return shader;
}

csg_shader_t csg_shader_none(void) {
  csg_shader_t shader;

  shader.flags = 0;
  shader.program = 0;
  bzero(&shader, sizeof(csg_shader_t));

  return shader;
}

void csg_shader_set_all_uniforms(const csg_shader_t* shader) {
  // TODO: cache of the uniform locations
  /*
   * Query uniform locations
   */
  GLuint program = shader->program;
  GLuint u_model = glGetUniformLocation(program, "u_model");
  GLuint u_view = glGetUniformLocation(program, "u_view");
  GLuint u_projection = glGetUniformLocation(program, "u_projection");
  GLuint u_diffuse_color = glGetUniformLocation(program, "u_diffuse_color");
  //  GLint u_use_texture =
  //      glGetUniformLocation(program, "u_use_texture");  // FIXME

  /*
   * Set the uniforms
   */
  glUniform4fv(u_diffuse_color, 1, (GLfloat*)&shader->u_diffuse_color);
  glUniformMatrix4fv(u_model, 1, GL_FALSE, (GLfloat*)&shader->u_model);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, (GLfloat*)&shader->u_view);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE,
                     (GLfloat*)&shader->u_projection);
  //  if (node->geometry.material.texture.flags & CSG_TEXTURE_FLAG_1D)
  //    glUniform1i(u_use_texture, 1);
  //  if (node->geometry.material.texture.flags & CSG_TEXTURE_FLAG_2D)
  //    glUniform1i(u_use_texture, 2);
  //  if (node->geometry.material.texture.flags & CSG_TEXTURE_FLAG_CUBEMAP)
  //    glUniform1i(u_use_texture, 3);
}
