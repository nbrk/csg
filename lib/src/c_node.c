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
#include <stdio.h>

#include "csg_internal.h"

csg_node_t* csg_node_create(csg_node_t* parent, void* cookie) {
  csg_node_t* node = csg_malloc(sizeof(*node));

  node->parent = parent;
  node->children = NULL;
  node->num_children = 0;
  node->cookie = cookie;
  node->transform = csg_transform_create();
  node->drawable = NULL;

  if (parent != NULL) {
    parent->num_children += 1;
    parent->children = csg_realloc(parent->children,
                                   parent->num_children * sizeof(csg_node_t*));
    parent->children[parent->num_children - 1] = node;
  }

  return node;
}

void csg_node_apply_depth_first(csg_node_t* node,
                                void (*func)(csg_node_t*, void*),
                                void* func_cookie) {
  func(node, func_cookie);
  if (node->num_children == 0)
    return;
  else {
    for (size_t i = 0; i < node->num_children; i++)
      csg_node_apply_depth_first(node->children[i], func, func_cookie);
    return;
  }
}

void* csg_node_get_cookie(csg_node_t* node) {
  return node->cookie;
}

csg_node_t** csg_node_get_children(csg_node_t* node, size_t* num_children) {
  *num_children = node->num_children;
  return node->children;
}

csg_node_t* csg_node_get_parent(csg_node_t* node) {
  return node->parent;
}

void csg_node_set_transform(csg_node_t* node, csg_transform_t* trans) {
  node->transform = trans;
}

void csg_node_set_drawable(csg_node_t* node, csg_drawable_t* draw) {
  node->drawable = draw;
}

extern csg_transform_t* csg_node_get_transform(csg_node_t* node) {
  return node->transform;
}
