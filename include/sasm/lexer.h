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

/**
 * Returns a new structure of SASM lexer
 *
 * @param the code string
 *
 * After use sasm lexer, you should call sasm_lexer_delete.
 */
sasm_lexer* sasm_lexer_new(string);

/**
 * Clear SASM lexer memory
 *
 * @param The lexer structure
 */
void sasm_lexer_delete(sasm_lexer*);

/**
 * Tokenize all code string to tokens
 *
 * @param The lexer structure
 *
 * The result of tokenization will be save at sasm_lexer->tokens
 */
void sasm_lexer_tokenize(sasm_lexer*);

#endif