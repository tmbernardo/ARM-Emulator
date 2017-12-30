/* sum_array_a.s */
.global sum_array_a
.func sum_array_a

/* void sum_array_a(int *array, int n) */
/* int *array : r0 */
/* int n : r1 */

/* int i : r2 */
/* int sum : r3 */
/* int tmp : r4 */
    
sum_array_a:
    mov r2, #0
    mov r3, #0

loop:
    cmp r2, r1
    beq done
    ldr r4, [r0, r2, lsl #2]
    add r3, r3, r4
    add r2, r2, #1
    b loop

done:
    mov r0, r3
    bx lr
    

