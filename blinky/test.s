.text
.type run_asm, %function
.global run_asm

run_asm:
    mov r0, #0x10

    bx lr
