#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "utils.h"
#include "value.h"

typedef enum {
  OP_RET,
  OP_CONST,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
} Opcode;

typedef struct {
  UTILS_DA_CREATE_FIELDS(uint8_t)
  Values constants;
} Chunk;

#define initChunk(chunk)                                                       \
  do {                                                                         \
    *(chunk) = (Chunk){0};                                                     \
    initValues(&(chunk)->constants);                                           \
  } while (false);

#define writeChunk utils_da_append

#define freeChunk(chunk)                                                       \
  do {                                                                         \
    utils_da_free((chunk));                                                    \
    freeValues(&(chunk)->constants);                                           \
  } while (false)

int addConstant(Chunk* chunk, Value value);

#ifdef CHUNK_IMPL

int addConstant(Chunk* chunk, Value value){
  writeValues(&chunk->constants, value);
  return chunk->constants.count-1;
}

#endif

#endif // CHUNK_H