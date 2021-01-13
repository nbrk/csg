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

#include "csg_internal.h"

static void animation_update_in(csg_animation_t* anim, int segment,
                                float delta) {
  assert(segment >= 0 && segment <= anim->num_waypoints - 1);

  anim->interpolant += delta;
  anim->interpolant = glm_clamp_zo(anim->interpolant);

  vec4* from = &anim->waypoints[segment];
  vec4* to = &anim->waypoints[segment + 1];

  glm_vec4_lerpc(*from, *to, anim->interpolant, anim->current_value);
}

void csg_animation_update(csg_animation_t* anim, float delta) {
  assert(anim->num_waypoints >= 2);
  if (delta == 0.0f) return;

  if (anim->completed == true) {
    if (delta < 0) anim->completed = false;
    return;
  }

  animation_update_in(anim, anim->current_segment, delta);
  if (anim->interpolant == 1.0f) {
    if (anim->current_segment < anim->total_segments - 1) {
      anim->current_segment++;
      anim->interpolant = 0.0f;  // reset to the beginning of the next segment
    } else
      anim->completed = true;
  } else if (anim->interpolant == 0.0f) {
    if (anim->current_segment > 0) {
      anim->current_segment--;
      anim->interpolant = 1.0f;  // reset to the end of the previous segment
      return;
    }
  }
}

// float csg_animation_update(csg_animation_t* anim, float delta) {
//  assert(anim->num_waypoints >= 2);
//  if (delta == 0.0f) return anim->interpolant;

//  // determine path segment to lerp inside
//  if (anim->interpolant == 1.0f &&
//      anim->current_path_segment >= anim->num_waypoints - 1)
//    return 1.0f;  // real end of the path

//  vec4* from = &anim->waypoints[anim->current_path_segment];
//  vec4* to = &anim->waypoints[anim->current_path_segment + 1];
//  anim->interpolant += delta;

//  // normalize resulting interpolant
//  if (anim->interpolant > 1.0f) anim->interpolant = 1.0f;
//  if (anim->interpolant < 0.0f) anim->interpolant = 0.0f;

//  glm_vec4_lerpc(*from, *to, anim->interpolant, anim->current_value);

//  // maybe move to next/prev path segments
//  if (anim->interpolant == 1.0f &&
//      anim->current_path_segment < anim->num_waypoints - 1)
//    anim->current_path_segment++;
//  if (anim->interpolant == 0.0f && anim->current_path_segment > 0)
//    anim->current_path_segment--;

//  return anim->interpolant;
//}

csg_animation_t* csg_animation_create(void) {
  csg_animation_t* anim = csg_malloc(sizeof(*anim));

  glm_vec4_zero(anim->current_value);
  anim->waypoints = NULL;
  anim->num_waypoints = 0;
  anim->current_segment = 0;
  anim->total_segments = 0;
  anim->interpolant = 0.0f;
  anim->completed = false;

  return anim;
}

float csg_animation_get(csg_animation_t* anim, float* x, float* y, float* z,
                        float* w) {
  *x = anim->current_value[0];
  *y = anim->current_value[1];
  *z = anim->current_value[2];
  *w = anim->current_value[3];
  return anim->interpolant;
}

size_t csg_animation_waypoint_add(csg_animation_t* anim, float x, float y,
                                  float z, float w) {
  size_t idx = anim->num_waypoints;
  anim->num_waypoints++;
  anim->waypoints =
      csg_realloc(anim->waypoints, anim->num_waypoints * sizeof(float) * 4);

  anim->total_segments = anim->num_waypoints - 1;
  anim->waypoints[idx][0] = x;
  anim->waypoints[idx][1] = y;
  anim->waypoints[idx][2] = z;
  anim->waypoints[idx][3] = w;

  return idx;
}
