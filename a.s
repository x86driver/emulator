    mov r8, #start
    bx r8
func1:
    mov r4, #7
    mov pc, lr

start:
    mov r0, #data
    ldmia r0, {r1, r2}
    mov r3, #func1
    blx r3
    clz r0, r1
    mov r3, #0xf0
    eors r0, r4, #0x7
    rors r0, r2, #3
    mov r0, r1
#    movs r0, #-1
#    lsl r0, r0, #2
#    mov r0, #0xf0000000
#    mov r1, #0xf0000000
#    sub r2, r0, r1, lsr #1
end:
    mov r7, #1
    svc #0
data:
    .word 0xf2345678
    .word 0xfedcba99
data2:

#    mov r0, #0      /* index */
#    mov r1, #16777216    /* 0 ~ 100 */
#    mov r2, #1      /* increment */
#start:
#    add r3, r3, r2  /* result */
#    add r0, r0, r2  /* index++ */

#    cmp r0, r1
#    bne start
/*
    mov r1, #1
    add r0, r0, r1, lsl #2


    movlts r1, r2

    moveqs r1, #131072

    streq r15, [r14], #12
    ldrne r0, [r15]
    strlt r1, [r14, r3, lsr #3]!
    ldrgt r13, [r15]

    str r0, [r1, #34]!
    streqb r11, [r12]!

    ldr r13, [pc, #-8]!
    mov r8, #32
    mov r9, #33
    add r1, r3, r7
    ldr r0, [pc]
    ldr r1, [r14], #0xfff
    mov r13, #100
    ldr r2, [r13, #-33]
    ldr r3, [r12, #11]!

    ldrneb r4, [r11]
    ldrb r5, [r10], #200
    ldrb r6, [r9, #203]
    ldrb r7, [r8, #203]!
*/
