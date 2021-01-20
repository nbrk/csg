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

static void test2(GLFWwindow* window) {
  struct nk_glfw* glfw = malloc(sizeof(*glfw));
  struct nk_context* nk =
      nk_glfw3_init(glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
  struct nk_font_atlas* atlas;
  nk_glfw3_font_stash_begin(glfw, &atlas);
  nk_glfw3_font_stash_end(glfw);

  //  csg_camera_t* cam =
  //      csg_camera_create(CSG_CAMERA_PROJECTION_PERSPECTIVE, 4 / 3);
  csg_transform_t* camtrans =
      csg_transform_create_translated(-10.0f, 0.0f, -50.0f);
  //  csg_camera_set_transform(cam, camtrans);

  csg_viewport_t* view = csg_viewport_create(0, 0, 1024, 768);
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
  csg_transform_t* trans = csg_transform_create_translated(-1.0f, 0.0f, 0.0f);
  csg_drawable_t* draw = csg_drawable_create(triangle, blue);
  csg_node_set_drawable(node, draw);
  csg_node_set_transform(node, trans);
  // other object..
  csg_node_t* node1 = csg_node_create(root, "Jack");
  csg_transform_t* trans1 = csg_transform_create_translated(0.0f, 0.0f, 0.0f);
  csg_drawable_t* draw1 = csg_drawable_create(triangle, red);
  csg_node_set_drawable(node1, draw1);
  csg_node_set_transform(node1, trans1);
  // other object..
  csg_node_t* node2 = csg_node_create(root, "Jill");
  csg_transform_t* trans2 = csg_transform_create_translated(1.0f, 0.0f, 0.0f);
  csg_drawable_t* draw2 = csg_drawable_create(triangle, green);
  csg_node_set_drawable(node2, draw2);
  csg_node_set_transform(node2, trans2);
  // other object..
  csg_node_t* node3 = csg_node_create(root, NULL);
  csg_transform_t* trans3 = csg_transform_create_translated(3.0f, 0.0f, 0.0f);
  csg_drawable_t* draw3 = csg_drawable_create(csg_geometry_create_cube(), red);
  csg_node_set_drawable(node3, draw3);
  csg_node_set_transform(node3, trans3);
  csg_animation_t* anim3 = csg_animation_create(CSG_ANIMATION_MODE_ONESHOT);
  csg_transform_set_translation_animation(trans3, anim3);
  csg_animation_add_point(anim3, 3.0f, 0.f, 0.f, 1.f);
  csg_animation_add_point(anim3, 10.0f, 0.f, 0.f, 1.f);
  //  csg_animation_add_point(anim3, 1.0f, 0.f, 8.f, 1.f);
  //  csg_animation_add_point(anim3, 1.0f, 0.f, 0.f, 1.f);
  //  csg_animation_add_point(anim3, 3.0f, 0.f, 0.f, 1.f);

  glfwSetTime(0.0);
  while (glfwWindowShouldClose(window) != GLFW_TRUE) {
    glfwPollEvents();

    int fbwidth, fbheight;
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);
    csg_viewport_reset(view, 0, 0, fbwidth, fbheight);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      printf("A\n");
      csg_transform_translate(trans3, -0.05f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      printf("D\n");
      csg_transform_translate(trans3, 0.05f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
      printf("PgUp\n");
      csg_transform_translate(trans3, 0.0f, 0.0f, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
      printf("PgDn\n");
      csg_transform_translate(trans3, 0.0f, 0.0f, -0.1f);
    }

    //    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //      printf("Left\n");
    //      csg_transform_translate(camtrans, 0.1f, 0.0f, 0.0f);
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //      printf("Right\n");
    //      csg_transform_translate(camtrans, -0.1f, 0.0f, 0.0f);
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //      printf("Up\n");
    //      csg_transform_translate(camtrans, 0.0f, -0.1f, 0.0f);
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    //      printf("Down\n");
    //      csg_transform_translate(camtrans, 0.0f, 0.1f, 0.0f);
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
    //      printf("U: ");
    //      csg_transform_update_translation_animation(trans3, 0.01f);
    //      //      csg_animation_update(anim3, 0.01f);
    //      float x, y, z, w;
    //      int cur_seg, total_segs;
    //      float interpolant =
    //          csg_animation_get_interpolant(anim3, &total_segs, &cur_seg);
    //      csg_animation_get_value(anim3, &x, &y, &z, &w);
    //      int flags = csg_animation_get_flags(anim3);

    //      printf(
    //          "cuurent vec4: %f, %f, %f, %f (seg %d/%d, interpolant: %f,
    //          flags: " "0x%x)\n", x, y, z, w, cur_seg, total_segs,
    //          interpolant, flags);
    //    }

    //    // update animations each frame
    //       csg_transform_translation_animation_update(trans3, 0.01f);

    csg_viewport_render(view, root);

    // UI
    nk_glfw3_new_frame(glfw);
    if (nk_begin(nk, "Cube control", nk_rect(100, 100, 320, 240),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
      nk_layout_row_dynamic(nk, 25, 1);
      static float trans_x, trans_y, trans_z;
      nk_property_float(nk, "Position X", -100.0f, &trans_x, 100.f, 1.f, 0.01f);
      nk_property_float(nk, "Position Y", -100.0f, &trans_y, 100.f, 1.f, 0.01f);
      nk_property_float(nk, "Position Z", -100.0f, &trans_z, 100.f, 1.f, 0.01f);

      csg_transform_set_position(trans3, trans_x, trans_y, trans_z);
    }
    nk_end(nk);
    if (nk_begin(nk, "Camera", nk_rect(400, 400, 320, 240),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
      nk_layout_row_dynamic(nk, 25, 1);
      float x, y, z;
      csg_camera_get_position(csg_viewport_get_camera(view), &x, &y, &z);
      nk_property_float(nk, "Position X", -100.0f, &x, 100.f, 1.f, 0.01f);
      nk_property_float(nk, "Position Y", -100.0f, &y, 100.f, 1.f, 0.01f);
      nk_property_float(nk, "Position Z", -100.0f, &z, 100.f, 1.f, 0.01f);

      csg_camera_set_position(csg_viewport_get_camera(view), x, y, z, true);
    }
    nk_end(nk);

    nk_glfw3_render(glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER,
                    MAX_ELEMENT_BUFFER);
    glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(window);
  }

  nk_glfw3_shutdown(glfw);
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

  test2(window1);

  glfwDestroyWindow(window1);
  glfwTerminate();

  return 0;
}
