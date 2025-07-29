#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/string.h"

#include "sasm/cpu.h"
#include "sasm/interpreter.h"
#include "sasm/lexer.h"
#include "sasm/parser.h"
#include "sasm/regs.h"

int sasm_err(int s, string m) {
  printf("Err(%d): %s\n", s, m);
  return s;
}

int main(int argc, string argv[]) {
  if (argc != 2)
    return sasm_err(1, "Please provide one file");

  string filename = argv[1];

  /** Open the file */
  file* file = file_open(filename, FILE_MODE_READ);
  if (file == null)
    return sasm_err(1, "Failed to open file.");

  /** Get file size */
  const int64_t size = file_seek(file, 0, SEEK_END);
  file_seek(file, 0, SEEK_SET);

  /** Read all content of file */
  string content = memory_alloc(size);
  if (content == null)
    return sasm_err(1, "Failed to allocate content buffer.");
  file_read(file, content, size);

  sasm_cpu_init();

  sasm_lexer* lexer = sasm_lexer_new(content);
  sasm_lexer_tokenize(lexer);

  // sasm_token* current_token = lexer->tokens;
  // while (current_token != null) {
  // printf("Token{\n\ttype=%s,\n\tvalue=%s,\n\tline=%d,\n\tcol=%d\n}\n",
  // sasm_token_type_tostr(current_token->type), current_token->value,
  // current_token->line, current_token->col);
  // current_token = current_token->next;
  // }

  sasm_parser* parser = sasm_parser_new(lexer->tokens);
  sasm_parser_parse(parser);

  sasm_interpreter* interpreter = sasm_interpreter_new(parser->lines);
  sasm_interpreter_run(interpreter);

  // printf("{\n\tA = %d\n\tB = %d\n\tC = %d\n\tD = %d\n}\n", sasm_regs_getA(),
  // sasm_regs_getB(), sasm_regs_getC(), sasm_regs_getD());

  memory_free(content);
  file_close(file);
  return 0;
}