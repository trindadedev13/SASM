#include "sasm/interpreter.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/types.h"

#include "sasm/cpu.h"
#include "sasm/parser.h"
#include "sasm/regs.h"
#include "sasm/stack.h"

sasm_interpreter* sasm_interpreter_new(sasm_line* lines) {
  sasm_interpreter* it = memory_alloc(sizeof(sasm_interpreter));
  it->raw_lines = lines;
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
      /**
       * mov instruction
       * MOV A, B
       *
       * it asigns arg2(v2) value to arg1(v1)
       */
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
      /**
       * add instruction
       * ADD A, B     = a = a + b
       * ADD A, B, C  = a = b + c
       *
       * It sums an number by an number
       * see the signature above
       */
      string v1 = line->instruction->v1;
      string v2 = line->instruction->v2;
      if (v1 == null || v2 == null) {
        printf("Error[%d:%d]: ADD Instruction Syntax: ADD [DEST], [V2], [V3]\n",
               line->line, line->col);
        return null;
      }
      string v3 = line->instruction->v3;
      sasm_regs_add(v1, v2, v3);
      return line->next;
    }

    case INS_SUB: {
      /**
       * sub instruction
       * SUB A, B     = a = a - b
       * SUB A, B, C  = a = b - c
       *
       * It subtracts an number by an number
       * see the signature above
       */
      string v1 = line->instruction->v1;
      string v2 = line->instruction->v2;
      if (v1 == null || v2 == null) {
        printf("Error[%d:%d]: SUB Instruction Syntax: SUB [DEST], [V2], [V3]\n",
               line->line, line->col);
        return null;
      }
      string v3 = line->instruction->v3;
      sasm_regs_sub(v1, v2, v3);
      return line->next;
    }

    case INS_INT: {
      /**
       * interruption
       * INT [id]
       *
       * well, its for make Simulated CPU do tasks
       */
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
        /** do the interrupt */
        case INT_SYSCALL: {
          /** interpret the syscall interrupt */
          switch (cpu->regs->D) {
            case SYSCALL_PUTI:
              /**
               * lmao idk why print_int dont return bytes like put_char
               * TODO: do what i said above
               */
              print_int(cpu->regs->A);
              break;
            case SYSCALL_PUTC:
              /**
               * print char in stdout
               * return the bytes written in A register
               */
              cpu->regs->A = put_char(cpu->regs->A);
              break;
            case SYSCALL_EXIT:
              /** just exit */
              exit(cpu->regs->A);
              break;
          }
          break;
        }
      };
      return line->next;
    }

    case INS_JMP: {
      /**
       * jump to label
       * JMP [label]
       *
       * it gets the label start line
       * and continue the flow from it
       */
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
      /**
       * call label (and return)
       * CALL [label]
       *
       * it does the same as JMP instruction
       * but it stores the return value in stack
       * to RET instruction, return to caller.
       */
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
        sasm_stack_push((ptr >> (i * 8)) & 0xFF);
      }
      return lab_line;
    }

    case INS_RET: {
      /**
       * return to label
       * RET
       *
       * it return to label caller
       * should be used with CALL
       * pops the return line pointer bytes of stack
       * aquiles trindade
       */

      /** If theres no pointer in the stack, so warning user. */
      if (sasm_stack_size() < sizeof(uintptr_t)) {
        printf("Warning[%d:%d]: RET without CALL.\n", line->line, line->col);
        return line->next;
      }
      uintptr_t ret_addr = 0;
      for (int i = sizeof(uintptr_t) - 1; i >= 0; i--) {
        ret_addr <<= 8;
        ret_addr |= sasm_stack_pop();
      }
      sasm_line* return_point_uns = (sasm_line*)ret_addr;

      /**
       * If popped pointer is null, so just continue the flow
       */
      sasm_line* return_point =
          (return_point_uns != null) ? return_point_uns : line->next;
      return return_point;
    }

    default: {
      printf("Error[%d:%d]: Unknown instruction.\n", line->line, line->col);
      return null;
    }
  }
  return null;
}