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
#include <stdio.h>

int main(int argc, char** argv) {
  csg_gui_adapter_t adapter =
      csg_gui_adapter_create(csg_gui_glfw3_adapter_ops(), 1024, 768, 0);

  // nodes
  csg_node_t* root = csg_node_create(NULL, NULL);

  csg_vec4_t clear_color = (csg_vec4_t){0.20f, 0.0f, 0.0f, 1.0f};

  while ((adapter.flags & CSG_GUI_FLAG_WANT_CLOSE) == 0) {
    csg_gui_adapter_update(&adapter);

    if (adapter.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS)
      adapter.flags |= CSG_GUI_FLAG_WANT_CLOSE;

    csg_gui_adapter_begin_frame(&adapter);

    // scene
    csg_render(root, csg_camera_default(), clear_color);

    // ui
    if (nk_begin(adapter.nk, "Render control", nk_rect(100, 100, 240, 160),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
      nk_layout_row_dynamic(adapter.nk, 25, 1);
      nk_property_float(adapter.nk, "Clear color Red", 0.0f, &clear_color.x,
                        1.0f, 0.01f, 0.001f);
      nk_property_float(adapter.nk, "Clear color Green", 0.0f, &clear_color.y,
                        1.0f, 0.01f, 0.001f);
      nk_property_float(adapter.nk, "Clear color Blue", 0.0f, &clear_color.z,
                        1.0f, 0.01f, 0.001f);
    }
    nk_end(adapter.nk);

    csg_gui_adapter_end_frame(&adapter);
  }

  return 0;
}
