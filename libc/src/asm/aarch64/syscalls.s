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

// size_t __ASM_WRITE__(int fd, const void* buffer, size_t len)
__ASM_WRITE__:
    mov    x8, #64        // syscall write
    svc    #0              // system call
    ret                   // returns to caller

// void __ASM_EXIT__(int status)
__ASM_EXIT__:
    mov    x8, #93        // syscall exit
    svc    #0              // ends the process
    ret

// int __ASM_OPENAT__(int dirfd, const char* filename, int flags, int mode)
__ASM_OPENAT__:
    mov    x8, #56  // openat syscall
    svc    #0
    ret

// size_t __ASM_READ__(int fd, void* dest, size_t count)
__ASM_READ__:
    mov    x8, #63  // read syscall
    svc    #0
    ret

// void __ASM_CLOSE__(int fd)
__ASM_CLOSE__:
    mov    x8, #57  // close syscall
    svc    #0
    ret

// void* __ASM_MMAP__(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
__ASM_MMAP__:
    mov    x8, #222
    svc    #0
    ret

// void __ASM_MUNMAP__(void* ptr, size_t size)
__ASM_MUNMAP__:
    mov    x8, #215
    svc    #0
    ret

// int64_t __ASM_CLOCK_GETTIME__(clockid_t clock_id, time_spec* ts)
__ASM_CLOCK_GETTIME__:
    mov    x8, #113        // syscall clock_gettime
    svc    #0
    ret

// long __ASM_LSEEK__(int fd, int offset, int whence);
__ASM_LSEEK__:
    mov    x8, #62
    svc    #0
    ret