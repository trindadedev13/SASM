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
    mov    $4, %eax         # syscall number for write (x86)
    mov    4(%esp), %ebx    # fd
    mov    8(%esp), %ecx    # buf
    mov    12(%esp), %edx   # len
    int     $0x80
    ret

# void __ASM_EXIT__(int status)
__ASM_EXIT__:
    mov    $1, %eax         # syscall number for exit (x86)
    mov    4(%esp), %ebx    # status
    int     $0x80
    ret

# int __ASM_OPENAT__(int dirfd, const char* filename, int flags, int mode)
__ASM_OPENAT__:
    mov    $295, %eax       # syscall number for openat (x86)
    mov    4(%esp), %ebx    # dirfd
    mov    8(%esp), %ecx    # filename
    mov    12(%esp), %edx   # flags
    mov    16(%esp), %esi   # mode
    int     $0x80
    ret

# ssize_t __ASM_READ__(int fd, void* dest, size_t count)
__ASM_READ__:
    mov    $3, %eax         # syscall number for read (x86)
    mov    4(%esp), %ebx    # fd
    mov    8(%esp), %ecx    # dest
    mov    12(%esp), %edx   # count
    int     $0x80
    ret

# void __ASM_CLOSE__(int fd)
__ASM_CLOSE__:
    mov    $6, %eax         # syscall number for close (x86)
    mov    4(%esp), %ebx    # fd
    int     $0x80
    ret

# void* __ASM_MMAP__(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
__ASM_MMAP__:
    push   %ebp
    mov    %esp, %ebp

    mov    8(%ebp), %ebx     # addr
    mov    12(%ebp), %ecx    # length
    mov    16(%ebp), %edx    # prot
    mov    20(%ebp), %esi    # flags
    mov    24(%ebp), %edi    # fd
    mov    28(%ebp), %ebp    # offset (em páginas, cuidado)

    mov    $192, %eax        # syscall mmap2
    int    $0x80

    leave
    ret

# void __ASM_MUNMAP__(void* ptr, size_t size)
__ASM_MUNMAP__:
    push   %ebp
    mov    %esp, %ebp

    mov    $91, %eax         # syscall number for munmap
    mov    8(%ebp), %ebx     # addr
    mov    12(%ebp), %ecx    # length
    int    $0x80

    pop    %ebp
    ret

# int64_t __ASM_CLOCK_GETTIME__(clockid_t clock_id, time_spec* ts)
__ASM_CLOCK_GETTIME__:
    mov    $265, %eax     # syscall clock_gettime
    mov    4(%esp), %ebx
    mov    8(%esp), %ecx
    int    $0x80
    ret
    
    
# long __ASM_LSEEK__(int fd, int offset, int whence);
__ASM_LSEEK__:
    mov    $19, %eax         # syscall number lseek
    mov    4(%esp), %ebx     # fd
    mov    8(%esp), %ecx     # offset low 32 bits
    mov    12(%esp), %edx    # offset high 32 bits
    mov    16(%esp), %esi    # whence
    int    $0x80             # kernel syscall
    ret