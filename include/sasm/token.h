#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "kcstd/types.h"

typedef enum {
  TOKEN_KEYWORD,     // mov, add, div, sub, mul
  TOKEN_IDENTIFIER,  // Name
  TOKEN_COLON,       // :
  TOKEN_COMMA,       // ,
  TOKEN_LBRACKET,    // [
  TOKEN_RBRACKET,    // [
  TOKEN_EOF
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

sasm_token* sasm_token_get_at(sasm_token*, size_t);

#endif