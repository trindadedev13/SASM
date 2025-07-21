.section .text
    .global _start
    .extern main

_start:
    # stack layout (rsp)
    # [argc]
    # [argv[0]...argv[argc - 1]]
    # [NULL]
    # [envp[0]...]
    # [NULL]

    mov    (%esp), %eax        # eax = argc
    lea    4(%esp), %ebx       # ebx = argv (char**)

    push   %ebx                # push argv
    push   %eax                # push argc
    call   main

    # eax already contains return value
    # call exit syscall
    mov    %eax, %ebx          # ebx = return code
    mov    $1, %eax            # eax = syscall number (exit)
    int    $0x80               # syscall