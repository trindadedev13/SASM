#ifndef __STACK_H__
#define __STACK_H__

#include "kcstd/types.h"

#define STACK_BASE 0x30C4   // stack start
#define STACK_LIMIT 0x0000  // stack end

/**
 * Initializes Stack.
 */
bool sasm_stack_init();

/**
 * Pushes a new value in stack
 *
 * @param The value(byte) to be pushed
 *
 * Returns true if success, false if error
 */
bool sasm_stack_push(byte);

/**
 * Pops the last pushed value in stack
 *
 * @param The stack structure
 *
 * Returns the popped value, or -1 if error
 */
int sasm_stack_pop();

/**
 * Returns the last pushed value in stack
 * without pop, or if error -1
 *
 * @param The stack structure
 */
int sasm_stack_peek();

/**
 * Returns the size of stack
 */
int sasm_stack_size();

#endif