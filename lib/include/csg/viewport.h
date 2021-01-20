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
#include <csg/types.h>

/**
 * @brief Create a viewport in the active OpenGL context
 * @param x
 * @param y
 * @param width
 * @param height
 * @param cam
 * @return
 */
extern csg_viewport_t* csg_viewport_create(int x, int y, int width, int height);

/**
 * @brief Reset viewport dimensions
 * @param view
 * @param x
 * @param y
 * @param width
 * @param height
 */
extern void csg_viewport_reset(csg_viewport_t* view, int x, int y, int width, int height);

/**
 * @brief Render subtree headed by a node
 * @param view
 * @param root subtree to draw
 */
extern void csg_viewport_render(csg_viewport_t* view, csg_node_t* root);

extern void csg_viewport_set_clear_color(csg_viewport_t* view, float r, float g, float b, float a);
extern csg_camera_t* csg_viewport_get_camera(csg_viewport_t* view);
