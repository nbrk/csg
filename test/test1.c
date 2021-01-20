/*
 * Copyright 2020 Nikolay Burkov <nbrk@linklevel.net>
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
#include <csg/csg.h>
#include <stdio.h>

void print_node(csg_node_t* node, void* cookie) {
  size_t indent_level = 0;
  csg_node_t* child = node;
  while (csg_node_get_parent(child) != NULL) {
    indent_level++;
    child = csg_node_get_parent(child);
  }
  for (size_t i = 0; i < indent_level; i++)
    putchar(' ');

  printf("Node @%p, cookie: %d\n", node, csg_node_get_cookie(node));
}

static void test1(void) {
  csg_node_t* root = csg_node_create(NULL, 1);
  csg_node_t* n1 = csg_node_create(root, 10);
  csg_node_t* n2 = csg_node_create(root, 20);
  csg_node_t* n3 = csg_node_create(n1, 300);
  csg_node_t* n4 = csg_node_create(n3, 4000);
  csg_node_t* n5 = csg_node_create(n3, 5000);
  csg_node_t* n6 = csg_node_create(n5, 60000);

  printf("From root node:\n");
  csg_node_apply_depth_first(root, print_node, NULL);
  printf("A leaf node:\n");
  csg_node_apply_depth_first(n6, print_node, NULL);
}

int main(int argc, char** argv) {
  csg_set_malloc_debug(true);
  csg_init();

  test1();

  return 0;
}
