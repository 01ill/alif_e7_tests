.syntax unified
.text
.type dotp_scalar, %function
.global dotp_scalar
/*
r0: A -> s0
r1: B -> s1
r2: c -> s2
r3: Length

r4: Zwischenspeicher für LR
*/

dotp_scalar:
    mov r4, #0
    vdup.32 q0, r4 // init c

    mov r4, lr // save lr in r4
    // vldr.f32 s2, [r2] // Load c
    wls lr, r3, loopEnd // while (r3 > 0)

loopStart:
    ldr r5, [r0], #4
    vmov.f32 s0, r5
    ldr r6, [r1], #4
    vmov.f32 s1, r6
    //vldrw.f32 s0, [r0], #4 // Load A[i]
    //vldrw.f32 s1, [r1], #4 // Load B[i]
    vmla.f32 s2, s0, s1 // c += A[i] * B[i]
    
    //add r0, r0, #4 // A++
    //add r1, r1, #4 // B++
    le lr, loopStart

loopEnd:
    vstr.32 s2, [r2] // save c
    vmov r0, s2 // return c

    mov lr, r4
    bx lr
