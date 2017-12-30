#include <stdio.h>

int fib_iter_c(int n) {
    int i;
    int previous = 0;
    int current = 1;
    int tmp;

    if (n == 0) {
        return previous;
    }

    for (i = 1; i < n; i++) {
        tmp = current;
        current += previous;
        previous = tmp;
    }

    return current;
}
