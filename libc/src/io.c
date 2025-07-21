#include "kcstd/io.h"

#include "kcstd/memory.h"
#include "kcstd/string.h"
#include "kcstd/types.h"

file std_files[] = {
    (file){.fd = 0, .filename = "__builtin__stdin__"},   // stdin
    (file){.fd = 1, .filename = "__builtin__stdout__"},  // stdout
    (file){.fd = 2, .filename = "__builtin__stderr__"}   // stderr
};

ssize_t write(int fd, const void* buf, size_t len) {
  return __ASM_WRITE__(fd, buf, len);
}

ssize_t read(int fd, void* dest, size_t count) {
  return __ASM_READ__(fd, dest, count);
}

void exit(int exit_code) {
  __ASM_EXIT__((long)exit_code);
}