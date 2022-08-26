

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include "s21_decimal.h"

unsigned int fun1(unsigned int a, unsigned int b) {
    unsigned int c;
    c = 0;
    while (a > b) {
        a -= b;
        c++;
    }
    return (c);
}
unsigned int fun2(unsigned int a, unsigned int b) {
    unsigned int res;
    unsigned int acc;
    unsigned int rb;

    res = 0;
    acc = 0;
    for (rb = 0x80000000; rb; rb >>= 1) {
        acc <<= 1;
        if (rb & a)
            acc |= 1;
        if (acc >= b) {
            acc -= b;
            res |= rb;
        }
    }
    return (res);
}

int main(void) {
    uint64_t a = 10000000000000000000;
    printBits(8, &a, 4);
    return 0;
}







