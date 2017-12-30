#include <stdio.h>
#include "header.h"

// creates an array within a given range
void initialize_array(int *array, int n, int start) {
    int i;

    for(i = 0; i < n; i++) {
        array[i] = start + i;
    }
}

// Tests the sum array assembly given a range
void sum_array_test(int start_num, int end_num) {
    struct arm_state *as;
    int c_sum;
    int a_sum;
    int array_length = end_num - start_num + 1;
    int array[array_length];

    initialize_array(&array[0], array_length, start_num);
    as = arm_state_new(1024, (unsigned int *) sum_array_a, (unsigned int) &array[0], array_length, 0, 0);

    c_sum = sum_array_c(&array[0], array_length);
    a_sum = arm_state_execute(as);
    
    printf("Sum of numbers from %d-%d\n", start_num,  end_num);
    printf("C sum: %d\n", c_sum);
    printf("Assembler sum: %d\n", a_sum);
    arm_analysis_print(as);
    printf("\n");
    
    arm_state_free(as);
}

// Tests find max value given a range
void find_max_test(int start, int end) {
    struct arm_state *as;
    int c_max;
    int a_max;
    int array_length = end - start + 1;
    int array[array_length];

    initialize_array(&array[0], array_length, start);
    as = arm_state_new(1024, (unsigned int *) find_max_a, (unsigned int) &array[0], array_length, 0, 0);

    c_max = find_max_c(&array[0], array_length);
    a_max = arm_state_execute(as);

    printf("Max numbers from %d-%d\n", start,  end);
    printf("C max: %d\n", c_max);
    printf("Assembler max: %d\n", a_max);
    arm_analysis_print(as);
    printf("\n");
    arm_state_free(as);
}

// Tests fibonacci iterative version given a number
void fib_iter_test(int n) {
    int i;

    printf("C Solution\n");
    for (i = 0; i < n; i++) {
        printf("%d : %d\n", i, fib_iter_c(i));
    }
    printf("\n");

    printf("Assembly Solution\n");
    for (i = 0; i < n; i++) {
        struct arm_state *as;
        as = arm_state_new(1024, (unsigned int *) fib_iter_a, i, 0, 0, 0);
        printf("%d : %d\n", i, arm_state_execute(as));
        arm_analysis_print(as);
        arm_state_free(as);
    }
    printf("\n");
}

// Tests fibonacci recursive version given a number
void fib_rec_test(int n) {
    int i;
 
    printf("C Solution\n");
    for (i = 0; i < n; i++) {
        printf("%d : %d\n", i, fib_rec_c(i));
    }
    printf("\n");

    printf("Assembly Solution\n");
    for (i = 0; i < n; i++) {
        struct arm_state *as;
        as = arm_state_new(1024, (unsigned int *) fib_rec_a, i, 0, 0, 0);
        printf("%d : %d\n", i, arm_state_execute(as));
        arm_analysis_print(as);
        arm_state_free(as);
    }
    printf("\n");
}

// Tests the find string functions given a string and substring
void find_str_test(char *s, char *sub) {
    struct arm_state *as = arm_state_new(1024, (unsigned int *) find_str_a, (unsigned int) s, (unsigned int) sub, 0, 0);
    int location;

    printf("C solution:\n");
    location = find_str_c(s, sub);

    printf("Location of '%s' in '%s' is: %d\n", s, sub, location);

    printf("Assembly Solution:\n");
    location = arm_state_execute(as);
    printf("Location of '%s' in '%s' is: %d\n", s, sub, location);
    arm_analysis_print(as);
    
    arm_state_free(as);
}

int main(void) {
    printf("------------Sum Aarray Test------------\n");
    sum_array_test(-500, - 250); 
    sum_array_test(-250, 0); 
    sum_array_test(0, 250); 
    sum_array_test(200, 1200); 

    printf("------------Find Max Test------------\n");
    find_max_test(-500, 250);
    find_max_test(-250, 0);
    find_max_test(0, 250);
    find_max_test(-200, 800);
    
    printf("------------Fibonacci Iterative Test------------\n");
    printf("(Place : Fibonacci number):\n"); 
    fib_iter_test(20);
    
    printf("------------Fibonacci Recursive Test------------\n");
    printf("(Place : Fibonacci number):\n");
    fib_rec_test(20);
    
    printf("------------Find String Test------------\n");
    char sub1[] = "sub beginning test";
    char s1[] = "sub";
    char sub2[] = "middle sub test";
    char s2[] = "sub";
    char sub3[] = "end test sub";
    char s3[] = "sub";
    char sub4[] = "partial (not found) test end su";
    char s4[] = "sub";
    char sub5[] = "partial (not found) su test";
    char s5[] = "sub";
    
    find_str_test(s1, sub1);
    find_str_test(s2, sub2);
    find_str_test(s3, sub3);
    find_str_test(s4, sub4);
    find_str_test(s5, sub5);
}
