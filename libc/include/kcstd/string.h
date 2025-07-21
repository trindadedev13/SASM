#ifndef __STRING_H__
#define __STRING_H__

#include "kcstd/types.h"

#define NULL_TERMINATOR '\0'
#define LONG_MAX 2147483647L  // 2^31 - 1 for 32-bit signed long

size_t str_len(const string str);

// Compares the strings
// Returns positive if a > b
// Returns negative if a < b
// Returns 0 if a == b
int str_cmp(string a, string b);
// Compares the bytes up to 'ofs' of a & b
// Returns positive if a > b
// Returns negative if a < b
// Returns 0 if a == b
int str_cmp_ofs(string a, string b, size_t ofs);

// Compare the string and check if they are equals
bool str_equals(string a, string b);

// Concatenates a string new in string src
string str_cat(string src, string new);

// Copies all string
string str_copy(string dest, string src);
// Copies just 'ofs' bytes of string src to dest
string str_copy_ofs(string dest, string src, size_t ofs);

// Duplicates a string
// Returns a allocated string
string str_dup(const string src);

string str_substring(const string str, size_t start, size_t end);

bool str_starts_with(string str, string start_with);

bool str_starts_with_ofs(string str, string start_with, size_t offset);

long str_tol(string, string*, int);

#endif