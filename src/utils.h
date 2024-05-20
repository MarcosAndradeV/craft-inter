#ifndef UTILS_H
#define UTILS_H
#include <assert.h>
#include <stdlib.h>

#define UTILS_DA_CREATE(dtype, name)                                           \
  typedef struct {                                                             \
    dtype *items;                                                              \
    size_t count;                                                              \
    size_t capacity;                                                           \
  } name;

// TODO: Test that
#define FOREACH_ARRAY(type, elem, array, body)  \
    for (size_t elem_##index = 0;                           \
         elem_##index < array.count;                        \
         ++elem_##index)                                    \
    {                                                       \
        type *elem = &array.elems[elem_##index];            \
        body;                                               \
    }

#define utils_da_append(xs, x)                                                 \
  do {                                                                         \
    if ((xs)->count >= (xs)->capacity) {                                       \
      if ((xs)->capacity == 0)                                                 \
        (xs)->capacity = 256;                                                  \
      else                                                                     \
        (xs)->capacity *= 2;                                                   \
      (xs)->items =                                                            \
          realloc((xs)->items, (xs)->capacity * sizeof(*(xs)->items));         \
    }                                                                          \
    (xs)->items[(xs)->count++] = (x);                                          \
  } while (0)

#define utils_da_append_many(da, new_items, new_items_count)                   \
  do {                                                                         \
    if ((da)->count + new_items_count > (da)->capacity) {                      \
      if ((da)->capacity == 0) {                                               \
        (da)->capacity = 256;                                                  \
      }                                                                        \
      while ((da)->count + new_items_count > (da)->capacity) {                 \
        (da)->capacity *= 2;                                                   \
      }                                                                        \
      (da)->items =                                                            \
          realloc((da)->items, (da)->capacity * sizeof(*(da)->items));         \
      assert((da)->items != NULL && "_");                                      \
    }                                                                          \
    memcpy((da)->items + (da)->count, new_items,                               \
           new_items_count * sizeof(*(da)->items));                            \
    (da)->count += new_items_count;                                            \
  } while (0)

#define utils_da_free(xs)                                                      \
  do {                                                                         \
    free((xs)->items);                                                         \
  } while (0)

typedef struct {
  char *items;
  size_t count;
  size_t capacity;
} Utils_String_Builder;

// FUNCTIONS
char *shift_args(int *argc, char ***argv);

#ifdef UTILS_IMPLEMENTATION

char *shift_args(int *argc, char ***argv) {
  assert(*argc > 0);
  char *result = **argv;
  *argc -= 1;
  *argv += 1;
  return result;
}

#endif // UTILS_IMPLEMENTATION
#endif // UTILS_H
