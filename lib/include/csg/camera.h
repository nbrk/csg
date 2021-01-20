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

#include <stdbool.h>

/*
 * CSG camera API
 */

/**
 * @brief Create default perspective projection camera. The camera is positioned
 * at (0,0,1), pointing at the center (0,0,0), with positive Y up
 * @param proj
 * @param aspect
 * @return
 */
extern csg_camera_t* csg_camera_create(float aspect);

extern void csg_camera_set_projection(csg_camera_t* cam, csg_camera_projection_e proj);
extern void csg_camera_set_position(csg_camera_t* cam, float x, float y, float z, bool keep_center);
extern void csg_camera_set_center(csg_camera_t* cam, float x, float y, float z);

extern void csg_camera_get_position(csg_camera_t* cam, float* x, float* y, float *z);
extern void csg_camera_get_center(csg_camera_t* cam, float* x, float* y, float *z);
