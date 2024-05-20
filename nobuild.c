#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-pedantic"

int main(int argc, char **argv) {
  GO_REBUILD_URSELF(argc, argv);

  if (!path_exists("build")) {
    MKDIRS("build");
  }
  //if (!path_exists("bin")) {
  //  MKDIRS("bin");
  //}
  //FOREACH_FILE_IN_DIR(file, "src", {
  //  if (ENDS_WITH(file, ".c"))
  //  CMD("cc", CFLAGS, "-o", CONCAT("bin/",NOEXT(file), ".o"), CONCAT("src/", file), "-c", "-Iinclude");
  //});

  //CMD("cc", CFLAGS, "-o", "build/crafti", "bin/main.o", "bin/chunk.o", "bin/debug.o", "bin/vm.o");
  CMD("cc",  "-o", "build/crafti", "src/main.c", "src/debug.c", "src/vm.c", "src/compiler.c", "-Iinclude");
  //CMD("./build/crafti");

  return 0;
}
