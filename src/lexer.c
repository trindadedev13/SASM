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

void sasm_token_add(sasm_token** head,
                    string value,
                    sasm_token_type type,
                    size_t line,
                    size_t col) {
  sasm_token* new_token = memory_alloc(sizeof(sasm_token));
  new_token->type = type;
  new_token->value = str_dup(value);
  new_token->line = line;
  new_token->col = col;
  new_token->next = null;

  if (*head == null) {
    *head = new_token;
  } else {
    sasm_token* current = *head;
    while (current->next)
      current = current->next;
    current->next = new_token;
  }
}

string sasm_token_type_tostr(sasm_token_type type) {
  switch (type) {
    case TOKEN_KEYWORD:
      return "Keyword";
    case TOKEN_IDENTIFIER:
      return "Identifer";
    case TOKEN_COMMA:
      return "Comma";
    case TOKEN_COLON:
      return "Colon";
  };
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
    };

    if (str_starts_with(self->input, ";", self->pos)) {
      self->pos += 2;
      while (self->pos < input_len && self->input[self->pos] != '\n') {
        sasm_lexer_advance(self);
      }
      continue;
    }

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
      if (str_equals(word, "mov") || str_equals(word, "add") ||
          str_equals(word, "sub") || str_equals(word, "div")) {
        sasm_token_add(&self->tokens, word, TOKEN_KEYWORD, self->line,
                       self->col);
      } else {
        sasm_token_add(&self->tokens, word, TOKEN_IDENTIFIER, self->line,
                       self->col);
      }
      memory_free(word);
      continue;
    }
    // other cases ahhhhhhhhhhhshshss
    sasm_lexer_advance(self);
  }
}
