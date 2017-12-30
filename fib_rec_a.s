/* fib_rec_a.s */
.global fib_rec_a
.func fib_rec_a

/* int fib_rec_a(int n) */
/* int n : sp */
/* return register : sp #4 */
/* fibonacci sum : sp #8 */

fib_rec_a:
    sub sp, sp, #16
    str lr, [sp, #4]
    str r0, [sp]
    
    cmp r0, #0
    beq fib_ret
    cmp r0, #1
    beq fib_ret
    
    b fib_else
    b fib_ret

fib_else:
    sub r0, r0, #1
    bl fib_rec_a
    
    str r0, [sp, #8]
    ldr r0, [sp]
    sub r0, r0, #2
    bl fib_rec_a
    
    ldr r1, [sp, #8]
    add r0, r1, r0

fib_ret:
    ldr lr, [sp, #4]
    add sp, sp, #16
    bx lr
