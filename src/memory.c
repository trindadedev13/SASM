#include "sasm/memory.h"

#include "kcstd/memory.h"
#include "kcstd/types.h"

sasm_memory* sasm_memory_new() {
  sasm_memory* it = memory_alloc(sizeof(sasm_memory));
  return it;
}

void sasm_memory_delete(sasm_memory* self) {
  memory_free(self);
}