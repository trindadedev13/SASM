.section .text
    .global __ASM_WRITE__
    .global __ASM_EXIT__
    .global __ASM_OPENAT__
    .global __ASM_READ__
    .global __ASM_CLOSE__
    .global __ASM_MMAP__
    .global __ASM_MUNMAP__
    .global __ASM_CLOCK_GETTIME__
    .global __ASM_LSEEK__

// void __ASM_WRITE__(int fd, const void* buffer, size_t len)
__ASM_WRITE__:
    mov    r7, #4         // syscall write
    svc    #0              // system call
    bx     lr             // returns to caller

// void __ASM_EXIT__(int status)
__ASM_EXIT__:
    mov    r7, #1         // syscall exit
    svc    #0              // ends the process

// int __ASM_OPENAT__(int dirfd, const char* filename, int flags, int mode)
__ASM_OPENAT__:
    mov    r7, #56  // openat syscall
    svc    #0
    bx     lr

// size_t __ASM_READ__(int fd, void* dest, size_t count)
__ASM_READ__:
    mov    r7, #63  // read syscall
    svc    #0
    bx     lr

// void __ASM_CLOSE__(int fd)
__ASM_CLOSE__:
    mov    r7, #57  // close syscall
    svc    #0
    bx     lr

// void* __ASM_MMAP__(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
__ASM_MMAP__:
    mov    r7, #192
    svc    #0
    bx     lr

// void __ASM_MUNMAP__(void* ptr, size_t size)
__ASM_MUNMAP__:
    mov    r7, #91     // syscall munmap
    svc    #0
    bx     lr

// int64_t __ASM_CLOCK_GETTIME__(clockid_t clock_id, time_spec* ts)
__ASM_CLOCK_GETTIME__:
    mov    r7, #263    // syscall clock_gettime
    svc    #0
    bx     lr

// long __ASM_LSEEK__(int fd, int offset, int whence);
__ASM_LSEEK__:
    mov    r7, #19
    svc    #0
    bx     lr