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
#include <csg/image.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define rand(x) arc4random(x)

int main(int argc, char** argv) {
  //  csg_malloc_set_debug(true);

  csg_gui_adapter_t adapter = csg_gui_adapter_create(
      csg_gui_glfw3_adapter_ops(), 0, 0, 1024, 768, 0, NULL);

  csg_geometry_t octa_geometry = csg_geometry_create_quad();

  csg_texture_t tex = csg_texture_create_from_image(
      "/usr/home/nbrk/Downloads/junk/meshes/Map001c.bmp", true);

  csg_material_t material = csg_material_create();
  material.texture = tex;
  //  material.diffuse_color.y = 0.4f;
  //  material.diffuse_color = (csg_vec4_t){1.0f, 1.0f, 1.0f, 1.0f};

  csg_camera_t camera = csg_camera_default();
  camera.position.z = 1.f;

  // nodes
  csg_node_t* root = csg_node_create(NULL, NULL);

  csg_node_t* node = csg_node_create(root, NULL);
  node->geometry = octa_geometry;
  node->geometry.material = material;

  const float CAMERA_SPEED = 1.f;

  while ((adapter.flags & CSG_GUI_FLAG_WANT_CLOSE) == 0) {
    csg_gui_adapter_begin_frame(&adapter);

    {
      if (adapter.keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS)
        adapter.flags |= CSG_GUI_FLAG_WANT_CLOSE;

      if (adapter.keyboard[CSG_GUI_KEY_W] == CSG_GUI_PRESS) {
        csg_camera_move_by(&camera, CAMERA_SPEED * adapter.last_frame_duration,
                           0.f, 0.0f);
      }
      if (adapter.keyboard[CSG_GUI_KEY_S] == CSG_GUI_PRESS) {
        csg_camera_move_by(&camera, -CAMERA_SPEED * adapter.last_frame_duration,
                           0.f, 0.0f);
      }
      if (adapter.keyboard[CSG_GUI_KEY_A] == CSG_GUI_PRESS) {
        csg_camera_move_by(&camera, 0.f,
                           -CAMERA_SPEED * adapter.last_frame_duration, 0.0f);
      }
      if (adapter.keyboard[CSG_GUI_KEY_D] == CSG_GUI_PRESS) {
        csg_camera_move_by(&camera, 0.f,
                           CAMERA_SPEED * adapter.last_frame_duration, 0.0f);
      }
      if (adapter.keyboard[CSG_GUI_KEY_PAGE_UP] == CSG_GUI_PRESS) {
        csg_camera_move_by(&camera, 0.f, 0.0f, 0.5f);
      }
      if (adapter.keyboard[CSG_GUI_KEY_PAGE_DOWN] == CSG_GUI_PRESS) {
        csg_camera_move_by(&camera, 0.f, 0.0f, -0.5f);
      }
      if (adapter.keyboard[CSG_GUI_KEY_LEFT_BRACKET] == CSG_GUI_PRESS) {
        camera.far_plane -= 1.0f;
        printf("Camera far plane: %f\n", camera.far_plane);
      }
      if (adapter.keyboard[CSG_GUI_KEY_RIGHT_BRACKET] == CSG_GUI_PRESS) {
        camera.far_plane += 1.0f;
        printf("Camera far plane: %f\n", camera.far_plane);
      }

      if (adapter.keyboard[CSG_GUI_KEY_1] == CSG_GUI_PRESS) {
        node->geometry.gl.polygon_mode = GL_POINT;
      }
      if (adapter.keyboard[CSG_GUI_KEY_2] == CSG_GUI_PRESS) {
        node->geometry.gl.polygon_mode = GL_LINE;
      }
      if (adapter.keyboard[CSG_GUI_KEY_3] == CSG_GUI_PRESS) {
        node->geometry.gl.polygon_mode = GL_FILL;
      }

      if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_RIGHT]) {
        if (1 || adapter.mouse_deltax != 0) {
          //        printf("adapter.mouse.delta{x, y} = %d, %d\n",
          //        adapter.mouse_deltax,
          //               adapter.mouse_deltay);
          camera.horizontal_angle -= 0.005f * adapter.mouse_deltax;
        }
        if (1 || adapter.mouse_deltay != 0) {
          camera.vertical_angle -= 0.005f * adapter.mouse_deltay;
        }
      }
      if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_LEFT]) {
        if (adapter.mouse_deltax != 0)
          root->transform.rotation.y += 0.01f * adapter.mouse_deltax;
        if (adapter.mouse_deltay != 0)
          root->transform.rotation.x += 0.01f * adapter.mouse_deltay;
      }
      if (adapter.mouse[CSG_GUI_MOUSE_BUTTON_MIDDLE]) {
        if (adapter.mouse_deltay != 0) {
          root->transform.scale.x -= 0.01f * adapter.mouse_deltay;
          root->transform.scale.y -= 0.01f * adapter.mouse_deltay;
          root->transform.scale.z -= 0.01f * adapter.mouse_deltay;
        }
      }

      //      printf("adapter.last_frame_duration: %f\n",
      //      adapter.last_frame_duration);

      csg_render(root, camera, (csg_vec4_t){0.1f, 0.2f, 0.1f, 1.0f});
    }
    csg_gui_adapter_end_frame(&adapter);

    if (adapter.last_frame_duration < 1.0 / 120.0)
      usleep((1.0 / 120.0 - adapter.last_frame_duration) * 1000000);
  }

  return 0;
}
