#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/string.h"

#include "sasm/lexer.h"

int sasm_err(int s, string m) {
  printf("Err(%d): %s\n", s, m);
  return s;
}

int main(int argc, string argv[]) {
  if (argc != 2)
    return sasm_err(1, "Please provide one file");

  string filename = argv[1];
  printf("%s\n", filename);

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

  sasm_lexer* lexer = sasm_lexer_new(content);
  sasm_lexer_tokenize(lexer);
  sasm_token* current_token = lexer->tokens;
  while (current_token != null) {
    printf("\nToken{\n\ttype=%s,\n\tvalue=%s,\n\tline=%d,\n\tcol=%d\n}",
           sasm_token_type_tostr(current_token->type), current_token->value,
           current_token->line, current_token->col);
    current_token = current_token->next;
  }

  memory_free(content);
  file_close(file);
  return 0;
}