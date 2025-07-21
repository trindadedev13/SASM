#ifndef __LEXER_H__
#define __LEXER_H__

#include "kcstd/types.h"
#include "sasm/token.h"

typedef struct {
  sasm_token* tokens;
  string input;
  size_t pos;
  size_t line;
  size_t col;
} sasm_lexer;

sasm_lexer* sasm_lexer_new(string);

void sasm_lexer_delete(sasm_lexer*);

void sasm_lexer_tokenize(sasm_lexer*);

#endif