#include "sasm/cpu.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"

#include "sasm/memory.h"
#include "sasm/regs.h"
#include "sasm/stack.h"

sasm_cpu* cpu = null;

bool sasm_cpu_init() {
  cpu = memory_alloc(sizeof(sasm_cpu));
  if (cpu == null) {
    printf("[Error]: Failed to initialize CPU\n");
    return false;
  }

  cpu->regs = sasm_regs_new();
  if (cpu->regs == null) {
    printf("[Error]: Failed to initialize Registers \n");
    return false;
  }

  cpu->memory = sasm_memory_new();
  if (cpu->memory == null) {
    printf("[Error]: Failed to initialize Memory\n");
    return false;
  }

  if (!sasm_stack_init()) {
    printf("[Error]: Failed to initialize Stack\n");
    return false;
  }

  return true;
}