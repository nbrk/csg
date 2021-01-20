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

static void test3(GLFWwindow* window) {
  csg_viewport_t* view = csg_viewport_create(0, 0, 1024, 768);
  csg_viewport_set_clear_color(view, 0.0f, 0.0f, 0.1f, 1.0f);
  csg_camera_set_position(csg_viewport_get_camera(view), 10.0f, 10.f, 50.f,
                          false);

  csg_node_t* root = csg_node_create(NULL, NULL);
#define NCUBES 64
  csg_node_t* cubes[NCUBES];
  csg_animation_t* anims[NCUBES];
  for (size_t i = 0; i < NCUBES; i++) {
    static float row = 0;
    if (i % 8 == 0)
      row++;
    cubes[i] = make_cube(root, (i * 2) % (8 * 2), 2 * row, 0.f);
    float x, y, z;
    csg_transform_get_position(csg_node_get_transform(cubes[i]), &x, &y, &z);
    anims[i] = csg_animation_create(CSG_ANIMATION_MODE_OSCILLATE);
    csg_animation_add_point(anims[i], x, y, z, 1.f);
    csg_animation_add_point(anims[i], x, y, z + (arc4random() % 50), 1.f);

    csg_animation_add_point(anims[i], x, y, z, 1.f);
    csg_animation_add_point(anims[i], x, y, z - (arc4random() % 50), 1.f);
    csg_transform_set_translation_animation(csg_node_get_transform(cubes[i]),
                                            anims[i]);
  }

  while (glfwWindowShouldClose(window) != GLFW_TRUE) {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      printf("Left\n");
      //      csg_transform_translate(camtrans, 0.1f, 0.0f, 0.0f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x - 0.1f, y, z,
                              false);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      printf("Right\n");
      //      csg_transform_translate(camtrans, -0.1f, 0.0f, 0.0f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x + 0.1f, y, z,
                              false);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      printf("Up\n");
      //      csg_transform_translate(camtrans, 0.0f, -0.1f, 0.0f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x, y + 0.1f, z,
                              false);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      printf("Down\n");
      //      csg_transform_translate(camtrans, 0.0f, 0.1f, 0.0f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x, y - 0.1f, z,
                              false);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
      printf("PgUp\n");
      //      csg_transform_translate(camtrans, 0.0f, 0.0f, 0.1f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x, y, z - 0.1f,
                              false);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
      printf("PgDn\n");
      //      csg_transform_translate(camtrans, 0.0f, 0.0f, -0.1f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x, y, z + 0.1f,
                              false);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
      printf("Z\n");
      //      csg_transform_rotate(camtrans, -0.01f, 0.0f, 1.0f, 0.0f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x - 0.1f, y,
                              z - 0.1f, true);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
      printf("C\n");
      //      csg_transform_rotate(camtrans, 0.01f, 0.0f, 1.0f, 0.0f);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      csg_camera_set_position(csg_viewport_get_camera(view), x + 0.1f, y,
                              z - 0.1f, true);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
      printf("Q\n");
      //      csg_transform_rotate(camtrans, 0.1f, 0.0f, 1.0f, 0.0f);
      for (size_t i = 0; i < NCUBES; i++) {
        csg_transform_rotate(csg_node_get_transform(cubes[i]), 0.1f, 0.0f, 1.f,
                             0.f);
      }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
      printf("E\n");
      //      csg_transform_rotate(camtrans, -0.1f, 0.0f, 1.0f, 0.0f);
      for (size_t i = 0; i < NCUBES; i++) {
        csg_transform_rotate(csg_node_get_transform(cubes[i]), -0.1f, 0.0f, 1.f,
                             0.f);
      }
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
      printf("MINUS\n");
      //      csg_transform_rotate(camtrans, 0.1f, 0.0f, 1.0f, 0.0f);
      for (size_t i = 0; i < NCUBES; i++) {
        csg_transform_scale(csg_node_get_transform(cubes[i]), 0.9f, 1.0f, 1.f);
      }
    }
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
      printf("EQUAL\n");
      //      csg_transform_rotate(camtrans, -0.1f, 0.0f, 1.0f, 0.0f);
      for (size_t i = 0; i < NCUBES; i++) {
        csg_transform_scale(csg_node_get_transform(cubes[i]), 1.1f, 1.0f, 1.f);
      }
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
      for (size_t i = 0; i < NCUBES; i++) {
        csg_transform_update_translation_animation(
            csg_node_get_transform(cubes[i]), 0.005f);
      }
    }

    //     process all animations
    for (size_t i = 0; i < NCUBES; i++) {
      csg_transform_update_translation_animation(
          csg_node_get_transform(cubes[i]), 0.005f);
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

  test3(window1);

  glfwDestroyWindow(window1);
  glfwTerminate();

  return 0;
}
