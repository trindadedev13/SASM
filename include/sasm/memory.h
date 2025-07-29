#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "kcstd/types.h"

#define MEMORY_SIZE 12500

typedef struct {
  byte data[MEMORY_SIZE];
} sasm_memory;

sasm_memory* sasm_memory_new();

void sasm_memory_delete(sasm_memory*);

#endif