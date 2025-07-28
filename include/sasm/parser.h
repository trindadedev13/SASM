#ifndef __PARSER_H__
#define __PARSER_H__

#include "sasm/token.h"

typedef enum {
  INS_MOV,
  INS_ADD,
  INS_SUB,
  INS_INT,
  INS_JMP,
  INS_CALL,
  INS_RET
} sasm_instruction_type;
typedef enum { INT_SYSCALL } sasm_interrupt_type;
typedef enum {
  SYSCALL_PUTI = 0x01,
  SYSCALL_PUTC = 0x02,
  SYSCALL_EXIT = 0x03
} sasm_syscall_type;

typedef struct {
  sasm_instruction_type type;
  string v1;
  string v2;
  string v3;
} sasm_instruction;

typedef struct sasm_line sasm_line;

struct sasm_line {
  string label;
  sasm_instruction* instruction;
  sasm_line* next;

  size_t line;
  size_t col;
};

sasm_instruction_type sasm_instruction_type_fromstr(string);
sasm_interrupt_type sasm_interrupt_type_fromstr(string);

sasm_line* sasm_line_new(string, sasm_instruction*, size_t, size_t);
void sasm_line_add(sasm_line**, sasm_line*);

typedef struct {
  sasm_token* tokens;
  sasm_line* lines;
  size_t pos;
} sasm_parser;

sasm_parser* sasm_parser_new(sasm_token*);

void sasm_parser_delete(sasm_parser*);

sasm_token* sasm_parser_consume(sasm_parser*, sasm_token_type);
sasm_token* sasm_parser_current(sasm_parser*);
sasm_token* sasm_parser_next(sasm_parser*);
sasm_token* sasm_parser_advance(sasm_parser*);

void sasm_parser_parse(sasm_parser*);

#endif