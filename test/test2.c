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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <csg/core.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "nuklear.h"

static void test2(GLFWwindow* window) {
  struct nk_glfw* glfw = malloc(sizeof(*glfw));
  struct nk_context* nk =
      nk_glfw3_init(glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
  struct nk_font_atlas* atlas;
  nk_glfw3_font_stash_begin(glfw, &atlas);
  nk_glfw3_font_stash_end(glfw);

  // reusable geometry
  //  csg_geometry_t* triangle = csg_geometry_create_triangle();
  csg_geometry_t* cube = csg_geometry_create_cube();

  // root
  csg_node_t* root = csg_node_create(NULL, NULL);
  root->geometry = cube;
  root->geometry->material = csg_material_create();
  // first object..
  csg_node_t* node1 = csg_node_create(root, NULL);
  node1->transform.translation.x = 10.0f;
  node1->geometry = cube;
  node1->geometry->material = csg_material_create();
  csg_node_t* node2 = csg_node_create(root, NULL);
  node2->transform.translation.x = -10.0f;
  node2->geometry = cube;
  node2->geometry->material = csg_material_create();

  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  csg_camera_t camera = csg_camera_create(
      CSG_PROJECTION_PERSPECTIVE, (csg_vec3_t){0.f, 0.f, 10.f},
      (csg_vec3_t){0.f, 0.f, 0.f}, (csg_vec3_t){0.f, 1.f, 0.f},
      (float)mode->width / (float)mode->height);

  csg_vec4_t clear_color = {0.0f, 0.2f, 0.0f, 1.0f};

  glfwSetTime(0.0);
  while (glfwWindowShouldClose(window) != GLFW_TRUE) {
    glfwPollEvents();

    //    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //      printf("A\n");
    //      csg_transform_translate(trans3, -0.05f, 0.0f, 0.0f);
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //      printf("D\n");
    //      csg_transform_translate(trans3, 0.05f, 0.0f, 0.0f);
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
    //      printf("PgUp\n");
    //      csg_transform_translate(trans3, 0.0f, 0.0f, 0.1f);
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
    //      printf("PgDn\n");
    //      csg_transform_translate(trans3, 0.0f, 0.0f, -0.1f);
    //    }

    csg_render(root, camera, clear_color);

    // UI
    nk_glfw3_new_frame(glfw);
    if (nk_begin(nk, "Render control", nk_rect(100, 100, 240, 160),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
      nk_layout_row_dynamic(nk, 25, 1);
      nk_property_float(nk, "Clear color Red", 0.0f, &clear_color.x, 1.0f,
                        0.01f, 0.001f);
      nk_property_float(nk, "Clear color Green", 0.0f, &clear_color.y, 1.0f,
                        0.01f, 0.001f);
      nk_property_float(nk, "Clear color Blue", 0.0f, &clear_color.z, 1.0f,
                        0.01f, 0.001f);
    }
    nk_end(nk);
    if (nk_begin(nk, "Material control", nk_rect(600, 500, 240, 160),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
      nk_layout_row_dynamic(nk, 25, 1);
      nk_property_float(nk, "Material color Red", 0.0f,
                        &root->geometry->material.diffuse_color.x, 1.0f, 0.01f,
                        0.001f);
      nk_property_float(nk, "Material color Green", 0.0f,
                        &root->geometry->material.diffuse_color.y, 1.0f, 0.01f,
                        0.001f);
      nk_property_float(nk, "Material color Blue", 0.0f,
                        &root->geometry->material.diffuse_color.z, 1.0f, 0.01f,
                        0.001f);
      nk_property_float(nk, "Material color Alpha", 0.0f,
                        &root->geometry->material.diffuse_color.w, 1.0f, 0.01f,
                        0.001f);
    }
    nk_end(nk);
    if (nk_begin(nk, "Model control", nk_rect(200, 100, 200, 440),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
      nk_layout_row_dynamic(nk, 25, 2);
      if (nk_button_label(nk, "CUBE")) {
        root->geometry = cube;
      }
      if (nk_button_label(nk, "TRI")) {
        //        root->geometry = triangle;
      }
      nk_layout_row_dynamic(nk, 25, 1);
      nk_label(nk, "Rotation:", NK_LEFT);
      nk_property_float(nk, "#X", 0.0f, &root->transform.rotation.x, M_PI * 2,
                        0.01f, 0.001f);
      nk_property_float(nk, "#Y", 0.0f, &root->transform.rotation.y, M_PI * 2,
                        0.01f, 0.001f);
      nk_property_float(nk, "#Z", 0.0f, &root->transform.rotation.z, M_PI * 2,
                        0.01f, 0.001f);
      nk_label(nk, "Translation:", NK_LEFT);
      nk_property_float(nk, "#X", -1000.0f, &root->transform.translation.x,
                        1000.0f, 1.0f, 0.1f);
      nk_property_float(nk, "#Y", -1000.0f, &root->transform.translation.y,
                        1000.0f, 1.0f, 0.1f);
      nk_property_float(nk, "#Z", -1000.0f, &root->transform.translation.z,
                        1000.0f, 1.0f, 0.1f);
      nk_label(nk, "Scale:", NK_LEFT);
      nk_property_float(nk, "#X", 0.0f, &root->transform.scale.x, 100.0f, 0.1f,
                        0.01f);
      nk_property_float(nk, "#Y", 0.0f, &root->transform.scale.y, 100.0f, 0.1f,
                        0.01f);
      nk_property_float(nk, "#Z", 0.0f, &root->transform.scale.z, 100.0f, 0.1f,
                        0.01f);
    }
    nk_end(nk);
    if (nk_begin(nk, "Camera control", nk_rect(300, 300, 160, 400),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
      nk_layout_row_dynamic(nk, 25, 1);
      nk_label(nk, "Position:", NK_LEFT);
      nk_property_float(nk, "#X", -1000.0f, &camera.position.x, 1000.0f, 1.0f,
                        0.01f);
      nk_property_float(nk, "#Y", -1000.0f, &camera.position.y, 1000.0f, 1.00f,
                        0.01f);
      nk_property_float(nk, "#Z", -1000.0f, &camera.position.z, 1000.0f, 1.00f,
                        0.01f);
      nk_label(nk, "Target:", NK_LEFT);
      nk_property_float(nk, "#X", -1000.0f, &camera.target.x, 1000.0f, 1.0f,
                        0.01f);
      nk_property_float(nk, "#Y", -1000.0f, &camera.target.y, 1000.0f, 1.0f,
                        0.01f);
      nk_property_float(nk, "#Z", -1000.0f, &camera.target.z, 1000.0f, 1.0f,
                        0.001f);
      nk_label(nk, "Up vector:", NK_LEFT);
      nk_property_float(nk, "#X", 0.0f, &camera.up.x, 1.0f, 0.01f, 0.001f);
      nk_property_float(nk, "#Y", 0.0f, &camera.up.y, 1.0f, 0.01f, 0.001f);
      nk_property_float(nk, "#Z", 0.0f, &camera.up.z, 1.0f, 0.01f, 0.001f);
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
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  GLFWwindow* window1 = glfwCreateWindow(mode->width, mode->height, "GLFW",
                                         glfwGetPrimaryMonitor(), NULL);
  glfwMakeContextCurrent(window1);
  glfwSetInputMode(window1, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  csg_set_malloc_debug(true);
  //  csg_init();

  test2(window1);

  glfwDestroyWindow(window1);
  glfwTerminate();

  return 0;
}
