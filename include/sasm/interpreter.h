#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "sasm/parser.h"
#include "sasm/stack.h"

typedef struct {
  sasm_line* raw_lines;
  sasm_stack* stack;
} sasm_interpreter;

sasm_interpreter* sasm_interpreter_new(sasm_line*);

sasm_line* sasm_interpreter_find_label_start(sasm_interpreter*, string);

sasm_line* sasm_interpreter_interpret_line(sasm_interpreter*, sasm_line*);

void sasm_interpreter_run(sasm_interpreter*);

#endif