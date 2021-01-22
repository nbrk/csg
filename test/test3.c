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
#include <csg/gui.h>
#include <csg/gui_glfw3.h>
#include <stdio.h>

// extern csg_gui_adapter_ops_t glfw3_adapter_ops;

int main(int argc, char** argv) {
  csg_gui_adapter_t adapter1 = csg_gui_adapter_create(
      csg_gui_glfw3_adapter_ops(), 0, 0, 320, 240, 0, NULL);
  csg_gui_adapter_t adapter2 = csg_gui_adapter_create(
      csg_gui_glfw3_adapter_ops(), 0, 0, 1024, 768, 0, NULL);

  csg_gui_adapter_t saved_adapter1 = adapter1;
  while (adapter1.flags & CSG_GUI_FLAG_RUNNING &&
         adapter2.flags & CSG_GUI_FLAG_RUNNING) {
    csg_gui_adapter_update(&adapter1);
    csg_gui_adapter_update(&adapter2);

    // just close on request
    if (adapter1.flags & CSG_GUI_FLAG_WANT_CLOSE) {
      printf("User wants to close adapter 1\n");
      adapter1.flags &= ~CSG_GUI_FLAG_RUNNING;
    }
    if (adapter2.flags & CSG_GUI_FLAG_WANT_CLOSE) {
      printf("User wants to close adapter 2\n");
      adapter2.flags &= ~CSG_GUI_FLAG_RUNNING;
    }

    if (adapter1.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS) {
      printf("User hits ESC on adapter 1\n");
      adapter1.flags &= ~CSG_GUI_FLAG_RUNNING;
    }
    if (adapter2.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS) {
      printf("User hits ESC on adapter 2\n");
      adapter2.flags &= ~CSG_GUI_FLAG_RUNNING;
    }

    if (adapter1.keyboard[CSG_GUI_KEY_R] == CSG_GUI_RELEASE) {
      // check old state to determine if there was s switch press->release
      if (saved_adapter1.keyboard[CSG_GUI_KEY_R] == CSG_GUI_PRESS) {
        printf("User releases R key after press on adapter 1\n");
        saved_adapter1 = adapter1;
      }
    }
    if (adapter1.keyboard[CSG_GUI_KEY_R] == CSG_GUI_PRESS)
      saved_adapter1 = adapter1;

    csg_gui_adapter_begin_frame(&adapter1);
    csg_gui_adapter_end_frame(&adapter1);

    csg_gui_adapter_begin_frame(&adapter2);
    csg_gui_adapter_end_frame(&adapter2);
  }

  return 0;
}
