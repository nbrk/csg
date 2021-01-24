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
#include <math.h>
#include <stdio.h>

#define rand(x) arc4random(x)

int main(int argc, char** argv) {
  csg_malloc_set_debug(true);

  csg_gui_adapter_t adapter = csg_gui_adapter_create(
      csg_gui_glfw3_adapter_ops(), 0, 0, 1024, 768, 0, NULL);

  csg_geometry_t cube_geometry = csg_geometry_create_cube();
  csg_geometry_t tri_geometry = csg_geometry_create_triangle();
  csg_material_t material = csg_material_create();
  csg_camera_t camera = csg_camera_default();

  // nodes
  csg_node_t* root = csg_node_create(NULL, NULL);
  {
    csg_node_t* node1 = csg_node_create(root, NULL);
    node1->geometry = cube_geometry;
    node1->geometry.material = material;
    node1->geometry.gl.polygon_mode = GL_LINE;
    csg_node_t* node2 = csg_node_create(root, NULL);
    node2->transform.translation.x = -2.0f;
    node2->geometry = tri_geometry;
    node2->geometry.gl.polygon_mode = GL_POINT;
    node2->geometry.material = material;
    node2->geometry.material.diffuse_color.z = 0.33f;
  }
  //  root->geometry.material = csg_material_create();
  //  root->geometry.material.diffuse_color = (csg_vec4_t){1.0f, 0.0f,
  //  0.0f, 1.0f}; root->geometry->material.diffuse_color = (csg_vec4_t){1.0f,
  //  0.7f,

  while ((adapter.flags & CSG_GUI_FLAG_WANT_CLOSE) == 0) {
    csg_gui_adapter_update(&adapter);

    if (adapter.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS)
      adapter.flags |= CSG_GUI_FLAG_WANT_CLOSE;

    if (adapter.keyboard[CSG_GUI_KEY_W] == CSG_GUI_PRESS) {
      camera.position.z--;
      camera.target.z--;
    }
    if (adapter.keyboard[CSG_GUI_KEY_S] == CSG_GUI_PRESS) {
      camera.position.z++;
      camera.target.z++;
    }
    if (adapter.keyboard[CSG_GUI_KEY_A] == CSG_GUI_PRESS) {
      camera.position.x--;
      camera.target.x--;
    }
    if (adapter.keyboard[CSG_GUI_KEY_D] == CSG_GUI_PRESS) {
      camera.position.x++;
      camera.target.x++;
    }

    if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_RIGHT]) {
      if (adapter.mouse_deltax != 0)
        camera.target.x += 0.1f * adapter.mouse_deltax;
      if (adapter.mouse_deltay != 0)
        camera.target.y -= 0.1f * adapter.mouse_deltay;
    }
    if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_LEFT]) {
      if (adapter.mouse_deltax != 0)
        root->transform.rotation.y += 0.01f * adapter.mouse_deltax;
      if (adapter.mouse_deltay != 0)
        root->transform.rotation.x += 0.01f * adapter.mouse_deltay;
    }

    csg_gui_adapter_begin_frame(&adapter);
    { csg_render(root, camera, (csg_vec4_t){0.2f, 0.2f, 0.3f, 1.0f}); }
    csg_gui_adapter_end_frame(&adapter);
  }

  return 0;
}
