/* fib_iter_a.s */
.global fib_iter_a
.func fib_iter_a

/* int fib_iter_a(int n) */
/* int n : r0 */

/* int i : r1 */
/* int previous : r2 */
/* int current : r3 */
/* int tmp : r4 */

fib_iter_a:
    cmp r0, #0
    moveq r3, #0
    beq done
    mov r1, #1
    mov r2, #0
    mov r3, #1

loop:
    cmp r1, r0
    beq done
    mov r4, r3
    add r3, r3, r2
    mov r2, r4
    add r1, r1, #1
    b loop

done:
    mov r0, r3
    bx lr
