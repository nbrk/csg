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


/**
 * @brief Opaque handle to the scenegraph node tree. A node contains optional
 * transformation and drawable components, as well as pointer to a user data.
 */
typedef struct csg_node_t csg_node_t;
/**
 * @brief Opaque handle to the subtree's model-space transformation component
 */
typedef struct csg_transform_t csg_transform_t;
/**
 * @brief Opaque handle to the node's drawable component (which is optional)
 */
typedef struct csg_drawable_t csg_drawable_t;
/**
 * @brief Opaque handle to the drawable's geometry data
 */
typedef struct csg_geometry_t csg_geometry_t;
/**
 * @brief Opaque handle to the drawable's material abstraction
 */
typedef struct csg_material_t csg_material_t;
/**
 * @brief Opaque handle to the viewport
 */
typedef struct csg_viewport_t csg_viewport_t;
/**
 * @brief Opaque handle to the camera
 */
typedef struct csg_camera_t csg_camera_t;
/**
 * @brief Opaque handle to animation used in transforms
 */
typedef struct csg_animation_t csg_animation_t;

typedef enum {
  CSG_MATERIAL_COLOR_DIFFUSE,
} csg_material_color_e;

typedef enum {
  CSG_CAMERA_PROJECTION_ORTHOGRAPHIC,
  CSG_CAMERA_PROJECTION_PERSPECTIVE,
} csg_camera_projection_e;

typedef enum {
  CSG_ANIMATION_MODE_ONESHOT,
  CSG_ANIMATION_MODE_RESTART,
  CSG_ANIMATION_MODE_OSCILLATE
} csg_animation_mode_e;
