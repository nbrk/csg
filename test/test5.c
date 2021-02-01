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
  //  csg_malloc_set_debug(true);

  csg_gui_adapter_t adapter = csg_gui_adapter_create(
      csg_gui_glfw3_adapter_ops(), 0, 0, 1024, 768, 0, NULL);

  csg_gui_adapter_t adapter2 =
      csg_gui_adapter_create(csg_gui_glfw3_adapter_ops(), 1100, 100, 640, 480,
                             0, adapter.backend_cookie);

  // nodes
  csg_node_t* root = csg_node_create(NULL, NULL);
  //  root->geometry = csg_geometry_create_cube();
  //  root->geometry->material = csg_material_create();
  //  root->geometry->material.diffuse_color = (csg_vec4_t){1.0f, 0.7f,
  //  0.4f, 1.0f};
  for (size_t i = 0; i <= 16; i += 2) {
    csg_node_t* node = csg_node_create(root, NULL);
    node->transform.translation.x = i - 8;
    node->geometry = csg_geometry_create_cube();
    node->geometry.material = csg_material_create();
    node->geometry.material.diffuse_color = (csg_vec4_t){
        1.0f / (rand() % 10), 1.f / (rand() % 10), 1.f / (rand() % 10), 1.0f};
  }

  csg_vec4_t clear_color = (csg_vec4_t){0.20f, 0.0f, 0.0f, 1.0f};
  csg_camera_t camera = csg_camera_default();

  while ((adapter.flags & CSG_GUI_FLAG_WANT_CLOSE) == 0) {
    //    csg_gui_adapter_update(&adapter);
    //    csg_gui_adapter_update(&adapter2);

    if (adapter.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS)
      adapter.flags |= CSG_GUI_FLAG_WANT_CLOSE;
    if (adapter2.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS)
      adapter.flags |= CSG_GUI_FLAG_WANT_CLOSE;

    if (adapter.keyboard[CSG_GUI_KEY_W] == CSG_GUI_PRESS) {
      camera.position.z--;
    }
    if (adapter.keyboard[CSG_GUI_KEY_S] == CSG_GUI_PRESS) {
      camera.position.z++;
    }
    if (adapter.keyboard[CSG_GUI_KEY_A] == CSG_GUI_PRESS) {
      camera.position.x--;
    }
    if (adapter.keyboard[CSG_GUI_KEY_D] == CSG_GUI_PRESS) {
      camera.position.x++;
    }

    if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_RIGHT]) {
      if (adapter.mouse_deltax != 0)
        camera.horizontal_angle += 0.1f * adapter.mouse_deltax;
      if (adapter.mouse_deltay != 0)
        camera.vertical_angle -= 0.1f * adapter.mouse_deltay;
    }
    if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_LEFT]) {
      if (adapter.mouse_deltax != 0)
        root->transform.rotation.y += 0.01f * adapter.mouse_deltax;
      if (adapter.mouse_deltay != 0)
        root->transform.rotation.x += 0.01f * adapter.mouse_deltay;
    }

    csg_gui_adapter_begin_frame(&adapter);
    {
      // scene
      csg_render(root, camera, clear_color);

      // ui
      if (nk_begin(adapter.nk, "Render control", nk_rect(100, 100, 320, 240),
                   NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE |
                       NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                       NK_WINDOW_MINIMIZABLE)) {
        nk_layout_row_dynamic(adapter.nk, 25, 1);
        nk_property_float(adapter.nk, "Clear color Red", 0.0f, &clear_color.x,
                          1.0f, 0.01f, 0.001f);
        nk_property_float(adapter.nk, "Clear color Green", 0.0f, &clear_color.y,
                          1.0f, 0.01f, 0.001f);
        nk_property_float(adapter.nk, "Clear color Blue", 0.0f, &clear_color.z,
                          1.0f, 0.01f, 0.001f);
        //        nk_property_float(adapter.nk, "Material Red", 0.0f,
        //                          &root->geometry->material.diffuse_color.x, 1.0f,
        //                          0.01f, 0.001f);
        //        nk_property_float(adapter.nk, "Material Green", 0.0f,
        //                          &root->geometry->material.diffuse_color.y, 1.0f,
        //                          0.01f, 0.001f);
        //        nk_property_float(adapter.nk, "Material Blue", 0.0f,
        //                          &root->geometry->material.diffuse_color.z, 1.0f,
        //                          0.01f, 0.001f);
      }
      nk_end(adapter.nk);
    }
    csg_gui_adapter_end_frame(&adapter);

    csg_gui_adapter_begin_frame(&adapter2);
    {
      // scene
      csg_render(root, csg_camera_default_ortho(),
                 (csg_vec4_t){0.2f, 0.2f, 0.3f, 1.0f});
      // ui
      if (nk_begin(adapter2.nk, "Model transform", nk_rect(0, 0, 320, 240),
                   NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE |
                       NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                       NK_WINDOW_MINIMIZABLE)) {
        nk_layout_row_dynamic(adapter2.nk, 25, 1);
        nk_property_float(adapter2.nk, "Rotation along X", -2.0f * M_PI,
                          &root->transform.rotation.x, 2.0f * M_PI, 0.10f,
                          0.01f);
        nk_property_float(adapter2.nk, "Rotation along Y", -2.0f * M_PI,
                          &root->transform.rotation.y, 2.0f * M_PI, 0.10f,
                          0.01f);
        nk_property_float(adapter2.nk, "Rotation along Z", -2.0f * M_PI,
                          &root->transform.rotation.z, 2.0f * M_PI, 0.10f,
                          0.01f);
      }
      nk_end(adapter2.nk);
    }
    csg_gui_adapter_end_frame(&adapter2);
  }

  csg_malloc_print_stat();
  return 0;
}
