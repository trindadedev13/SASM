# SASM (Simulated Assembly)
- SASM is a simple operating system simulator that reads assembly language and does something.

## Interrupts
### INT 0x80
- Interrupt 80 is the Interrupt for #Syscalls.
- Usage:
```asm
MOV     D, <SyscallNumber>  ; The syscall
INT     0x80                ; The interrupt
```

## Syscalls
### 0x01 - PUTI
- Put Int, Print a integer in stdout
- A = int
- Usage:
```asm
MOV     A, 0x01  ; integer 1
MOV     D, 0x01  ; syscall PUTI
INT     0x80     ; perform syscall
```

### 0x01 - PUTC
- Put Char, Print a char in stdout
- A = char
- Returns in A = Bytes writter
- Usage:
```asm
MOV     A, 0x0A  ; char A in hexa
MOV     D, 0x02  ; syscall PUTC
INT     0x80     ; perform syscall
```