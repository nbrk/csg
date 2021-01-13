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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#undef malloc

static bool debug = 0;

struct memchunk {
  void* mem;
  size_t size;
  LIST_ENTRY(memchunk)
  entries;
};

LIST_HEAD(listhead, memchunk);
static struct listhead allocations = LIST_HEAD_INITIALIZER(allocations);

void csg_set_malloc_debug(bool val) { debug = val; }

void* csg_malloc(size_t size) {
  struct memchunk* chunk = malloc(sizeof(*chunk));

  chunk->mem = malloc(size);

  chunk->size = size;
  LIST_INSERT_HEAD(&allocations, chunk, entries);

  /*
   * Zero out the memory chunk
   */
  bzero(chunk->mem, chunk->size);

  if (debug)
    printf("csg_malloc: allocation @%p (%zu bytes)\n", chunk, chunk->size);

  return chunk->mem;
}

void* csg_realloc(void* mem, size_t size) {
  struct memchunk* chunk;
  LIST_FOREACH(chunk, &allocations, entries) {
    if (chunk->mem == mem) {
      size_t oldsize = chunk->size;

      chunk->mem = realloc(chunk->mem, size);
      chunk->size = size;

      if (debug)
        printf("csg_realloc: @%p (%zu -> %zu bytes)\n", chunk, oldsize,
               chunk->size);

      return chunk->mem;
    }
  }

  // chunk that contains 'mem' not found; malloc
  return csg_malloc(size);
}

void csg_free(void* mem) {
  //  if (LIST_EMPTY(&allocations)) return;  // XXX

  struct memchunk* chunk;
  LIST_FOREACH(chunk, &allocations, entries) {
    if (chunk->mem == mem) {
      free(chunk->mem);
      LIST_REMOVE(chunk, entries);

      if (debug) printf("csg_free: free @%p (%zu bytes)\n", chunk, chunk->size);

      return;
    }
  }
  // not found; XXX not ours, so do not free() it
  return;
}

void csg_print_malloc_stat(void) {
  struct memchunk* chunk;
  size_t total = 0;

  printf("Active allocations:\n");

  if (LIST_EMPTY(&allocations)) {
    printf("No allocations yet.\n");
    return;
  }

  LIST_FOREACH(chunk, &allocations, entries) {
    printf("Chunk @%p (%zu bytes)\n", chunk, chunk->size);
    total += chunk->size;
  }
  printf("Total memory used: %lu bytes\n", total);
}
