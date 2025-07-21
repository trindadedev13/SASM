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

    mov    (%rsp), %rdi  #  rdi = argc
    lea    8(%rsp), %rsi #  rsi = argv(char**)

    # call user main
    call   main

    mov    %rax, %rdi # rdi = ret value 
    mov    $60, %rax  # rax = exit syscall
    syscall