#ifndef __STACK_H__
#define __STACK_H__

#include "kcstd/types.h"

#define STACK_MAX 8192

typedef struct {
  byte arr[STACK_MAX];
  int top;
} sasm_stack;

/**
 * Returns new SASM_Stack structure
 *
 * FIXME!!: Add Delete Stack function to clear memory
 */
sasm_stack* sasm_stack_new();

/**
 * Pushes a new value in stack
 *
 * @param The stack structure
 * @param The value(byte) to be pushed
 *
 * Returns true if success, false if error
 */
bool sasm_stack_push(sasm_stack*, byte);

/**
 * Pops the last pushed value in stack
 *
 * @param The stack structure
 *
 * Returns the popped value, or -1 if error
 */
int sasm_stack_pop(sasm_stack*);

/**
 * Returns the last pushed value in stack
 * without pop, or if error -1
 *
 * @param The stack structure
 */
int sasm_stack_peek(sasm_stack*);

/**
 * Returns if stack if full
 *
 * @param The stack structure
 */
bool sasm_stack_is_full(sasm_stack*);

/**
 * Returns if stack if empty
 *
 * @param The stack structure
 */
bool sasm_stack_is_empty(sasm_stack*);

#endif