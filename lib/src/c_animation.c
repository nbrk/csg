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

#define ANIMATION_FLAG_COMPLETED 1
#define ANIMATION_FLAG_STOPPED 1 << 1

void csg_animation_update(csg_animation_t* anim, float delta) {
  assert(anim->num_waypoints >= 2);
  assert(anim->current_segment < anim->total_segments);

  // skip everything if 'stopped' or no delta
  if (anim->flags & ANIMATION_FLAG_STOPPED || delta == 0.0f) return;

  // check if we need to restart or run backwards upon completion
  if (anim->flags & ANIMATION_FLAG_COMPLETED) {
    if (anim->mode == CSG_ANIMATION_MODE_RESTART) {
      // restart afresh from the segment zero
      anim->current_segment = 0;
      anim->interpolant = 0.0f;
      anim->flags &= ~ANIMATION_FLAG_COMPLETED;
    }
    // ...
  }

  // process only active, non-completed animations
  if ((anim->flags & ANIMATION_FLAG_COMPLETED) == 0) {
    anim->interpolant += delta;
    anim->interpolant = glm_clamp_zo(anim->interpolant);

    // update the value by lerping inside the active segment
    vec4* from = &anim->waypoints[anim->current_segment];
    vec4* to = &anim->waypoints[anim->current_segment + 1];
    glm_vec4_lerpc(*from, *to, anim->interpolant, anim->current_value);

    // check for end point
    if (anim->interpolant == 1.0f) {
      if (anim->current_segment == anim->total_segments - 1) {
        // end of the whole path
        anim->flags |= ANIMATION_FLAG_COMPLETED;
      } else {
        // end of a segment
        anim->current_segment++;
        anim->interpolant = 0.0f;
        // clear 'completed' as the path may potentially contain new segments
        anim->flags &= ~ANIMATION_FLAG_COMPLETED;
      }
    }
  }
}

csg_animation_t* csg_animation_create(csg_animation_mode_e mode) {
  csg_animation_t* anim = csg_malloc(sizeof(*anim));

  glm_vec4_zero(anim->current_value);
  anim->waypoints = NULL;
  anim->num_waypoints = 0;
  anim->current_segment = 0;
  anim->total_segments = 0;
  anim->interpolant = 0.0f;
  anim->mode = mode;
  anim->flags = 0;

  return anim;
}

float csg_animation_get_value(csg_animation_t* anim, float* x, float* y,
                              float* z, float* w) {
  *x = anim->current_value[0];
  *y = anim->current_value[1];
  *z = anim->current_value[2];
  *w = anim->current_value[3];
  return anim->interpolant;
}

size_t csg_animation_add_waypoint(csg_animation_t* anim, float x, float y,
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
