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

void print_node(csg_node_t* node, void* cookie) {
  size_t indent_level = 0;
  csg_node_t* child = node;
  while (csg_node_get_parent(child) != NULL) {
    indent_level++;
    child = csg_node_get_parent(child);
  }
  for (size_t i = 0; i < indent_level; i++) putchar(' ');

  printf("Node @%p, cookie: %d\n", node, csg_node_get_cookie(node));
}

static void test1(void) {
  csg_node_t* root = csg_node_create(NULL, 1);
  csg_node_t* n1 = csg_node_create(root, 10);
  csg_node_t* n2 = csg_node_create(root, 20);
  csg_node_t* n3 = csg_node_create(n1, 300);
  csg_node_t* n4 = csg_node_create(n3, 4000);
  csg_node_t* n5 = csg_node_create(n3, 5000);
  csg_node_t* n6 = csg_node_create(n5, 60000);

  printf("From root node:\n");
  csg_node_apply_depth_first(root, print_node, NULL);
  printf("A leaf node:\n");
  csg_node_apply_depth_first(n6, print_node, NULL);
}

static void test2(GLFWwindow* window) {
  csg_camera_t* cam =
      csg_camera_create(CSG_CAMERA_PROJECTION_PERSPECTIVE, 4 / 3);
  csg_transform_t* camtrans = csg_transform_create(0.0f, 0.0f, -20.0f);
  csg_camera_set_transform(cam, camtrans);

  csg_viewport_t* view = csg_viewport_create(0, 0, 1024, 768, cam);
  csg_viewport_set_clear_color(view, 0.0f, 0.1f, 0.0f, 1.0f);

  csg_material_t* blue = csg_material_create();
  csg_material_set_color(blue, CSG_MATERIAL_COLOR_DIFFUSE, 0.f, 0.f, 1.0f,
                         1.0f);
  csg_material_t* red = csg_material_create();
  csg_material_set_color(red, CSG_MATERIAL_COLOR_DIFFUSE, 1.0f, 0.f, 0.0f,
                         1.0f);
  csg_material_t* green = csg_material_create();
  csg_material_set_color(green, CSG_MATERIAL_COLOR_DIFFUSE, 0.0f, 1.0f, 0.0f,
                         1.0f);

  // reusable geometry
  csg_geometry_t* triangle = csg_geometry_create_triangle();

  // invisible root
  csg_node_t* root = csg_node_create(NULL, NULL);
  // first object..
  csg_node_t* node = csg_node_create(root, "Bob");
  csg_transform_t* trans = csg_transform_create(-1.0f, 0.0f, 0.0f);
  csg_drawable_t* draw = csg_drawable_create(triangle, blue);
  csg_node_set_drawable(node, draw);
  csg_node_set_transform(node, trans);
  // other object..
  csg_node_t* node1 = csg_node_create(root, "Jack");
  csg_transform_t* trans1 = csg_transform_create(0.0f, 0.0f, 0.0f);
  csg_drawable_t* draw1 = csg_drawable_create(triangle, red);
  csg_node_set_drawable(node1, draw1);
  csg_node_set_transform(node1, trans1);
  // other object..
  csg_node_t* node2 = csg_node_create(root, "Jill");
  csg_transform_t* trans2 = csg_transform_create(1.0f, 0.0f, 0.0f);
  csg_drawable_t* draw2 = csg_drawable_create(triangle, green);
  csg_node_set_drawable(node2, draw2);
  csg_node_set_transform(node2, trans2);
  // other object..
  csg_node_t* node3 = csg_node_create(root, NULL);
  csg_transform_t* trans3 = csg_transform_create(3.0f, 0.0f, 0.0f);
  csg_drawable_t* draw3 = csg_drawable_create(csg_geometry_create_cube(), red);
  csg_node_set_drawable(node3, draw3);
  csg_node_set_transform(node3, trans3);
  csg_animation_t* anim3 = csg_animation_create(CSG_ANIMATION_MODE_RESTART);
  csg_transform_set_translation_animation(trans3, anim3);
  csg_animation_add_point(anim3, 3.0f, 0.f, 0.f, 1.f);
  csg_animation_add_point(anim3, 4.0f, 0.f, 0.f, 1.f);
  csg_animation_add_point(anim3, 1.0f, 0.f, 8.f, 1.f);
  csg_animation_add_point(anim3, 1.0f, 0.f, 0.f, 1.f);
  csg_animation_add_point(anim3, 3.0f, 0.f, 0.f, 1.f);

  glfwSetTime(0.0);
  while (glfwWindowShouldClose(window) != GLFW_TRUE) {
    glfwPollEvents();

    int fbwidth, fbheight;
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);
    csg_viewport_reset(view, 0, 0, fbwidth, fbheight);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      printf("A\n");
      csg_transform_translate(trans1, -0.05f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      printf("D\n");
      csg_transform_translate(trans1, 0.05f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
      printf("PgUp\n");
      csg_transform_translate(camtrans, 0.0f, 0.0f, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
      printf("PgDn\n");
      csg_transform_translate(camtrans, 0.0f, 0.0f, -0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      printf("Left\n");
      csg_transform_translate(camtrans, 0.1f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      printf("Right\n");
      csg_transform_translate(camtrans, -0.1f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      printf("Up\n");
      csg_transform_translate(camtrans, 0.0f, -0.1f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      printf("Down\n");
      csg_transform_translate(camtrans, 0.0f, 0.1f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
      printf("U: ");
      csg_transform_translation_animation_update(trans3, 0.01f);
      //      csg_animation_update(anim3, 0.01f);
      float x, y, z, w;
      int cur_seg, total_segs;
      float interpolant =
          csg_animation_get_interpolant(anim3, &total_segs, &cur_seg);
      csg_animation_get_value(anim3, &x, &y, &z, &w);
      int flags = csg_animation_get_flags(anim3);

      printf(
          "cuurent vec4: %f, %f, %f, %f (seg %d/%d, interpolant: %f, flags: "
          "0x%x)\n",
          x, y, z, w, cur_seg, total_segs, interpolant, flags);
    }

    // update animations each frame
    csg_transform_translation_animation_update(trans3, 0.01f);

    csg_viewport_render(view, root);

    glfwSwapBuffers(window);
  }
}

static csg_node_t* make_cube(csg_node_t* parent, float x, float y, float z) {
  static csg_geometry_t* cube_geom = NULL;
  if (cube_geom == NULL) cube_geom = csg_geometry_create_cube();

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
  csg_transform_t* trans = csg_transform_create(x, y, z);
  csg_node_set_transform(node, trans);

  return node;
}

static void test3(GLFWwindow* window) {
  // XXX what's wrong with this code???:
  //  csg_viewport_t* view = csg_viewport_create(
  //      0, 0, 1024, 768,
  //      csg_camera_create(CSG_CAMERA_PROJECTION_PERSPECTIVE, 4 / 3));
  //  csg_camera_set_transform(csg_viewport_get_camera(view),
  //                           csg_transform_create(0.f, 0.f, 10.0f));

  csg_camera_t* cam =
      csg_camera_create(CSG_CAMERA_PROJECTION_PERSPECTIVE, 4 / 3);
  csg_transform_t* camtrans = csg_transform_create(-7.0f, -4.0f, -20.0f);
  csg_camera_set_transform(cam, camtrans);

  csg_viewport_t* view = csg_viewport_create(0, 0, 1024, 768, cam);
  csg_viewport_set_clear_color(view, 0.0f, 0.0f, 0.1f, 1.0f);

  csg_node_t* root = csg_node_create(NULL, NULL);
#define NCUBES 64
  csg_node_t* cubes[NCUBES];
  csg_animation_t* anims[NCUBES];
  for (size_t i = 0; i < NCUBES; i++) {
    static float row = 0;
    if (i % 8 == 0) row++;
    cubes[i] = make_cube(root, (i * 2) % (8 * 2), 2 * row, 0.f);
    float x, y, z;
    csg_transform_get_position(csg_node_get_transform(cubes[i]), &x, &y, &z);
    anims[i] = csg_animation_create(CSG_ANIMATION_MODE_OSCILLATE);
    csg_animation_add_point(anims[i], x, y, z, 1.f);
    csg_animation_add_point(anims[i], x, y, z + (arc4random() % 50), 1.f);

    //    csg_animation_add_point(anims[i], x, y, z, 1.f);
    //    csg_animation_add_point(anims[i], x, y, z - (arc4random() % 50), 1.f);
    csg_transform_set_translation_animation(csg_node_get_transform(cubes[i]),
                                            anims[i]);
  }

  while (glfwWindowShouldClose(window) != GLFW_TRUE) {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      printf("Left\n");
      csg_transform_translate(camtrans, 0.1f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      printf("Right\n");
      csg_transform_translate(camtrans, -0.1f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      printf("Up\n");
      csg_transform_translate(camtrans, 0.0f, -0.1f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      printf("Down\n");
      csg_transform_translate(camtrans, 0.0f, 0.1f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
      printf("PgUp\n");
      csg_transform_translate(camtrans, 0.0f, 0.0f, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
      printf("PgDn\n");
      csg_transform_translate(camtrans, 0.0f, 0.0f, -0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
      printf("Q\n");
      csg_transform_rotate(camtrans, 0.1f, 0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
      printf("E\n");
      csg_transform_rotate(camtrans, -0.1f, 0.0f, 1.0f, 0.0f);
    }

    //     proceed all animations
    for (size_t i = 0; i < NCUBES; i++) {
      csg_transform_translation_animation_update(
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

  //  test1();
  //  test2(window1);
  test3(window1);

  glfwDestroyWindow(window1);
  glfwTerminate();

  return 0;
}
