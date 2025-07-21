#ifndef __PARSER_H__
#define __PARSER_H__

#include "sasm/lexer.h"

typedef struct {
  sasm_token token[128];
} sasm_parser;

#endif