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
 * @brief Create transform object with identity matrices
 * @return
 */
extern csg_transform_t* csg_transform_create(void);
extern csg_transform_t* csg_transform_create_translated(float orig_x, float orig_y, float orig_z);
extern void csg_transform_get_position(csg_transform_t* trans, float* x, float* y, float* z);
extern void csg_transform_set_translation_animation(csg_transform_t* trans, csg_animation_t* anim);
extern void csg_transform_update_translation_animation(csg_transform_t* trans, float delta);
extern void csg_transform_translate(csg_transform_t* trans, float dx, float dy, float dz);
extern void csg_transform_rotate(csg_transform_t* trans, float angle_rad, float x, float y, float z);
extern void csg_transform_scale(csg_transform_t* trans, float sx, float sy, float sz);
