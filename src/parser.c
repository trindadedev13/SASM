#include "sasm/parser.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"

sasm_line* sasm_line_new(string label,
                         sasm_instruction* ins,
                         size_t line,
                         size_t col) {
  sasm_line* it = memory_alloc(sizeof(sasm_line));
  if (label != null)
    it->label = str_dup(label);
  else
    it->label = null;
  it->instruction = ins;
  it->next = null;
  it->line = line;
  it->col = col;
  return it;
}

void sasm_line_add(sasm_line** head, sasm_line* new_line_t) {
  if (*head == null) {
    *head = new_line_t;
  } else {
    sasm_line* current = *head;
    while (current->next)
      current = current->next;
    current->next = new_line_t;
  }
}

sasm_instruction_type sasm_instruction_type_fromstr(string str) {
  if (str_equals(str, "MOV")) {
    return INS_MOV;
  } else if (str_equals(str, "ADD")) {
    return INS_ADD;
  } else if (str_equals(str, "SUB")) {
    return INS_SUB;
  } else if (str_equals(str, "INT")) {
    return INS_INT;
  } else if (str_equals(str, "JMP")) {
    return INS_JMP;
  } else if (str_equals(str, "CALL")) {
    return INS_CALL;
  } else if (str_equals(str, "RET")) {
    return INS_RET;
  }
  return -1;
}

sasm_interrupt_type sasm_interrupt_type_fromstr(string str) {
  if (str_equals(str, "0x80")) {
    return INT_SYSCALL;
  }
  return -1;
}

sasm_parser* sasm_parser_new(sasm_token* tokens) {
  sasm_parser* it = memory_alloc(sizeof(sasm_parser));
  it->tokens = tokens;
  it->pos = 0;
  return it;
}

void sasm_parser_delete(sasm_parser* self) {
  memory_free(self);
}

sasm_token* sasm_parser_consume(sasm_parser* self,
                                sasm_token_type expected_type) {
  sasm_token* token = sasm_token_get_at(self->tokens, self->pos);
  if (token->type != expected_type) {
    printf("Error at [%d:%d]: Expected %s, but got %s", token->line, token->col,
           sasm_token_type_tostr(expected_type),
           sasm_token_type_tostr(token->type));
    return null;
  }
  self->pos++;
  return token;
}

sasm_token* sasm_parser_current(sasm_parser* self) {
  sasm_token* token = sasm_token_get_at(self->tokens, self->pos);
  if (token == null) {
    printf("Failed to get current token at position %d\n", self->pos);
    return null;
  }
  return token;
}

sasm_token* sasm_parser_next(sasm_parser* self) {
  sasm_token* token = sasm_token_get_at(self->tokens, self->pos + 1);
  if (token == null) {
    printf("Failed to get next token at position %d\n", self->pos + 1);
    return null;
  }
  return token;
}

sasm_token* sasm_parser_advance(sasm_parser* self) {
  self->pos++;
  return sasm_parser_current(self);
}

void sasm_parser_parse(sasm_parser* self) {
  string current_label = null;

  while (true) {
    sasm_token* token = sasm_parser_current(self);
    if (token == null || token->type == TOKEN_EOF)
      break;

    switch (token->type) {
      case TOKEN_KEYWORD: {
        string v1 = null;
        string v2 = null;
        string v3 = null;

        sasm_token* opcode = sasm_parser_consume(self, TOKEN_KEYWORD);
        if (opcode == null) {
          printf("Error[%d:%d]: Failed to get token\n");
          break;
        }

        /**
         * check if its RET call, if so skip params checking.
         * You might be wondering why not just treat these parameters like in
         * others. well, we cant because if u call RET, next IDENTIFIER TOKEN can
         * be like new label and parser will think its a param of RET, so and
         * error. i dont like this way of fix it, because of duplication of
         * code, but at least it works.
         * aquiles trindade
         */
        if (str_equals(opcode->value, "RET")) {
          sasm_instruction* instr = memory_alloc(sizeof(sasm_instruction));
          instr->type = INS_RET;
          instr->v1 = null;
          instr->v2 = null;
          instr->v3 = null;

          sasm_line* line_t =
              sasm_line_new(current_label, instr, opcode->line, opcode->col);
          sasm_line_add(&self->lines, line_t);
          continue;
        }

        if (sasm_parser_current(self)->type == TOKEN_IDENTIFIER) {
          v1 = sasm_parser_consume(self, TOKEN_IDENTIFIER)->value;

          if (sasm_parser_current(self)->type == TOKEN_COMMA) {
            sasm_parser_consume(self, TOKEN_COMMA);

            if (sasm_parser_current(self)->type == TOKEN_LBRACKET) {
              sasm_parser_consume(self, TOKEN_LBRACKET);

              string array_values = memory_alloc(1);
              array_values[0] = '\0';
              bool first = true;

              while (sasm_parser_current(self)->type != TOKEN_RBRACKET) {
                if (!first)
                  sasm_parser_consume(self, TOKEN_COMMA);

                sasm_token* value_token =
                    sasm_parser_consume(self, TOKEN_IDENTIFIER);
                if (value_token != null) {
                  size_t old_len = str_len(array_values);
                  size_t val_len = str_len(value_token->value);

                  array_values =
                      memory_realloc(array_values, old_len + val_len + 2);
                  if (!first) {
                    str_cat(array_values, " ");
                  }
                  str_cat(array_values, value_token->value);
                }

                first = false;
              }

              sasm_parser_consume(self, TOKEN_RBRACKET);
              v2 = array_values;
            } else if (sasm_parser_current(self)->type == TOKEN_IDENTIFIER) {
              v2 = sasm_parser_consume(self, TOKEN_IDENTIFIER)->value;
            }

            if (sasm_parser_current(self)->type == TOKEN_COMMA) {
              sasm_parser_consume(self, TOKEN_COMMA);
              if (sasm_parser_current(self)->type == TOKEN_IDENTIFIER) {
                v3 = sasm_parser_consume(self, TOKEN_IDENTIFIER)->value;
              }
            }
          }
        }

        sasm_instruction* instr = memory_alloc(sizeof(sasm_instruction));
        instr->type = sasm_instruction_type_fromstr(opcode->value);
        instr->v1 = v1 ? str_dup(v1) : null;
        instr->v2 = v2 ? str_dup(v2) : null;
        instr->v3 = v3 ? str_dup(v3) : null;

        sasm_line* line_t =
            sasm_line_new(current_label, instr, opcode->line, opcode->col);

        sasm_line_add(&self->lines, line_t);
        current_label = null;
        break;
      }

      case TOKEN_IDENTIFIER: {
        sasm_token* id = sasm_parser_consume(self, TOKEN_IDENTIFIER);
        if (sasm_parser_current(self)->type == TOKEN_COLON) {
          sasm_parser_consume(self, TOKEN_COLON);
          if (sasm_parser_current(self)->type == TOKEN_KEYWORD) {
            if (current_label != null)
              memory_free(current_label);
            current_label = str_dup(id->value);
          } else {
            sasm_line* line_t =
                sasm_line_new(str_dup(id->value), null, id->line, id->col);
            sasm_line_add(&self->lines, line_t);
          }
          continue;
        }
        break;
      }

      case TOKEN_COMMA:
      case TOKEN_COLON:
      default:
        printf("Warning[%d:%d]: Unexpected token %s (%s)\n", token->line,
               token->col, sasm_token_type_tostr(token->type), token->value);
        sasm_parser_advance(self);
    }
  }
}