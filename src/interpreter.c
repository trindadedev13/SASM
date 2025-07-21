#include "sasm/interpreter.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/types.h"

#include "sasm/node.h"
#include "sasm/regs.h"

sasm_interpreter* sasm_interpreter_new(sasm_node* nodes) {
  sasm_interpreter* it = memory_alloc(sizeof(sasm_interpreter));
  it->raw_nodes = nodes;
  it->labels = null;
  {
    sasm_node* current = nodes;
    while (current != null) {
      if (current->type == NODE_LABEL) {
        sasm_node_add(&it->labels, current);
      }
      current = current->next;
    }
  }

  return it;
}

void sasm_interpreter_run(sasm_interpreter* self) {
  sasm_node* current_node = self->raw_nodes;
  while (current_node != null) {
    sasm_interpreter_interpret_node(current_node);
    current_node = current_node->next;
  }
}

void sasm_interpreter_interpret_node(sasm_node* node) {
  if (node == null)
    return;
  switch (node->type) {
    case NODE_INSTRUCTION: {
      switch (node->instruction_t.instype) {
        case I_MOV: {
          string v1 = node->instruction_t.v1;
          string v2 = node->instruction_t.v2;
          if (v1 == null || v2 == null) {
            printf("Error[%d:%d]: Mov Instruction Syntax: MOV [DEST], [VALUE]",
                   node->line, node->col);
            exit(1);
          }
          // printf("MOV %s, %s\n", v1, v2);
          sasm_regs_mov(v1, v2);
          break;
        }
        case I_ADD: {
          string v1 = node->instruction_t.v1;
          string v2 = node->instruction_t.v2;
          if (v1 == null || v2 == null) {
            /**
             * ADD A, B     = a = a + b
             * ADD A, B, C  = a = b + c
             */
            printf(
                "Error[%d:%d]: Add Instruction Syntax: ADD [DEST], [V2], [V3]",
                node->line, node->col);
            exit(1);
          }
          string v3 = node->instruction_t.v3;
          sasm_regs_add(v1, v2, v3);
          break;
        }
        case I_SUB: {
          string v1 = node->instruction_t.v1;
          string v2 = node->instruction_t.v2;
          if (v1 == null || v2 == null) {
            /**
             * SUB A, B     = a = a + b
             * SUB A, B, C  = a = b + c
             */
            printf(
                "Error[%d:%d]: Add Instruction Syntax: ADD [DEST], [V2], [V3]",
                node->line, node->col);
            exit(1);
          }
          string v3 = node->instruction_t.v3;
          sasm_regs_sub(v1, v2, v3);
          break;
        }
      }
      break;
    }
    case NODE_INTERRUPT: {
      switch (node->interrupt_t.intid) {
        case INT_SYSCALL:
          // printf("{\n\tsyscall = %c\n\tch = %c\n}\n", ch, syscall);
          switch (sasm_regs_getD()) {
            case INT_SYSCALL_PUTC:
              sasm_regs_setA(put_char(sasm_regs_getA()));
              break;
            case INT_SYSCALL_PUTI:
              print_int(sasm_regs_getA());
          }
          break;
      }
      break;
    }
    case NODE_LABEL: {
      sasm_node* n = node->label_t.body;
      while (n != null) {
        sasm_interpreter_interpret_node(n);
        n = n->next;
      }
      break;
    }
  };
}