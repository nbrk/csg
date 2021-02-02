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
#include <csg/gui.h>
#include <csg/gui_glfw3.h>
#include <csg/image.h>
#include <csg/shader.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

extern void camera_control(csg_camera_t* camera, csg_gui_adapter_t* adapter);

static void logic(csg_node_t* root, csg_camera_t* camera,
                  csg_gui_adapter_t* adapter) {
  camera_control(camera, adapter);
}

int main(int argc, char** argv) {
  csg_gui_adapter_t adapter = csg_gui_adapter_create(
      csg_gui_glfw3_adapter_ops(), 0, 0, 1024, 768, 0, NULL);

  csg_geometry_t cube_geom = csg_geometry_create_cube();
  csg_geometry_t sphere_geom = csg_geometry_create_sphere(50);
  csg_geometry_t quad_geom = csg_geometry_create_quad();

  unsigned int simple_prog = csg_shader_program_assemble_mem(
      csg_shader_collection_get("simple", GL_VERTEX_SHADER),
      csg_shader_collection_get("simple", GL_FRAGMENT_SHADER));
  unsigned int tex_prog = csg_shader_program_assemble_mem(
      csg_shader_collection_get("textured", GL_VERTEX_SHADER),
      csg_shader_collection_get("textured", GL_FRAGMENT_SHADER));
  unsigned int tex_color_prog = csg_shader_program_assemble_mem(
      csg_shader_collection_get("textured-color", GL_VERTEX_SHADER),
      csg_shader_collection_get("textured-color", GL_FRAGMENT_SHADER));
  unsigned int cube_tex_prog = csg_shader_program_assemble_mem(
      csg_shader_collection_get("cubemap", GL_VERTEX_SHADER),
      csg_shader_collection_get("cubemap", GL_FRAGMENT_SHADER));
  csg_texture_t grass_tex = csg_texture_create_2d_from_image(
      "/usr/home/nbrk/Downloads/junk/meshes/grass_grass_0130_01.jpg", false);
  csg_texture_t cube_tex = csg_texture_create_cubemap_from_image(
      "/usr/home/nbrk/Downloads/junk/meshes/skybox/right.jpg",
      "/usr/home/nbrk/Downloads/junk/meshes/skybox/left.jpg",
      "/usr/home/nbrk/Downloads/junk/meshes/skybox/top.jpg",
      "/usr/home/nbrk/Downloads/junk/meshes/skybox/bottom.jpg",
      "/usr/home/nbrk/Downloads/junk/meshes/skybox/front.jpg",
      "/usr/home/nbrk/Downloads/junk/meshes/skybox/back.jpg", false);

  csg_camera_t camera = csg_camera_default();
  camera.position.z = 2.f;

  // nodes
  csg_node_t* root = csg_node_create(NULL, NULL);

  csg_node_t* node1 = csg_node_create(root, NULL);
  node1->transform.scale = (csg_vec3_t){100.f, 100.f, 100.f};
  node1->geometry = cube_geom;
  node1->shader = csg_shader_create(cube_tex_prog);
  node1->shader.u_diffuse_color = (csg_vec4_t){1.0f, 0.0f, 0.0f, 1.0f};

  csg_node_t* node2 = csg_node_create(root, NULL);
  node2->geometry = quad_geom;
  node1->texture = grass_tex;
  node2->shader = csg_shader_create(simple_prog);
  node2->shader.u_diffuse_color = (csg_vec4_t){0.0f, 0.0f, 1.0f, 1.0f};

  while ((adapter.flags & CSG_GUI_FLAG_WANT_CLOSE) == 0) {
    csg_gui_adapter_begin_frame(&adapter);
    csg_render(root, camera, (csg_vec4_t){0.1f, 0.2f, 0.1f, 1.0f});
    logic(root, &camera, &adapter);

    if (adapter.keyboard[CSG_GUI_KEY_1] == CSG_GUI_PRESS) {
      node2->shader.program = simple_prog;
    }
    if (adapter.keyboard[CSG_GUI_KEY_2] == CSG_GUI_PRESS) {
      node2->shader.program = tex_prog;
    }
    if (adapter.keyboard[CSG_GUI_KEY_3] == CSG_GUI_PRESS) {
      node2->shader.program = tex_color_prog;
    }
    if (adapter.keyboard[CSG_GUI_KEY_9] == CSG_GUI_PRESS) {
      node1->shader.program = simple_prog;
    }
    if (adapter.keyboard[CSG_GUI_KEY_0] == CSG_GUI_PRESS) {
      node1->shader.program = cube_tex_prog;
    }

    csg_gui_adapter_end_frame(&adapter, 120);
  }

  return 0;
}
