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
        printf("Error[%d:%d]: MOV Instruction Syntax: MOV [DEST], [VALUE]\n",
               line->line, line->col);
        return null;
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
        printf("Error[%d:%d]: ADD Instruction Syntax: ADD [DEST], [V2], [V3]\n",
               line->line, line->col);
        return null;
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
        printf("Error[%d:%d]: SUB Instruction Syntax: ADD [DEST], [V2], [V3]\n",
               line->line, line->col);
        return null;
      }
      string v3 = line->instruction->v3;
      sasm_regs_sub(v1, v2, v3);
      return line->next;
    }

    case INS_INT: {
      string v1 = line->instruction->v1;
      if (v1 == null) {
        printf("Error[%d:%d]: INT Instruction Syntax: INT [ID]\n", line->line,
               line->col);
        return null;
      }
      sasm_interrupt_type type = sasm_interrupt_type_fromstr(v1);
      if (type == -1) {
        printf("Error[%d:%d]: Unknown interrupt: %s\n", line->line, line->col,
               v1);
        return null;
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
        printf("Error[%d:%d]: JMP Instruction Syntax: JMP [label]\n",
               line->line, line->col);
        return null;
      }
      sasm_line* lab_line = sasm_interpreter_find_label_start(self, v1);
      if (lab_line == null) {
        printf("Error[%d:%d]: Unknown label: %s\n", line->line, line->col, v1);
        return null;
      }
      return lab_line;
    }

    case INS_CALL: {
      string v1 = line->instruction->v1;
      if (v1 == null) {
        printf("Error[%d:%d]: CALL Instruction Syntax: CALL [label]\n",
               line->line, line->col);
        return null;
      }
      sasm_line* lab_line = sasm_interpreter_find_label_start(self, v1);
      if (lab_line == null) {
        printf("Error[%d:%d]: Unknown label: %s\n", line->line, line->col, v1);
        return null;
      }
      /**
       * push next line pointer in stack
       * push byte per byte
       * in 32 bits it will be 4 bytes, so 4 itens in stack
       * aquiles trindade
       */
      uintptr_t ptr = (uintptr_t)(line->next);
      for (int i = 0; i < sizeof(uintptr_t); i++) {
        sasm_stack_push(self->stack, (ptr >> (i * 8)) & 0xFF);
      }
      return lab_line;
    }

    case INS_RET: {
      /**
       * pops the return line pointer bytes of stack
       * FIXME!!: if its not called (executed by the default program flow) it
       * gives errors. aquiles trindade
       */
      uintptr_t ret_addr = 0;
      for (int i = sizeof(uintptr_t) - 1; i >= 0; i--) {
        ret_addr <<= 8;
        ret_addr |= sasm_stack_pop(self->stack);
      }
      sasm_line* return_point = (sasm_line*)ret_addr;
      return return_point;
    }

    default: {
      printf("Error[%d:%d]: Unknown instruction.\n", line->line, line->col);
      return null;
    }
  }
  return null;
}