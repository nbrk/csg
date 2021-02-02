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

#include <csg/core.h>
#include <csg/gui.h>
#include <printf.h>

void camera_control(csg_camera_t* camera, csg_gui_adapter_t* adapter) {
  const float CAMERA_SPEED = 1.f;

  if (adapter->keyboard[CSG_GUI_KEY_ESCAPE] == CSG_GUI_PRESS)
    adapter->flags |= CSG_GUI_FLAG_WANT_CLOSE;

  if (adapter->keyboard[CSG_GUI_KEY_W] == CSG_GUI_PRESS) {
    csg_camera_move_by(camera, CAMERA_SPEED * adapter->last_frame_duration, 0.f,
                       0.0f);
  }
  if (adapter->keyboard[CSG_GUI_KEY_S] == CSG_GUI_PRESS) {
    csg_camera_move_by(camera, -CAMERA_SPEED * adapter->last_frame_duration,
                       0.f, 0.0f);
  }
  if (adapter->keyboard[CSG_GUI_KEY_A] == CSG_GUI_PRESS) {
    csg_camera_move_by(camera, 0.f,
                       -CAMERA_SPEED * adapter->last_frame_duration, 0.0f);
  }
  if (adapter->keyboard[CSG_GUI_KEY_D] == CSG_GUI_PRESS) {
    csg_camera_move_by(camera, 0.f, CAMERA_SPEED * adapter->last_frame_duration,
                       0.0f);
  }
  if (adapter->keyboard[CSG_GUI_KEY_LEFT_BRACKET] == CSG_GUI_PRESS) {
    camera->far_plane -= 1.0f;
    printf("Camera far plane: %f\n", camera->far_plane);
  }
  if (adapter->keyboard[CSG_GUI_KEY_RIGHT_BRACKET] == CSG_GUI_PRESS) {
    camera->far_plane += 1.0f;
    printf("Camera far plane: %f\n", camera->far_plane);
  }

  if (adapter->mouse[CSG_GUI_MOUSE_BUTTON_RIGHT]) {
    if (1 || adapter->mouse_deltax != 0) {
      camera->horizontal_angle -= 0.005f * adapter->mouse_deltax;
    }
    if (1 || adapter->mouse_deltay != 0) {
      camera->vertical_angle -= 0.005f * adapter->mouse_deltay;
    }
  }
}
