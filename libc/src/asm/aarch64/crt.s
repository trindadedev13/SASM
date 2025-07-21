.section .text
    .global _start
    .extern main

_start:
    // stack layout (sp)
    // [argc]
    // [argv[0]...argv[argc - 1]]
    // [NULL]
    // [envp[0]...]
    // [NULL]

    mov    x0, sp      // x0 = &sp
    ldr    x1, [x0]    // x1 = argc
    add    x2, x0, #8  // x2  = &argv[0]

    mov    x0, x1  // x0 = argc
    mov    x1, x2  // x1 = argv (char**)

    // call main.
    // result will be in x0
    bl     main

    // exit
    mov    x8, #93
    svc    0