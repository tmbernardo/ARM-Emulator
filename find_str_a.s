/* find_str.s */
.global find_str_a
.func find_str_a

/* int find_str_a(char *s, char *sub) */
/* char *s : r0 */
/* char *sub : r1 */

/* int i : r2 */
/* int j : r3 */
/* int sub[i] : r4 */
/* char s[j] : r5 */
/* int k : r6 */

find_str_a:
    mov r2, #0
    mov r3, #0

sub_loop:
    ldrb r4, [r1, r2]
    
    cmp r4, #0
    moveq r2, #-1
    beq done
    
    b s_loop

s_loop:
    add r6, r2, r3
    ldrb r4, [r1, r6]
    ldrb r5, [r0, r3]
    
    cmp r5, #0
    beq done
    
    cmp r4, #0
    moveq r2, #-1
    beq done

    cmp r4, r5
    addeq r3, r3, #1
    beq s_loop
    addne r2, r2, #1
    movne r3, #0
    bne sub_loop

done:
    mov r0, r2
    bx lr
