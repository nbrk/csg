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

  csg_camera_t camera = csg_camera_default();
  camera.position.z = 1.f;

  // nodes
  csg_node_t* root = csg_node_create(NULL, NULL);
  root->geometry = sphere_geom;
  root->geometry.material = csg_material_create();
  root->geometry.material.diffuse_color = (csg_vec4_t){0.f, 0.f, 0.9f, 1.0f};

  while ((adapter.flags & CSG_GUI_FLAG_WANT_CLOSE) == 0) {
    csg_gui_adapter_begin_frame(&adapter);
    csg_render(root, camera, (csg_vec4_t){0.1f, 0.2f, 0.1f, 1.0f});
    logic(root, &camera, &adapter);
    csg_gui_adapter_end_frame(&adapter, 120);
  }

  return 0;
}
