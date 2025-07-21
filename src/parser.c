#include "sasm/parser.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"

// #include "sasm/regs.h"

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
    exit(1);
    return NULL;
  }
  self->pos++;
  return token;
}

sasm_token* sasm_parser_current(sasm_parser* self) {
  sasm_token* token = sasm_token_get_at(self->tokens, self->pos);
  if (token == null) {
    printf("Failed to get current token at position %d\n", self->pos);
    exit(1);
    return null;
  }
  return token;
}

sasm_token* sasm_parser_next(sasm_parser* self) {
  sasm_token* token = sasm_token_get_at(self->tokens, self->pos + 1);
  if (token == null) {
    printf("Failed to get next token at position %d\n", self->pos + 1);
    exit(1);
    return null;
  }
  return token;
}

sasm_token* sasm_parser_advance(sasm_parser* self) {
  self->pos++;
  return sasm_parser_current(self);
}

void sasm_parser_parse(sasm_parser* self) {
  sasm_node* current_label = NULL;

  while (true) {
    sasm_token* token = sasm_parser_current(self);

    if (token == NULL) {
      printf("Fatal error: Unexpected end of token stream\n");
      break;
    }

    if (token->type == TOKEN_EOF)
      break;

    switch (token->type) {
      case TOKEN_KEYWORD: {
        string v1 = null;
        string v2 = null;
        string v3 = null;

        sasm_token* opcode = sasm_parser_consume(self, TOKEN_KEYWORD);

        if (str_equals(opcode->value, "INT")) {
          sasm_token* id = sasm_parser_consume(self, TOKEN_IDENTIFIER);
          sasm_node* node = sasm_interrupt_node_new(
              sasm_intid_fromstr(id->value), id->line, id->col);

          if (current_label != NULL) {
            sasm_node_add(&current_label->label_t.body, node);
          } else {
            sasm_node_add(&self->nodes, node);
          }
          continue;
        }

        v1 = sasm_parser_consume(self, TOKEN_IDENTIFIER)->value;

        if (sasm_parser_current(self)->type == TOKEN_COMMA) {
          sasm_parser_consume(self, TOKEN_COMMA);

          if (sasm_parser_current(self)->type == TOKEN_LBRACKET) {
            sasm_parser_consume(self, TOKEN_LBRACKET);

            string array_values = memory_alloc(1);
            array_values[0] = '\0';

            bool first = true;

            while (sasm_parser_current(self)->type != TOKEN_RBRACKET) {
              if (!first) {
                // só consome a vírgula, NÃO adiciona ao array
                sasm_parser_consume(self, TOKEN_COMMA);
              }

              sasm_token* value_token =
                  sasm_parser_consume(self, TOKEN_IDENTIFIER);
              if (value_token != null) {
                size_t old_len = str_len(array_values);
                size_t val_len = str_len(value_token->value);

                array_values = memory_realloc(
                    array_values,
                    old_len + val_len + 2);  // +1 para espaço ou '\0'
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

        sasm_node* node = sasm_instruction_node_new(
            sasm_instruction_opcode_fromstr(opcode->value), v1, v2, v3,
            opcode->line, opcode->col);

        if (current_label != NULL) {
          sasm_node_add(&current_label->label_t.body, node);
        } else {
          sasm_node_add(&self->nodes, node);
        }

        // printf("{\n\tdest = %s\n\tval = %s\n}\n", v1, v2);
        break;
      }

      case TOKEN_IDENTIFIER: {
        if (sasm_parser_next(self)->type == TOKEN_COLON) {
          sasm_token* id = sasm_parser_consume(self, TOKEN_IDENTIFIER);
          sasm_parser_consume(self, TOKEN_COLON);

          sasm_node* label_node =
              sasm_label_node_new(id->value, id->line, id->col);
          current_label = label_node;

          sasm_node_add(&self->nodes, label_node);
          continue;
        } else {
          printf("Warning[%d:%d]: Unexpected identifier '%s'\n", token->line,
                 token->col, token->value);
          sasm_parser_advance(self);
        }
        break;
      }

      case TOKEN_COMMA:
      case TOKEN_COLON:
      default: {
        printf("Error[%d:%d]: Unexpected token %s:%s\n", token->line,
               token->col, sasm_token_type_tostr(token->type), token->value);
        sasm_parser_advance(self);
        break;
      }
    }
  }
}
