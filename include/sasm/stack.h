#ifndef __STACK_H__
#define __STACK_H__

#include "kcstd/types.h"

#define STACK_MAX 8192

typedef struct {
  byte arr[STACK_MAX];
  int top;
} sasm_stack;

sasm_stack* sasm_stack_new();

bool sasm_stack_push(sasm_stack*, byte);

int sasm_stack_pop(sasm_stack*);
int sasm_stack_peek(sasm_stack*);

bool sasm_stack_is_full(sasm_stack*);
bool sasm_stack_is_empty(sasm_stack*);

#endif