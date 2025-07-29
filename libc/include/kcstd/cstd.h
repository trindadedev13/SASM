#ifndef __KCSTD_CSTD_H_
#define __KCSTD_CSTD_H_

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/string.h"

// Macros for who likes c names

// IO
#define putchar(c) put_char(c)
#define puts(s) put_string(s)

// IO::FILE
#define FILE file
#define fopen(str, mode) file_open(str, mode)
#define fread(ptr, size, count, file) file_read(file, ptr, (size) * (count))
#define fwrite(ptr, size, count, file) file_write(file, ptr, (size) * (count))
#define fclose(ptr) file_close(ptr)

// Memory
#define malloc(size) memory_alloc(size)
#define realloc(ptr, new_size) memory_realloc(ptr, new_size)
#define free(ptr) memory_free(ptr)
#define memmove(dest, src, size) memory_move(dest, src, size)
#define memcpy(dest, src, size) memory_copy(dest, src, size)
#define memset(dest, new, size) memory_set(dest, new, size)

// String
#define strlen(str) str_len(str)
#define strcmp(a, b) str_cmp(a, b)
#define strncmp(a, b, ofs) str_cmp_ofs(a, b, ofs)
#define strcat(src, new) str_cat(src, new)
#define strcpy(dest, src) str_copy(dest, dest)
#define strncpy(dest, src, ofs) str_copy_ofs(dest, src, ofs)
#define strdup(src) str_dup(src)

// Char
#define isspace(c) is_space(c)
#define isdigit(c) is_digit(c)
#define isalpha(c) is_alpha

#endif