#ifndef __CPU_H__
#define __CPU_H__

#include "kcstd/types.h"

#include "sasm/memory.h"
#include "sasm/regs.h"

typedef struct {
  sasm_memory* memory;
  sasm_regs* regs;
} sasm_cpu;

/** Global Simulated CPU structure */
extern sasm_cpu* cpu;

/**
 * Initializes global simulated CPU.
 */
bool sasm_cpu_init();

#endif