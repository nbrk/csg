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
#pragma once

#include <stdbool.h>

/*
 *
 * C Scene-Graph (GUI abstraction layer)
 *
 */

/******************************************************************************
 *
 * CONSTANTS/ENUMS
 */
#define CSG_GUI_FLAG_RUNNING 1
#define CSG_GUI_FLAG_FULLSCREEN 2
#define CSG_GUI_FLAG_WANT_CLOSE 4

/******************************************************************************
 *
 * DATA
 */
typedef struct csg_gui_adapter_t csg_gui_adapter_t;
typedef struct csg_gui_adapter_ops_t csg_gui_adapter_ops_t;

struct csg_gui_adapter_ops_t {
  csg_gui_adapter_t (*create_func)(int width, int height, int flags);
  void (*destroy_func)(csg_gui_adapter_t* adapter);
  void (*update_input_func)(csg_gui_adapter_t* adapter);
  void (*begin_frame_func)(csg_gui_adapter_t* adapter);
  void (*end_frame_func)(csg_gui_adapter_t* adapter);
};

struct csg_gui_adapter_t {
  void* cookie;
  int flags;

  // adapter backend (adaptee) ops
  csg_gui_adapter_ops_t ops;

  // viewport state
  int screen_width;
  int screen_height;

  // mouse state
  int mouse_x;
  int mouse_y;
};

/******************************************************************************
 *
 * FUNCTIONS
 */
extern csg_gui_adapter_t csg_gui_adapter_create(
    csg_gui_adapter_ops_t backend_ops, int width, int height, int flags);
extern void csg_gui_adapter_destroy(csg_gui_adapter_t* adapter);
extern void csg_gui_adapter_update_input(csg_gui_adapter_t* adapter);
extern void csg_gui_adapter_begin_frame(csg_gui_adapter_t* adapter);
extern void csg_gui_adapter_end_frame(csg_gui_adapter_t* adapter);
