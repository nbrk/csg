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
#include <csg/core.h>
#include <string.h>

csg_node_t* csg_node_create(csg_node_t* parent, void* cookie) {
  csg_node_t* node = csg_malloc(sizeof(*node));

  node->parent = parent;
  node->children = NULL;
  node->num_children = 0;
  node->cookie = cookie;
  node->transform.rotation = (csg_vec3_t){0.0f, 0.0f, 0.0f};
  node->transform.translation = (csg_vec3_t){0.0f, 0.0f, 0.0f};
  node->transform.scale = (csg_vec3_t){1.0f, 1.0f, 1.0f};
  node->geometry = csg_geometry_none();

  // TODO: support dynamic child/parent relationships
  if (parent != NULL) {
    parent->num_children += 1;
    parent->children = csg_realloc(parent->children,
                                   parent->num_children * sizeof(csg_node_t*));
    parent->children[parent->num_children - 1] = node;
  }

  return node;
}

void csg_node_traverse(csg_node_t* node,
                       csg_traverse_mode_e type,
                       void (*func)(csg_node_t*, void*),
                       void* func_cookie) {
  assert(node != NULL);

  if (type == CSG_TRAVERSE_ONLY_SELF) {
    func(node, func_cookie);
  }
  if (type == CSG_TRAVERSE_ONLY_CHILDREN) {
    for (size_t i = 0; i < node->num_children; i++)
      func(node->children[i], func_cookie);
  }
  if (type == CSG_TRAVERSE_SELF_AND_CHILDREN) {
    func(node, func_cookie);
    for (size_t i = 0; i < node->num_children; i++)
      func(node->children[i], func_cookie);
  }
  if (type == CSG_TRAVERSE_DEPTH_FIRST) {
    func(node, func_cookie);
    for (size_t i = 0; i < node->num_children; i++)
      csg_node_traverse(node->children[i], CSG_TRAVERSE_DEPTH_FIRST, func,
                        func_cookie);
  }
  if (type == CSG_TRAVERSE_BREADTH_FIRST) {
    // TODO: breadth-first search
    assert(0 && "TODO");
  }
  if (type == CSG_TRAVERSE_PATH_FROM_ROOT) {
    // WARNING: assuming the tree to be no more than 1024 parents depth
    csg_node_t* parents_to_root[1024];
    memset(parents_to_root, (int)NULL, 1024);
    size_t num_parents_to_root = 0;
    csg_node_t* parent = node->parent;
    while (parent != NULL) {
      parents_to_root[num_parents_to_root] = parent;
      num_parents_to_root++;
      parent = parent->parent;
    }
    for (size_t i = num_parents_to_root; i > 0; i--)
      func(parents_to_root[i - 1], func_cookie);
    // all parents starting from the root are done; now do ourselfes
    func(node, func_cookie);
  }
  if (type == CSG_TRAVERSE_PATH_TO_ROOT) {
    func(node, func_cookie);
    csg_node_t* parent = node->parent;
    while (parent != NULL)
      csg_node_traverse(parent, CSG_TRAVERSE_PATH_TO_ROOT, func, func_cookie);
  }
}
