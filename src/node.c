#include "sasm/node.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/string.h"

void sasm_node_print(sasm_node* n) {
  switch (n->type) {
    case NODE_INSTRUCTION:
      printf("Instruction{\n\ttype = %d\n\tv1 = %s\n\tv2 = %s\n\tv3 = %s\n}\n",
             n->instruction_t.instype, n->instruction_t.v1, n->instruction_t.v2,
             n->instruction_t.v3);
      break;
    case NODE_INTERRUPT:
      printf("Interrupt{\n\ttype = %d\n}\n", n->interrupt_t.intid);
      break;
    case NODE_LABEL:
      printf("Label{\n\tname = %s\n}\n", n->label_t.name);
      sasm_node* bnode = n->label_t.body;
      while (bnode != null) {
        sasm_node_print(bnode);
        bnode = bnode->next;
      }
      break;
  };
}

sasm_node* sasm_instruction_node_new(sasm_instruction_type instype,
                                     string v1,
                                     string v2,
                                     string v3,
                                     size_t line,
                                     size_t col) {
  sasm_node* node = memory_alloc(sizeof(sasm_node));
  node->type = NODE_INSTRUCTION;
  node->next = null;
  node->line = line;
  node->col = col;
  node->instruction_t.instype = instype;
  node->instruction_t.v1 = v1;
  node->instruction_t.v2 = v2;
  node->instruction_t.v3 = v3;
  return node;
}

sasm_node* sasm_interrupt_node_new(sasm_intid_t id, size_t line, size_t col) {
  sasm_node* node = memory_alloc(sizeof(sasm_node));
  node->type = NODE_INTERRUPT;
  node->next = null;
  node->line = line;
  node->col = col;
  node->interrupt_t.intid = id;
  return node;
}

sasm_node* sasm_label_node_new(string name, size_t line, size_t col) {
  sasm_node* node = memory_alloc(sizeof(sasm_node));
  node->type = NODE_LABEL;
  node->next = null;
  node->line = line;
  node->col = col;
  node->label_t.name = name;
  return node;
}

void sasm_node_add(sasm_node** head, sasm_node* new_node) {
  if (*head == null) {
    *head = new_node;
  } else {
    sasm_node* current = *head;
    while (current->next)
      current = current->next;
    current->next = new_node;
  }
}

sasm_instruction_type sasm_instruction_opcode_fromstr(string op) {
  if (str_equals(op, "MOV")) {
    return I_MOV;
  } else if (str_equals(op, "ADD")) {
    return I_ADD;
  } else if (str_equals(op, "SUB")) {
    return I_SUB;
  }
  return -1;
}

sasm_intid_t sasm_intid_fromstr(string id) {
  if (str_equals(id, "0x80")) {
    return INT_SYSCALL;
  }
  return -1;
}