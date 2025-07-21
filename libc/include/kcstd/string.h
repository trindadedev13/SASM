#ifndef __STRING_H__
#define __STRING_H__

#include "kcstd/types.h"

#define NULL_TERMINATOR '\0'

size_t str_len(const string str);

// Compares the strings
// Returns positive if a > b
// Returns negative if a < b
// Returns 0 if a == b
int str_cmp(string a, string b);
// Compares the bytes up to 'lmt' of a & b
// Returns positive if a > b
// Returns negative if a < b
// Returns 0 if a == b
int str_cmp_lmt(string a, string b, size_t lmt);

// Compare the string and check if they are equals
bool str_equals(string a, string b);

// Concatenates a string new in string src
string str_cat(string src, string new);

// Copies all string
string str_copy(string dest, string src);
// Copies just 'lmt' bytes of string src to dest
string str_copy_lmt(string dest, string src, size_t lmt);

// Duplicates a string
// Returns a allocated string
string str_dup(const string src);

string str_substring(const string str, size_t start, size_t end);

bool str_starts_with(string str, string start_with, size_t offset);

#endif