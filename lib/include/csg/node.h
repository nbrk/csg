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

#include <stdlib.h>

/**
 * @brief Create a new root node or a child node
 * @param parent pointer to a parent node or NULL if the node is a root
 * @return
 */
extern csg_node_t* csg_node_create(csg_node_t* parent, void* cookie);
extern void csg_node_destroy_recursively(csg_node_t* node);
extern void csg_node_apply_depth_first(csg_node_t* node, void (*func)(csg_node_t*, void*), void* func_cookie);
extern void* csg_node_get_cookie(csg_node_t* node);
extern csg_node_t** csg_node_get_children(csg_node_t* node, size_t* num_children);
extern csg_node_t* csg_node_get_parent(csg_node_t* node);
extern void csg_node_set_transform(csg_node_t* node, csg_transform_t* trans);
extern csg_transform_t* csg_node_get_transform(csg_node_t* node);
extern void csg_node_set_drawable(csg_node_t* node, csg_drawable_t* draw);
