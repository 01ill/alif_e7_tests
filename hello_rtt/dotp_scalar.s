.text
.type dotp_scalar, %function
.global dotp_scalar

/*
r0: A
r1: B
r2: C
r3: Length
*/


dotp_scalar:
    mov r4, lr
    wls lr, r3, loopEnd
loopStart:
    vldr.f32 s0, [r0]
    vldr.f32 s1, [r1]
    vldr.f32 s2, [r2]
    //vmul s1, s1, s2
    // vadd s0, s1, s0
    vmul.f32 s2, s1, s0
    // vadd.f32 s2, s1, s2

    vstr s2, [r2]
    
    add r0, r0, #4
    add r1, r1, #4
    add r2, r2, #4
    le lr, loopStart

loopEnd:
    mov lr, r4
    bx lr
