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

#include <stdbool.h>

/*
 *
 * C Scene-Graph (GUI abstraction layer)
 *
 */

/******************************************************************************
 *
 * CONSTANTS/ENUMS
 */
#define CSG_GUI_FLAG_RUNNING 1
#define CSG_GUI_FLAG_FULLSCREEN 2
#define CSG_GUI_FLAG_WANT_CLOSE 4

// Keyboard printable keys
#define CSG_GUI_KEY_SPACE 32
#define CSG_GUI_KEY_APOSTROPHE 39 /* ' */
#define CSG_GUI_KEY_COMMA 44      /* , */
#define CSG_GUI_KEY_MINUS 45      /* - */
#define CSG_GUI_KEY_PERIOD 46     /* . */
#define CSG_GUI_KEY_SLASH 47      /* / */
#define CSG_GUI_KEY_0 48
#define CSG_GUI_KEY_1 49
#define CSG_GUI_KEY_2 50
#define CSG_GUI_KEY_3 51
#define CSG_GUI_KEY_4 52
#define CSG_GUI_KEY_5 53
#define CSG_GUI_KEY_6 54
#define CSG_GUI_KEY_7 55
#define CSG_GUI_KEY_8 56
#define CSG_GUI_KEY_9 57
#define CSG_GUI_KEY_SEMICOLON 59 /* ; */
#define CSG_GUI_KEY_EQUAL 61     /* = */
#define CSG_GUI_KEY_A 65
#define CSG_GUI_KEY_B 66
#define CSG_GUI_KEY_C 67
#define CSG_GUI_KEY_D 68
#define CSG_GUI_KEY_E 69
#define CSG_GUI_KEY_F 70
#define CSG_GUI_KEY_G 71
#define CSG_GUI_KEY_H 72
#define CSG_GUI_KEY_I 73
#define CSG_GUI_KEY_J 74
#define CSG_GUI_KEY_K 75
#define CSG_GUI_KEY_L 76
#define CSG_GUI_KEY_M 77
#define CSG_GUI_KEY_N 78
#define CSG_GUI_KEY_O 79
#define CSG_GUI_KEY_P 80
#define CSG_GUI_KEY_Q 81
#define CSG_GUI_KEY_R 82
#define CSG_GUI_KEY_S 83
#define CSG_GUI_KEY_T 84
#define CSG_GUI_KEY_U 85
#define CSG_GUI_KEY_V 86
#define CSG_GUI_KEY_W 87
#define CSG_GUI_KEY_X 88
#define CSG_GUI_KEY_Y 89
#define CSG_GUI_KEY_Z 90
#define CSG_GUI_KEY_LEFT_BRACKET 91  /* [ */
#define CSG_GUI_KEY_BACKSLASH 92     /* \ */
#define CSG_GUI_KEY_RIGHT_BRACKET 93 /* ] */
#define CSG_GUI_KEY_GRAVE_ACCENT 96  /* ` */
#define CSG_GUI_KEY_WORLD_1 161      /* non-US #1 */
#define CSG_GUI_KEY_WORLD_2 162      /* non-US #2 */
// Keyboard function keys
#define CSG_GUI_KEY_ESCAPE 256
#define CSG_GUI_KEY_ENTER 257
#define CSG_GUI_KEY_TAB 258
#define CSG_GUI_KEY_BACKSPACE 259
#define CSG_GUI_KEY_INSERT 260
#define CSG_GUI_KEY_DELETE 261
#define CSG_GUI_KEY_RIGHT 262
#define CSG_GUI_KEY_LEFT 263
#define CSG_GUI_KEY_DOWN 264
#define CSG_GUI_KEY_UP 265
#define CSG_GUI_KEY_PAGE_UP 266
#define CSG_GUI_KEY_PAGE_DOWN 267
#define CSG_GUI_KEY_HOME 268
#define CSG_GUI_KEY_END 269
#define CSG_GUI_KEY_CAPS_LOCK 280
#define CSG_GUI_KEY_SCROLL_LOCK 281
#define CSG_GUI_KEY_NUM_LOCK 282
#define CSG_GUI_KEY_PRINT_SCREEN 283
#define CSG_GUI_KEY_PAUSE 284
#define CSG_GUI_KEY_F1 290
#define CSG_GUI_KEY_F2 291
#define CSG_GUI_KEY_F3 292
#define CSG_GUI_KEY_F4 293
#define CSG_GUI_KEY_F5 294
#define CSG_GUI_KEY_F6 295
#define CSG_GUI_KEY_F7 296
#define CSG_GUI_KEY_F8 297
#define CSG_GUI_KEY_F9 298
#define CSG_GUI_KEY_F10 299
#define CSG_GUI_KEY_F11 300
#define CSG_GUI_KEY_F12 301
#define CSG_GUI_KEY_F13 302
#define CSG_GUI_KEY_F14 303
#define CSG_GUI_KEY_F15 304
#define CSG_GUI_KEY_F16 305
#define CSG_GUI_KEY_F17 306
#define CSG_GUI_KEY_F18 307
#define CSG_GUI_KEY_F19 308
#define CSG_GUI_KEY_F20 309
#define CSG_GUI_KEY_F21 310
#define CSG_GUI_KEY_F22 311
#define CSG_GUI_KEY_F23 312
#define CSG_GUI_KEY_F24 313
#define CSG_GUI_KEY_F25 314
#define CSG_GUI_KEY_KP_0 320
#define CSG_GUI_KEY_KP_1 321
#define CSG_GUI_KEY_KP_2 322
#define CSG_GUI_KEY_KP_3 323
#define CSG_GUI_KEY_KP_4 324
#define CSG_GUI_KEY_KP_5 325
#define CSG_GUI_KEY_KP_6 326
#define CSG_GUI_KEY_KP_7 327
#define CSG_GUI_KEY_KP_8 328
#define CSG_GUI_KEY_KP_9 329
#define CSG_GUI_KEY_KP_DECIMAL 330
#define CSG_GUI_KEY_KP_DIVIDE 331
#define CSG_GUI_KEY_KP_MULTIPLY 332
#define CSG_GUI_KEY_KP_SUBTRACT 333
#define CSG_GUI_KEY_KP_ADD 334
#define CSG_GUI_KEY_KP_ENTER 335
#define CSG_GUI_KEY_KP_EQUAL 336
#define CSG_GUI_KEY_LEFT_SHIFT 340
#define CSG_GUI_KEY_LEFT_CONTROL 341
#define CSG_GUI_KEY_LEFT_ALT 342
#define CSG_GUI_KEY_LEFT_SUPER 343
#define CSG_GUI_KEY_RIGHT_SHIFT 344
#define CSG_GUI_KEY_RIGHT_CONTROL 345
#define CSG_GUI_KEY_RIGHT_ALT 346
#define CSG_GUI_KEY_RIGHT_SUPER 347
#define CSG_GUI_KEY_MENU 348
// Total size of the keys array
#define CSG_GUI_NUM_KEYS CSG_GUI_KEY_MENU + 1

