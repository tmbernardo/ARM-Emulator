#include <stdio.h>

int find_str_c(char *s, char *sub) {
    int i;
    int j;
    char c;

    for (i = 0; sub[i] != '\0'; i++) { 
        for (j = 0; s[j] != '\0' && sub[i + j] != '\0'; j++) {
            if (s[j] != sub[i +j]) {
                break;
            }
        }
        if (s[j] == '\0') {
            return i;
        }
    }
    return -1;
}
