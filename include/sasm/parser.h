#ifndef __PARSER_H__
#define __PARSER_H__

#include "sasm/node.h"
#include "sasm/token.h"

typedef struct {
  sasm_token* tokens;
  sasm_node* nodes;
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