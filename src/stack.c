#include "sasm/stack.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/types.h"

sasm_stack* sasm_stack_new() {
  sasm_stack* it = memory_alloc(sizeof(sasm_stack));
  memory_set(it->arr, 0, STACK_MAX);
  it->top = -1;
  return it;
}

bool sasm_stack_push(sasm_stack* self, byte val) {
  if (self == null)
    return false;
  if (sasm_stack_is_full(self)) {
    printf("Error: SASM Stack Overflow.\n");
    return false;
  }
  self->arr[++self->top] = val;
  return true;
}

int sasm_stack_pop(sasm_stack* self) {
  if (self == null || sasm_stack_is_empty(self)) {
    printf("Error: SASM Stack Underflow.\n");
    return -1;
  }
  int a = self->arr[self->top--];
  return a;
}

int sasm_stack_peek(sasm_stack* self) {
  if (self == null)
    return -1;
  return self->arr[self->top];
}

bool sasm_stack_is_full(sasm_stack* self) {
  if (self == null)
    return true;
  return self->top >= STACK_MAX - 1;
}

bool sasm_stack_is_empty(sasm_stack* self) {
  if (self == null)
    return true;
  return self->top == -1;
}