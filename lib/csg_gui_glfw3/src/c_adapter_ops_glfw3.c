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
#include <csg/core.h>
#include <csg/gui.h>
#include <csg/gui_glfw3.h>
#include <limits.h>
#include <stdio.h>

static void poll_events(csg_gui_adapter_t* adapter) {
  glfwPollEvents();

  /*
   * Read important window events
   */
  GLFWwindow* window = adapter->backend_cookie;
  if (glfwWindowShouldClose(window) == GLFW_TRUE) {
    adapter->flags |= CSG_GUI_FLAG_WANT_CLOSE;
  }

  /*
   * Read keyboard state from the backend
   */
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

  /*
   * Read mouse state from the backend
   */
  for (size_t i = 0; i <= CSG_GUI_NUM_MOUSE_BUTTONS; i++)
    adapter->mouse[i] = glfwGetMouseButton(window, i);
  double x, y;
  // NOTE: cursor position has discrete steps in csg_gui
  glfwGetCursorPos(window, &x, &y);
  if (adapter->mouse_deltax == INT_MAX && adapter->mouse_deltay == INT_MAX) {
    // special case for when there is no 'old' coordinates
    adapter->mouse_x = (int)x;
    adapter->mouse_y = (int)y;
  }
  adapter->mouse_deltax = (int)x - adapter->mouse_x;
  adapter->mouse_deltay = (int)y - adapter->mouse_y;
  adapter->mouse_x = (int)x;
  adapter->mouse_y = (int)y;
}

static void glfw_adapter_begin_frame(csg_gui_adapter_t* adapter) {
  glfwMakeContextCurrent(adapter->backend_cookie);
  adapter->last_frame_duration = glfwGetTime();
  glfwSetTime(0.0);

  nk_glfw3_new_frame(adapter->ui_cookie);
}

static void glfw_adapter_end_frame(csg_gui_adapter_t* adapter) {
  nk_glfw3_render(adapter->ui_cookie, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER,
                  MAX_ELEMENT_BUFFER);

  // FIXME: restore OpenGL state to ours after the Nuklear
  glEnable(GL_DEPTH_TEST);

  glfwSwapBuffers(adapter->backend_cookie);

  // populate the adapter's inputs/flags
  poll_events(adapter);
}

static void glfw_adapter_destroy(csg_gui_adapter_t* adapter) {
  glfwDestroyWindow(adapter->backend_cookie);
}

static csg_gui_adapter_t glfw_adapter_create(int x_pos, int y_pos, int width,
                                             int height, int flags,
                                             void* shared_backend) {
  glfwInit();

  // FIXME: Proper OpenGL context version/compatibility.
  // OpenGL 3 core absolutely requires VAOs to encapsulate the data objects, but
  // it's not obvious how to properly share the data between windows then.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow* window = NULL;
  bool fullscreen = flags != 0 ? true : false;

  if (fullscreen) {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    width = mode->width;
    height = mode->height;
  }
  window = glfwCreateWindow(width, height, "GLFW",
                            fullscreen ? glfwGetPrimaryMonitor() : NULL,
                            shared_backend);
  glfwSetWindowPos(window, x_pos, y_pos);
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  csg_gui_adapter_t adapter;
  adapter.backend_cookie = window;
  adapter.flags = CSG_GUI_FLAG_FULLSCREEN | CSG_GUI_FLAG_RUNNING;
  adapter.mouse_x = 0;
  adapter.mouse_y = 0;
  adapter.mouse_deltax = INT_MAX;
  adapter.mouse_deltay = INT_MAX;
  adapter.screen_width = width;
  adapter.screen_height = height;
  adapter.last_frame_duration = 0.0;

  poll_events(&adapter);

  // NOTE: no need to set the backend ops (will be set by higher level
  // csg_adapter_create() )

  // create default UI/Nuklear context and set up the UI cookie
  struct nk_glfw* nk_glfw = csg_malloc(sizeof(*nk_glfw));
  nk_glfw3_init(nk_glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
  struct nk_font_atlas* atlas;
  nk_glfw3_font_stash_begin(nk_glfw, &atlas);
  nk_glfw3_font_stash_end(nk_glfw);
  adapter.ui_cookie = nk_glfw;
  adapter.nk = &nk_glfw->ctx;

  return adapter;
}

csg_gui_adapter_ops_t csg_gui_glfw3_adapter_ops(void) {
  csg_gui_adapter_ops_t glfw3_adapter_ops = {
      .create_func = glfw_adapter_create,
      .destroy_func = glfw_adapter_destroy,
      .begin_frame_func = glfw_adapter_begin_frame,
      .end_frame_func = glfw_adapter_end_frame};
  return glfw3_adapter_ops;
}
