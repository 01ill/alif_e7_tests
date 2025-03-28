.text
.type dotp_mve, %function
.global dotp_mve

/*
r0 + q0: A, r1 + q1: B, r2 + q2: C

r3: Länge des Arrays
r4: LR Zwischenspeicher
*/



dotp_mve:
    mov r5, #0
    vdup.32 q2, r5 // init c
    mov r4, lr // save lr
    dlstp.32 lr, r3 // start loop

loopMVE:
    vldrw.f32 q0, [r0], #16 // Vier Elemente von A laden
    vldrw.f32 q1, [r1], #16 // Vier Elemente von B laden

    vfma.f32 q2, q0, q1 // c[i] += A[i] * B[i]

    letp lr, loopMVE // check loop

loopEnd:
    // q2[0] = s8, q2[1] = s9, q2[2] = s10, q2[3] = s11
    vadd.f32 s2, s8, s9
    vadd.f32 s2, s2, s10
    vadd.f32 s2, s2, s11

    // vstrw.f32 q2, [r2] // c speichern
    vstr.32 s2, [r2]
    vmov r0, s2 // return c

    mov lr, r4 // restore lr
    bx lr // return
