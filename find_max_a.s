/* find_max_a.s */
.global find_max_a
.func find_max_a

/* int find_max_a(int *array, int n) */
/* int *array : r0 */
/* int n : r1 */

/* int i : r2 */
/* int max : r3 */
/* int tmp : r4 */
    
find_max_a:
    mov r2, #1
    ldr r3, [r0, #0]

loop:
    cmp r2, r1
    beq done
    ldr r4, [r0, r2, lsl #2]
    cmp r4, r3
    movgt r3, r4
    add r2, r2, #1
    b loop

done:
    mov r0, r3
    bx lr
