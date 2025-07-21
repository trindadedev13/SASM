#ifndef __LEXER_H__
#define __LEXER_H__

#include "kcstd/types.h"

typedef enum {
  TOKEN_KEYWORD,     // mov, add, div, sub, mul
  TOKEN_IDENTIFIER,  // Name
  TOKEN_COLON,       // :
  TOKEN_COMMA        // ,
} sasm_token_type;

typedef struct sasm_token sasm_token;

struct sasm_token {
  string value;
  sasm_token_type type;
  size_t line;
  size_t col;
  sasm_token* next;
};

void sasm_token_add(sasm_token**, string, sasm_token_type, size_t, size_t);

string sasm_token_type_tostr(sasm_token_type);

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