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

    mov    r0, sp      // r0 = &sp
    ldr    r1, [r0]    // r1 = argc
    add    r2, r0, #4  // r2  = &argv[0]

    mov    r0, r1  // r0 = argc
    mov    r1, r2  // r1 = argv (char**)

    // call main.
    // result will be in r0
    bl     main

    // exit
    mov    r7, #1
    svc    0