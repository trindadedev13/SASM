#include "kcstd/string.h"

#include "kcstd/char.h"
#include "kcstd/limits.h"
#include "kcstd/memory.h"
#include "kcstd/types.h"

size_t str_len(const string str) {
  size_t len = 0;
  while (str[len] != NULL_TERMINATOR) {
    len++;
  }
  return len;
}

int str_cmp(string a, string b) {
  while (*a && (*a == *b)) {
    a++;
    b++;
  }
  return *(byte*)a - *(byte*)b;
}

int str_cmp_ofs(string a, string b, size_t ofs) {
  size_t i = 0;
  while (i < ofs) {
    if (a[i] != b[i]) {
      return (byte)a[i] - (byte)b[i];
    }
    if (a[i] == NULL_TERMINATOR || b[i] == '\0')
      break;
    i++;
  }
  return 0;
}

bool str_equals(string a, string b) {
  return str_cmp(a, b) == 0;
}

string str_cat(string src, string new) {
  string ptr = src;
  while (*ptr != NULL_TERMINATOR) {
    ptr++;
  }
  while (*new != NULL_TERMINATOR) {
    *ptr = *new;
    ptr++;
    new++;
  }
  *ptr = NULL_TERMINATOR;
  return src;
}

string str_copy(string dest, string src) {
  string og_d = dest;
  while (*src != NULL_TERMINATOR) {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = NULL_TERMINATOR;
  return og_d;
}

string str_copy_ofs(string dest, string src, size_t ofs) {
  size_t i = 0;
  while (i < ofs && src[i] != NULL_TERMINATOR) {
    dest[i] = src[i];
    i++;
  }
  while (i < ofs) {
    dest[i] = NULL_TERMINATOR;
    i++;
  }
  return dest;
}

string str_dup(const string src) {
  size_t len = str_len(src) + 1;
  void* new = memory_alloc(len);
  if (new == null)
    return null;
  return memory_copy(new, src, len);
}

string str_substring(const string str, size_t start, size_t end) {
  if (!str || start > end || end > str_len(str)) {
    return null;
  }

  size_t len = end - start;
  string result = memory_alloc(len + 1);
  if (!result)
    return NULL;

  memory_copy(result, str + start, len);
  result[len] = '\0';
  return result;
}

bool str_starts_with(string str, string start_with) {
  if (str_cmp_ofs(str, start_with, str_len(start_with)) == 0) {
    return true;
  }
  return false;
}

bool str_starts_with_ofs(string str, string start_with, size_t offset) {
  if (str_cmp_ofs(str + offset, start_with, str_len(start_with)) == 0) {
    return true;
  }
  return false;
}

long str_tol(string nptr, string* endptr, int base) {
  string s = nptr;
  long result = 0;
  int sign = 1;

  // skip spaces
  while (is_space(*s))
    s++;

  // trheat signal
  if (*s == '-') {
    sign = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  // Detect base auto
  if ((base == 0 || base == 16) && s[0] == '0' &&
      (s[1] == 'x' || s[1] == 'X')) {
    base = 16;
    s += 2;
  } else if (base == 0 && s[0] == '0') {
    base = 8;
    s++;
  } else if (base == 0) {
    base = 10;
  }

  // Digit-Digit Convertion
  while (*s) {
    int digit;
    if (is_digit(*s))
      digit = *s - '0';
    else if (is_alpha(*s))
      digit = to_lower(*s) - 'a' + 10;
    else
      break;

    if (digit >= base)
      break;

    // Overflow check
    if (result > (LONG_MAX - digit) / base) {
      result = LONG_MAX;
      break;
    }

    result = result * base + digit;
    s++;
  }

  if (endptr)
    *endptr = (string)s;

  return sign * result;
}