#include "sasm/interpreter.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/types.h"

#include "sasm/parser.h"
#include "sasm/regs.h"
#include "sasm/stack.h"

sasm_interpreter* sasm_interpreter_new(sasm_line* lines) {
  sasm_interpreter* it = memory_alloc(sizeof(sasm_interpreter));
  it->raw_lines = lines;
  it->stack = sasm_stack_new();
  return it;
}

void sasm_interpreter_run(sasm_interpreter* self) {
  sasm_line* line = self->raw_lines;
  while (line != null) {
    line = sasm_interpreter_interpret_line(self, line);
  }
}

sasm_line* sasm_interpreter_find_label_start(sasm_interpreter* self,
                                             string name) {
  if (self == null) {
    printf("Error: Interpreter is null.");
    return null;
  }

  sasm_line* c = self->raw_lines;
  while (c != null) {
    if (c->label != null && str_equals(c->label, name))
      return c;
    c = c->next;
  }
  return null;
}

sasm_line* sasm_interpreter_interpret_line(sasm_interpreter* self,
                                           sasm_line* line) {
  if (line == null)
    return null;

  switch (line->instruction->type) {
    case INS_MOV: {
      string v1 = line->instruction->v1;
      string v2 = line->instruction->v2;
      if (v1 == null || v2 == null) {
        printf("Error[%d:%d]: MOV Instruction Syntax: MOV [DEST], [VALUE]",
               line->line, line->col);
        exit(1);
      }
      sasm_regs_mov(v1, v2);
      return line->next;
    }

    case INS_ADD: {
      string v1 = line->instruction->v1;
      string v2 = line->instruction->v2;
      if (v1 == null || v2 == null) {
        /**
         * ADD A, B     = a = a + b
         * ADD A, B, C  = a = b + c
         */
        printf("Error[%d:%d]: ADD Instruction Syntax: ADD [DEST], [V2], [V3]",
               line->line, line->col);
        exit(1);
      }
      string v3 = line->instruction->v3;
      sasm_regs_add(v1, v2, v3);
      return line->next;
    }

    case INS_SUB: {
      string v1 = line->instruction->v1;
      string v2 = line->instruction->v2;
      if (v1 == null || v2 == null) {
        /**
         * SUB A, B     = a = a + b
         * SUB A, B, C  = a = b + c
         */
        printf("Error[%d:%d]: SUB Instruction Syntax: ADD [DEST], [V2], [V3]",
               line->line, line->col);
        exit(1);
      }
      string v3 = line->instruction->v3;
      sasm_regs_sub(v1, v2, v3);
      return line->next;
    }

    case INS_INT: {
      string v1 = line->instruction->v1;
      if (v1 == null) {
        printf("Error[%d:%d]: INT Instruction Syntax: INT [ID]", line->line,
               line->col);
        exit(1);
      }
      sasm_interrupt_type type = sasm_interrupt_type_fromstr(v1);
      if (type == -1) {
        printf("Error[%d:%d]: Unknown interrupt: %s", line->line, line->col,
               v1);
        exit(1);
      }
      switch (type) {
        case INT_SYSCALL: {
          switch (sasm_regs_getD()) {
            case SYSCALL_PUTI:
              print_int(sasm_regs_getA());
              break;
            case SYSCALL_PUTC:
              sasm_regs_setA(put_char(sasm_regs_getA()));
              break;
            case SYSCALL_EXIT:
              exit(sasm_regs_getA());
              break;
          }
          break;
        }
      };
      return line->next;
    }

    case INS_JMP: {
      string v1 = line->instruction->v1;
      if (v1 == null) {
        printf("Error[%d:%d]: JMP Instruction Syntax: JMP [label]", line->line,
               line->col);
        exit(1);
      }
      sasm_line* line = sasm_interpreter_find_label_start(self, v1);
      return line;
    }

    default: {
      printf("Error[%d:%d]: Unknown instruction.\n", line->line, line->col);
      exit(1);
      break;
    }
  }
}