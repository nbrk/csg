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

#include "test4_ui.h"

#include <stdlib.h>

#include "nuklear.h"

static void ui_update_rendering(struct nk_context* nk,
                                struct ui_cookie* cookie);

struct nk_glfw* ui_init(void* glfwwindow) {
  struct nk_glfw* glfw = malloc(sizeof(*glfw));
  nk_glfw3_init(glfw, glfwwindow, NK_GLFW3_INSTALL_CALLBACKS);
  struct nk_font_atlas* atlas;
  nk_glfw3_font_stash_begin(glfw, &atlas);
  nk_glfw3_font_stash_end(glfw);
  return glfw;
}

void ui_draw(struct nk_glfw* nk_glfw) {
  nk_glfw3_render(nk_glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER,
                  MAX_ELEMENT_BUFFER);
  glEnable(GL_DEPTH_TEST);
}

void ui_update(struct nk_glfw* nk_glfw, void* cookie) {
  nk_glfw3_new_frame(nk_glfw);
  struct nk_context* nk = &nk_glfw->ctx;

  ui_update_rendering(nk, cookie);
}

void ui_update_rendering(struct nk_context* nk, struct ui_cookie* cookie) {
  csg_vec4_t* clear_color = &cookie->clear_color;

  if (nk_begin(nk, "Render control", nk_rect(100, 100, 240, 160),
               NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                   NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)) {
    nk_layout_row_dynamic(nk, 25, 1);
    nk_property_float(nk, "Clear color Red", 0.0f, &clear_color->x, 1.0f, 0.01f,
                      0.001f);
    nk_property_float(nk, "Clear color Green", 0.0f, &clear_color->y, 1.0f,
                      0.01f, 0.001f);
    nk_property_float(nk, "Clear color Blue", 0.0f, &clear_color->z, 1.0f,
                      0.01f, 0.001f);
  }
  nk_end(nk);
}
