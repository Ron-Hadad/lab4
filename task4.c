#include <stdio.h>

int digit_cnt(char* str) {
    int count = 0;
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            count++;
        }
        str++;
    }
    return count;
}

int main(int argc, char **argv) {
    printf("The number of digits in the string is:(new function) %d\n", digit_cnt(argv[1]));
    return 0;
}