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

/**
 * Returns the instruction type enum value based on string
 *
 * @param The string
 */
sasm_instruction_type sasm_instruction_type_fromstr(string);

/**
 * Returns the interrupt type enum value based on string
 *
 * @param The string
 */
sasm_interrupt_type sasm_interrupt_type_fromstr(string);

/**
 * Returns a new SASM Line
 *
 * @param The label
 * @param The struction of line
 * @param The line(number)
 * @param The column(number)
 *
 * FIXME!!: Add Delete SASM Line function to clear memory
 */
sasm_line* sasm_line_new(string, sasm_instruction*, size_t, size_t);

/**
 * Adds a new lines to a linked array of lines
 *
 * @param The line linked array
 * @param The line to be added
 */
void sasm_line_add(sasm_line**, sasm_line*);

typedef struct {
  sasm_token* tokens;
  sasm_line* lines;
  size_t pos;
} sasm_parser;

/**
 * Returns a new SASM Parser structure
 *
 * @param The linked tokens array to be parsed
 *
 * After use sasm parser, you should call sasm_parser_delete.
 */
sasm_parser* sasm_parser_new(sasm_token*);

/**
 * Clear parser memory
 *
 * @param The parser structure
 */
void sasm_parser_delete(sasm_parser*);

/**
 * Returns the current token, if value is expected
 * else returns null
 *
 * @param The parser structure
 * @param The expected token type
 */
sasm_token* sasm_parser_consume(sasm_parser*, sasm_token_type);

/**
 * Returns the current token being parsed
 *
 * @param The parser structure
 */
sasm_token* sasm_parser_current(sasm_parser*);

/**
 * Returns the next token without advance
 *
 * @param The parser structure
 */
sasm_token* sasm_parser_next(sasm_parser*);

/**
 * Advance tokens array and returns current.
 *
 * @param The parser structure
 */
sasm_token* sasm_parser_advance(sasm_parser*);

/**
 * Parse the tokens
 *
 * @param The parser structure
 *
 * The result of parsing will be saved at sasm_parser#lines
 */
void sasm_parser_parse(sasm_parser*);

#endif