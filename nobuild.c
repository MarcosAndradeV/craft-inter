#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-pedantic"

int main(int argc, char **argv) {
  GO_REBUILD_URSELF(argc, argv);

  if (!path_exists("build")) {
    MKDIRS("build");
  }
  CMD("cc", CFLAGS, "-o", "build/crafti", "src/main.c");
  CMD("./build/crafti");

  return 0;
}
