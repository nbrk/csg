/*
 * Copyright 2020 Nikolay Burkov <nbrk@linklevel.net>
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
#include <GLFW/glfw3.h>
#include <csg/csg.h>
#include <stdio.h>

#include "nuklear.h"

static csg_node_t* make_cube(csg_node_t* parent, float x, float y, float z) {
  static csg_geometry_t* cube_geom = NULL;
  if (cube_geom == NULL)
    cube_geom = csg_geometry_create_cube();

  srand(arc4random());

  // XXX what's wrong with this code???:
  csg_material_t* mtrl = csg_material_create();
  csg_material_set_color(mtrl, CSG_MATERIAL_COLOR_DIFFUSE,
                         1.0f / (arc4random() % 10), 1.0f / (arc4random() % 10),
                         1.0f / (arc4random() % 10), 1.0f);

  //  csg_node_t* node = csg_node_create(parent, NULL);
  //  csg_transform_t* trans = csg_transform_create(x, y, z);
  //  csg_node_set_transform(node, trans);

  //  csg_drawable_t* draw = csg_drawable_create(cube_geom, mtrl);
  //  csg_node_set_drawable(node, draw);

  csg_node_t* node = csg_node_create(parent, "Jill");
  csg_drawable_t* draw = csg_drawable_create(cube_geom, mtrl);
  csg_node_set_drawable(node, draw);
  csg_transform_t* trans = csg_transform_create_translated(x, y, z);
  csg_node_set_transform(node, trans);

  return node;
}

static void test4(GLFWwindow* window) {
  csg_viewport_t* view = csg_viewport_create(0, 0, 1024, 768);

  csg_node_t* root = csg_node_create(NULL, NULL);

  for (int i = 0; i < 4; i++) {
    csg_node_t* cube = csg_node_create(root, NULL);
    csg_node_set_drawable(cube, csg_drawable_create(csg_geometry_create_cube(),
                                                    csg_material_create()));
    csg_transform_translate(csg_node_get_transform(cube), rand() % 20 + 3.f,
                            0.f, 0.f);
  }

  while (glfwWindowShouldClose(window) != GLFW_TRUE) {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      printf("A, ");
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x - 0.1f, y, z,
                              false);
      float cx, cy, cz;
      csg_camera_get_center(csg_viewport_get_camera(view), &cx, &cy, &cz);
      printf("Camera pos: (%.3f, %.3f, %.3f), center: (%.3f, %.3f, %.3f)\n", x,
             y, z, cx, cy, cz);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      printf("D, ");
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x + 0.1f, y, z,
                              false);
      float cx, cy, cz;
      csg_camera_get_center(csg_viewport_get_camera(view), &cx, &cy, &cz);
      printf("Camera pos: (%.3f, %.3f, %.3f), center: (%.3f, %.3f, %.3f)\n", x,
             y, z, cx, cy, cz);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
      printf("Z, ");
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x - 0.1f, y, z,
                              true);
      float cx, cy, cz;
      csg_camera_get_center(csg_viewport_get_camera(view), &cx, &cy, &cz);
      printf("Camera pos: (%.3f, %.3f, %.3f), center: (%.3f, %.3f, %.3f)\n", x,
             y, z, cx, cy, cz);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
      printf("C, ");
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x + 0.1f, y, z,
                              true);
      float cx, cy, cz;
      csg_camera_get_center(csg_viewport_get_camera(view), &cx, &cy, &cz);
      printf("Camera pos: (%.3f, %.3f, %.3f), center: (%.3f, %.3f, %.3f)\n", x,
             y, z, cx, cy, cz);
    }

    csg_viewport_render(view, root);

    glfwSwapBuffers(window);
  }
}

int main(int argc, char** argv) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow* window1 = glfwCreateWindow(1024, 768, "Window 1", NULL, NULL);
  glfwMakeContextCurrent(window1);
  glfwSetInputMode(window1, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSwapInterval(1);

  csg_set_malloc_debug(true);
  csg_init();

   test4(window1);

  glfwDestroyWindow(window1);
  glfwTerminate();

  return 0;
}
