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
#include <GLFW/glfw3.h>
#include <csg/gui.h>
#include <stdio.h>

static void glfw_adapter_update(csg_gui_adapter_t* adapter) {
  glfwPollEvents();

  GLFWwindow* window = adapter->backend_cookie;
  if (glfwWindowShouldClose(window) == GLFW_TRUE) {
    adapter->flags |= CSG_GUI_FLAG_WANT_CLOSE;
  }

  // read keyboard state from the backend
  // NOTE: assuming full compatibility between csg_gui and glfw key/mouse button
  for (size_t i = 0; i <= CSG_GUI_NUM_KEYS; i++) {
    int glfw_state = glfwGetKey(window, i);
    char state = CSG_GUI_RELEASE;  // XXX
    if (glfw_state == GLFW_PRESS) {
      // check previous state to sense the transition from release to press
      if (adapter->keyboard[i] == CSG_GUI_RELEASE)
        state = CSG_GUI_FROM_RELEASE_TO_PRESS;
      else
        state = CSG_GUI_PRESS;
    }
    if (glfw_state == GLFW_RELEASE) {
      // check previous state to sense the transition from press to release
      if (adapter->keyboard[i] == CSG_GUI_PRESS)
        state = CSG_GUI_FROM_PRESS_TO_RELEASE;
      else
        state = CSG_GUI_RELEASE;
    }
    if (glfw_state == GLFW_REPEAT) state = CSG_GUI_REPEAT;

    adapter->keyboard[i] = state;
  }

  // read mouse state from the backend
  for (size_t i = 0; i <= CSG_GUI_NUM_MOUSE_BUTTONS; i++)
    adapter->mouse[i] = glfwGetMouseButton(window, i);
  double x, y;
  // NOTE: cursor position has discrete steps in csg_gui
  glfwGetCursorPos(window, &x, &y);
  adapter->mouse_x = (int)x;
  adapter->mouse_y = (int)y;
}
static void glfw_adapter_begin_frame(csg_gui_adapter_t* adapter) {
  glfwMakeContextCurrent(adapter->backend_cookie);
  // no ther preps to draw...
}

static void glfw_adapter_end_frame(csg_gui_adapter_t* adapter) {
  glfwSwapBuffers(adapter->backend_cookie);
}

static void glfw_adapter_destroy(csg_gui_adapter_t* adapter) {
  glfwDestroyWindow(adapter->backend_cookie);
}

static csg_gui_adapter_t glfw_adapter_create(int width, int height, int flags) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow* window1 = NULL;
  bool fullscreen = flags != 0 ? true : false;

  if (fullscreen) {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    width = mode->width;
    height = mode->height;
  }
  window1 = glfwCreateWindow(width, height, "GLFW",
                             fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
  glfwMakeContextCurrent(window1);
  glfwSetInputMode(window1, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  csg_gui_adapter_t adapter;
  adapter.backend_cookie = window1;
  adapter.flags = CSG_GUI_FLAG_FULLSCREEN | CSG_GUI_FLAG_RUNNING;
  adapter.mouse_x = 0;
  adapter.mouse_y = 0;
  adapter.screen_width = width;
  adapter.screen_height = height;
  // no need to set the backend ops (will be set by csg_adapter_create() )

  return adapter;
}

csg_gui_adapter_ops_t glfw3_adapter_ops = {
    .create_func = glfw_adapter_create,
    .update_func = glfw_adapter_update,
    .destroy_func = glfw_adapter_destroy,
    .begin_frame_func = glfw_adapter_begin_frame,
    .end_frame_func = glfw_adapter_end_frame};
