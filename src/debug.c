#include <stdio.h>

#include "chunk.h"
#include "debug.h"

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->items[offset + 1];
  printf("%-16s %4d \n", name, constant);
  //printValue(chunk->constants.items[constant]);
  //printf("'\n");
  return offset + 2;
}

void disassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);
  for (size_t offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);
  uint8_t instruction = chunk->items[offset];
  switch (instruction) {
  case OP_CONST:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_RET:
    return simpleInstruction("OP_RETURN", offset);
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBTRACT:
    return simpleInstruction("OP_SUBTRACT", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
}

void printValue(Value value) { printf("%d", value); }