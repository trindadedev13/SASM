#ifndef __VA_ARGS__
#define __VA_ARGS__

// va list is implemented by compiler
// so we dont have to implement it

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif
typedef __gnuc_va_list va_list;

#define va_start(ap, last) __builtin_va_start((ap), last)
#define va_end(ap) __builtin_va_end((ap))
#define va_arg(ap, type) __builtin_va_arg((ap), type)

#endif