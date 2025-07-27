#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "sasm/node.h"
#include "sasm/stack.h"

typedef struct {
  sasm_node* raw_nodes;
  sasm_node* labels;
  sasm_stack* stack;
} sasm_interpreter;

sasm_interpreter* sasm_interpreter_new(sasm_node*);

void sasm_interpreter_interpret_node(sasm_node*);

void sasm_interpreter_run(sasm_interpreter*);

#endif