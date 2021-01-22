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
#include <csg/core.h>
#include <stdio.h>

void print_node(csg_node_t* node, void* cookie) {
  size_t indent_level = 0;
  csg_node_t* child = node;
  while (child->parent != NULL) {
    indent_level++;
    child = child->parent;
  }
  for (size_t i = 0; i < indent_level; i++) putchar(' ');

  printf("Node @%p, cookie: %s\n", node, node->cookie);
}

static void test1(void) {
  csg_node_t* root = csg_node_create(NULL, 1);
  csg_node_t* n1 = csg_node_create(root, 10);
  csg_node_t* n2 = csg_node_create(root, 20);
  csg_node_t* n3 = csg_node_create(n1, 300);
  csg_node_t* n4 = csg_node_create(n3, 4000);
  csg_node_t* n5 = csg_node_create(n3, 5000);
  csg_node_t* n6 = csg_node_create(n5, 60000);

  printf("From root node (DFS):\n");
  csg_node_traverse(root, CSG_TRAVERSE_DEPTH_FIRST, print_node, NULL);
  printf("From root node (S_A_C):\n");
  csg_node_traverse(root, CSG_TRAVERSE_SELF_AND_CHILDREN, print_node, NULL);
  printf("A leaf node (DFS):\n");
  csg_node_traverse(n6, CSG_TRAVERSE_DEPTH_FIRST, print_node, NULL);
  printf("A leaf node (P_F_R):\n");
  csg_node_traverse(n6, CSG_TRAVERSE_PATH_FROM_ROOT, print_node, NULL);
}

static void test2(void) {
  printf("TEST 2\n");
  csg_node_t* root = csg_node_create(NULL, "<ROOT>");
  csg_node_t* a = csg_node_create(root, "A");
  csg_node_t* b = csg_node_create(root, "B");
  csg_node_t* c = csg_node_create(root, "C");
  // a branch
  csg_node_t* aa = csg_node_create(a, "AA");
  csg_node_t* aab = csg_node_create(aa, "AAB");
  csg_node_t* ab = csg_node_create(a, "AB");
  // b branch
  csg_node_t* ba = csg_node_create(b, "BA");
  csg_node_t* bb = csg_node_create(b, "BB");
  csg_node_t* bba = csg_node_create(bb, "BBA");
  csg_node_t* bc = csg_node_create(b, "BC");
  // c branch
  csg_node_t* cc = csg_node_create(c, "CC");
  csg_node_t* ccc = csg_node_create(cc, "CCC");
  csg_node_t* cccc = csg_node_create(ccc, "CCCC");
  csg_node_t* cccd = csg_node_create(ccc, "CCCD");

  printf("TRAVERSE/PRINT:\n");
  csg_node_traverse(root, CSG_TRAVERSE_DEPTH_FIRST, print_node, NULL);

  {
    //    root->_counter = 100;
    //    a->_counter = -20;
    //    aa->_counter = -5;
    //    cc->_counter = +1;
    //    ccc->_counter = +1;
    //    cccd->_counter = -2;
  }

  //  printf("RENDER:\n");
  //  csg_render(root,
  //             csg_camera_create(CSG_PROJECTION_ORTHOGRAPHIC, (csg_vec3_t){},
  //                               (csg_vec3_t){}, (csg_vec3_t){}, 1.0f),
  //             (csg_vec4_t){});
}

int main(int argc, char** argv) {
  csg_set_malloc_debug(true);
  //  csg_init();

  //  test1();
  test2();

  return 0;
}
