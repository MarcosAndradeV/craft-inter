#ifndef VALUE_H
#define VALUE_H
#include "common.h"
#include "utils.h"

typedef uint8_t Value;

UTILS_DA_CREATE(Value, Values)

#define initValues(values) do{*(values) = (Values){0};}while(false);
#define writeValues utils_da_append
#define freeValues utils_da_free

#endif // VALUE_H