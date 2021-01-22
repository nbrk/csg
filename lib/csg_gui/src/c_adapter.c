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
#include <assert.h>
#include <csg/gui.h>
#include <stdlib.h>

void csg_gui_adapter_update_input(csg_gui_adapter_t* adapter) {
  assert(adapter->ops.update_input_func != NULL);

  adapter->ops.update_input_func(adapter);
}

csg_gui_adapter_t csg_gui_adapter_create(csg_gui_adapter_ops_t backend_ops,
                                         int width, int height, int flags) {
  assert(backend_ops.create_func != NULL);

  csg_gui_adapter_t adapter = backend_ops.create_func(width, height, flags);
  adapter.ops = backend_ops;
  return adapter;
}

void csg_gui_adapter_destroy(csg_gui_adapter_t* adapter) {
  assert(adapter->ops.destroy_func != NULL);

  adapter->ops.destroy_func(adapter);
}

void csg_gui_adapter_begin_frame(csg_gui_adapter_t* adapter) {
  assert(adapter->ops.begin_frame_func != NULL);
  adapter->ops.begin_frame_func(adapter);
}

void csg_gui_adapter_end_frame(csg_gui_adapter_t* adapter) {
  assert(adapter->ops.end_frame_func != NULL);
  adapter->ops.end_frame_func(adapter);
}
