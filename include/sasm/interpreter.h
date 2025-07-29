#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "sasm/parser.h"
#include "sasm/stack.h"

typedef struct {
  sasm_line* raw_lines;
  sasm_stack* stack;
} sasm_interpreter;

/**
 * Returns a new structure of SASM Interpreter.
 *
 * @param The lines to be interpreted.
 *
 * FIXME!!: Add Delete function to clear memory
 */
sasm_interpreter* sasm_interpreter_new(sasm_line*);

/**
 * Returns the first instruction after label start
 *
 * @param The interpreter structure.
 * @param The label name to be found.
 *
 * Example:
 *
 * Exit:
 *   MOV A, 0x00
 *   ADD A, 0x01
 *
 * It will return the MOV instruction line.
 */
sasm_line* sasm_interpreter_find_label_start(sasm_interpreter*, string);

/**
 * Interpret an SASM Line.
 *
 * @param The interpreter structure.
 * @param The line structure.
 *
 * Returns the next line to be interpreted.
 */
sasm_line* sasm_interpreter_interpret_line(sasm_interpreter*, sasm_line*);

/**
 * Interpret all lines given by sasm_interpreter_new
 */
void sasm_interpreter_run(sasm_interpreter*);

#endif