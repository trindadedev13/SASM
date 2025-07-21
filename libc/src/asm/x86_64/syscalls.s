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

# void __ASM_WRITE__(int fd, const void *buf, size_t len)
__ASM_WRITE__:
    movq    $1, %rax       # syscall number for write
    syscall                # perform syscall
    ret                    # return to caller

# void __ASM_EXIT__ exit(int status)
__ASM_EXIT__:
    movq     $60, %rax      # syscall number for exit
    syscall                # perform syscall

# int __ASM_OPENAT__(int dirfd, const char* filename, int flags, int mode)
__ASM_OPENAT__:
    movq    $257, %rax  # openat syscall
    syscall
    ret

# size_t __ASM_READ__(int fd, void* dest, size_t count)
__ASM_READ__:
    movq    $0, %rax  # read syscall
    syscall
    ret

# void __ASM_CLOSE__(int fd)
__ASM_CLOSE__:
    movq    $3, %rax  # close syscall
    syscall
    ret

# void* __ASM_MMAP__(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
__ASM_MMAP__:
    movq    $9, %rax           # syscall number for mmap
    syscall
    ret

# void __ASM_MUNMAP__(void* ptr, size_t size)
__ASM_MUNMAP__:
    movq    $11, %rax          # syscall number for munmap
    syscall
    ret

# int64_t __ASM_CLOCK_GETTIME__(clockid_t clock_id, time_spec* ts)
__ASM_CLOCK_GETTIME__:
    movq    $228, %rax
    syscall
    ret

# long __ASM_LSEEK__(int fd, int offset, int whence);
__ASM_LSEEK__:
    movq    $8, %rax         # syscall number lseek
    syscall
    ret