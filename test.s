push {r4}
push {lr}
vmov.f32 r3, s0
mov r4, #4
dlstp.32 lr, r4
loop:
vldrw.f32 q0, [r0], #16
vldrw.f32 q1, [r1], #16
vfma.f32 q2, q0, r3
vstrw.f32 q2, [r2], #16
letp lr, #-20
pop {r4}
pop {pc}

pop {r1, r4, pc}