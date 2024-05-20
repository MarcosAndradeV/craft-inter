#define UTILS_IMPLEMENTATION
#include "utils.h"
#include "debug.h"
#include "vm.h"
#include "stdio.h"

static void repl() {
  char line[1024];
  for (;;) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
        break;
    }
    interpret(line);
  }
}

static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
    if (file == NULL) {
      fprintf(stderr, "Could not open file \"%s\".\n", path);
      exit(4);
    }
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
      fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
      exit(4);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
      fprintf(stderr, "Could not read file \"%s\".\n", path);
      exit(4);
    }
    buffer[bytesRead] = '\0';
  fclose(file);
  return buffer;
}

static void runFile(const char* path) {
  char* source = readFile(path);
  InterpretResult result = interpret(source);
  free(source);
  if (result == INTERPRET_COMPILE_ERROR) exit(12);
  if (result == INTERPRET_RUNTIME_ERROR) exit(21); // i dont know
}

int main(int argc, char **argv) {
  char* program = shift_args(&argc, &argv);
  initVM();
  switch(argc){
    case 0: {
      repl();
      break;
    }
    case 1: {
      runFile(shift_args(&argc, &argv));
      break; 
    } 
    default: {
      fprintf(stderr, "Usage: %s [path]\n", program);
      exit(1);
      break;
    }
  }
  freeVM();
  return 0;
}
