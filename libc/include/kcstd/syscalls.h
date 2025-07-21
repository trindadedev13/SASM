#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include "kcstd/types.h"

extern ssize_t __ASM_WRITE__(int fd, const void* buffer, size_t len);

extern ssize_t __ASM_READ__(int fd, const void* dest, size_t count);

extern void __ASM_EXIT__(long status);

extern int __ASM_OPENAT__(int dirfd, const char* filename, int flags, int mode);

extern void __ASM_CLOSE__(int fd);

extern void* __ASM_MMAP__(void* addr,
                          size_t size,
                          int prot,
                          int flags,
                          int fd,
                          size_t offset);

extern void __ASM_MUNMAP__(void* ptr, size_t size);

extern uint64_t __ASM_LSEEK__(int fd, uint64_t offset, int whence);

#endif