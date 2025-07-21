#include "sasm/token.h"

#include "kcstd/memory.h"
#include "kcstd/string.h"

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
      return "Identifier";
    case TOKEN_COMMA:
      return "Comma";
    case TOKEN_COLON:
      return "Colon";
    case TOKEN_LBRACKET:
      return "LeftBracket";
    case TOKEN_RBRACKET:
      return "RightBracket";
    case TOKEN_EOF:
      return "Eof";
  };
}

sasm_token* sasm_token_get_at(sasm_token* head, size_t pos) {
  size_t index = 0;
  while (head != null) {
    if (index == pos)
      return head;
    head = head->next;
    index++;
  }
  return null;
}