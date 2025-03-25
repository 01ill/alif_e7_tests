.text
.type dotp_mve, %function
.global dotp_mve

/*
r0: A, r1: B, r2: C
r3: Länge des Arrays
r4: Laufzeitvariable i
*/



dotp_mve:
    mov r4, lr
    dlstp.32 lr, r3

loopMVE:
    vldrw.f32 q0, [r0], #16 // Vier Elemente von A laden
    vldrw.f32 q1, [r1], #16 // Vier Elemente von B laden
    vldrw.f32 q2, [r2] // Vier Elemente von C laden

    vmul.f32 q2, q1, q0
    // vadd.f32 q2, q1, q2

    vstrw.f32 q2, [r2], #16

    letp lr, loopMVE

    mov lr, r4
    bx lr