// Mouse buttons
#define CSG_GUI_MOUSE_BUTTON_1 0
#define CSG_GUI_MOUSE_BUTTON_2 1
#define CSG_GUI_MOUSE_BUTTON_3 2
#define CSG_GUI_MOUSE_BUTTON_4 3
#define CSG_GUI_MOUSE_BUTTON_5 4
#define CSG_GUI_MOUSE_BUTTON_LEFT CSG_GUI_MOUSE_BUTTON_1
#define CSG_GUI_MOUSE_BUTTON_RIGHT CSG_GUI_MOUSE_BUTTON_2
#define CSG_GUI_MOUSE_BUTTON_MIDDLE CSG_GUI_MOUSE_BUTTON_3
// Total size of mouse buttons array
#define CSG_GUI_NUM_MOUSE_BUTTONS CSG_GUI_MOUSE_BUTTON_5 + 1

// Key/mouse button state
#define CSG_GUI_RELEASE 0
#define CSG_GUI_PRESS 1
#define CSG_GUI_REPEAT 2

/******************************************************************************
 *
 * DATA
 */
typedef struct csg_gui_adapter_t csg_gui_adapter_t;
typedef struct csg_gui_adapter_ops_t csg_gui_adapter_ops_t;

struct csg_gui_adapter_ops_t {
  csg_gui_adapter_t (*create_func)(int width, int height, int flags);
  void (*destroy_func)(csg_gui_adapter_t* adapter);
  void (*update_func)(csg_gui_adapter_t* adapter);
  void (*begin_frame_func)(csg_gui_adapter_t* adapter);
  void (*end_frame_func)(csg_gui_adapter_t* adapter);
};

struct csg_gui_adapter_t {
  void* cookie;
  int flags;

  // adapter backend (adaptee) ops
  csg_gui_adapter_ops_t ops;

  // viewport state
  int screen_width;
  int screen_height;

  // mouse state
  int mouse_x;
  int mouse_y;
  char mouse[CSG_GUI_NUM_MOUSE_BUTTONS];

  // keyboard state
  char keyboard[CSG_GUI_NUM_KEYS];
};

/******************************************************************************
 *
 * FUNCTIONS
 */
extern csg_gui_adapter_t csg_gui_adapter_create(
    csg_gui_adapter_ops_t backend_ops, int width, int height, int flags);
extern void csg_gui_adapter_destroy(csg_gui_adapter_t* adapter);
extern void csg_gui_adapter_update(csg_gui_adapter_t* adapter);
extern void csg_gui_adapter_begin_frame(csg_gui_adapter_t* adapter);
extern void csg_gui_adapter_end_frame(csg_gui_adapter_t* adapter);
