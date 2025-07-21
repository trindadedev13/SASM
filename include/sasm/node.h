#ifndef __NODE_H__
#define __NODE_H__

#include "kcstd/string.h"

typedef enum { NODE_INSTRUCTION, NODE_INTERRUPT, NODE_LABEL } sasm_node_type;

typedef enum { I_MOV, I_ADD, I_SUB } sasm_instruction_type;

typedef int sasm_intid_t;

#define INT_SYSCALL 0x80  // int 0x10
#define INT_SYSCALL_PUTI 0x01
#define INT_SYSCALL_PUTC 0x02

typedef struct sasm_node sasm_node;

struct sasm_node {
  sasm_node_type type;
  size_t line;
  size_t col;

  struct {
    string name;
    sasm_node* body;
  } label_t;

  struct {
    sasm_instruction_type instype;  // add
    string v1;                      // R
    string v2;                      // B
    string v3;                      // G
  } instruction_t;

  struct {
    sasm_intid_t intid;  // 0x10
  } interrupt_t;

  sasm_node* next;
};

void sasm_node_print(sasm_node*);

sasm_node* sasm_label_node_new(string, size_t, size_t);

sasm_node* sasm_instruction_node_new(sasm_instruction_type,
                                     string,
                                     string,
                                     string,
                                     size_t,
                                     size_t);

sasm_node* sasm_interrupt_node_new(sasm_intid_t, size_t, size_t);

void sasm_node_add(sasm_node**, sasm_node*);

sasm_instruction_type sasm_instruction_opcode_fromstr(string);

sasm_intid_t sasm_intid_fromstr(string);

#endif