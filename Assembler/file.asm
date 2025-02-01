li r0, 256
li r1, 511
label:
    addi r0, r0, 1
    sub r2, r0, r1, r3
    jnz, r2, label
    li r0, 0
    jimm label