#include "sasm/lexer.h"

#include "kcstd/char.h"
#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/string.h"
#include "kcstd/types.h"

sasm_lexer* sasm_lexer_new(string input) {
  sasm_lexer* it = memory_alloc(sizeof(sasm_lexer));
  it->input = input;
  it->tokens = null;
  it->pos = 0;
  it->line = 1;
  it->col = 1;
  return it;
}

void sasm_lexer_delete(sasm_lexer* self) {
  memory_free(self);
}

void sasm_lexer_advance(sasm_lexer* self) {
  if (self->input[self->pos] == '\n') {
    self->line++;
    self->col = 1;
  } else {
    self->col++;
  }
  self->pos++;
}

void sasm_lexer_tokenize(sasm_lexer* self) {
  size_t input_len = str_len(self->input);
  while (self->pos < input_len) {
    char ch = self->input[self->pos];

    if (is_space(ch)) {
      sasm_lexer_advance(self);
      continue;
    }

    switch (ch) {
      case ':':
        sasm_token_add(&self->tokens, ":", TOKEN_COLON, self->line, self->col);
        sasm_lexer_advance(self);
        continue;
      case ',':
        sasm_token_add(&self->tokens, ",", TOKEN_COMMA, self->line, self->col);
        sasm_lexer_advance(self);
        continue;
      case '[':
        sasm_token_add(&self->tokens, "[", TOKEN_LBRACKET, self->line,
                       self->col);
        sasm_lexer_advance(self);
        continue;
      case ']':
        sasm_token_add(&self->tokens, "]", TOKEN_RBRACKET, self->line,
                       self->col);
        sasm_lexer_advance(self);
        continue;
    };

    // comments (starting with ;)
    if (str_starts_with_ofs(self->input, ";", self->pos)) {
      self->pos += 2;
      while (self->pos < input_len && self->input[self->pos] != '\n') {
        sasm_lexer_advance(self);
      }
      continue;
    }

    // hexadecimal or decimal number (starts with digit or 0x)
    if (is_digit(ch)) {
      size_t start = self->pos;

      if (self->input[self->pos] == '0' && (self->pos + 1 < input_len) &&
          (self->input[self->pos + 1] == 'x' ||
           self->input[self->pos + 1] == 'X')) {
        // hexadecimal
        sasm_lexer_advance(self);  // consume '0'
        sasm_lexer_advance(self);  // consume 'x' or 'X'
        while (self->pos < input_len && is_hex_digit(self->input[self->pos])) {
          sasm_lexer_advance(self);
        }
      } else {
        // decimal
        while (self->pos < input_len && is_digit(self->input[self->pos])) {
          sasm_lexer_advance(self);
        }
      }

      string num = str_substring(self->input, start, self->pos);
      sasm_token_add(&self->tokens, num, TOKEN_IDENTIFIER, self->line,
                     self->col);
      memory_free(num);
      continue;
    }

    // identifiers and keywords
    if (is_alpha(ch) || ch == '_') {
      size_t start = self->pos;
      while (self->pos < input_len && (is_alpha(self->input[self->pos]) ||
                                       is_digit(self->input[self->pos]) ||
                                       self->input[self->pos] == '_')) {
        sasm_lexer_advance(self);
      }
      string word = str_substring(self->input, start, self->pos);
      if (word == null) {
        printf("Error[%d:%d]: Failed to get word", self->line, self->col);
        break;
      }
      if (str_equals(word, "MOV") || str_equals(word, "ADD") ||
          str_equals(word, "SUB") || str_equals(word, "DIV") ||
          str_equals(word, "INT")) {
        sasm_token_add(&self->tokens, word, TOKEN_KEYWORD, self->line,
                       self->col);
      } else {
        sasm_token_add(&self->tokens, word, TOKEN_IDENTIFIER, self->line,
                       self->col);
      }
      memory_free(word);
      continue;
    }

    // just ignore the invalid char
    sasm_lexer_advance(self);
  }

  sasm_token_add(&self->tokens, "", TOKEN_EOF, self->line, self->col);
}
