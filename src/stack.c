#include "sasm/stack.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/types.h"

#include "sasm/cpu.h"

bool sasm_stack_init() {
  cpu->regs->SP = STACK_BASE;
  return true;
}

bool sasm_stack_push(byte val) {
  if (cpu->regs->SP <= STACK_LIMIT) {
    printf("Error: SASM Stack Overflow.\n");
    return false;
  }

  cpu->regs->SP -= 1;
  cpu->memory->data[cpu->regs->SP] = val;
  return true;
}

int sasm_stack_pop() {
  if (cpu->regs->SP >= STACK_BASE) {
    printf("Error: SASM Stack Underflow.\n");
    return -1;
  }
  return cpu->memory->data[cpu->regs->SP++];
}

int sasm_stack_peek() {
  if (cpu->regs->SP >= STACK_BASE)
    return -1;

  return cpu->memory->data[cpu->regs->SP];
}

int sasm_stack_size() {
  return STACK_BASE - cpu->regs->SP;
}