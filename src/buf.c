//
// Created by Gladis Ariane on 7/30/22.
//

#include <stdlib.h>
#include "s21_decimal.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

void Foo();

unsigned long divu10(unsigned long n) {
    unsigned long q, r;
    q = (n >> 1) + (n >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q >> 3;
    r = n - (((q << 2) + q) << 1);
    return q + (r > 9);
}

int main() {

    //uint32_t a[3] = {473,0xF000000F,473};
    //printBits(12,a,4);
    //shiftl1(a,3);
    //printBits(12,a,4);
    0.05
    10
    50 /10**0
    Foo();
}

void Foo() {
    int a = 101;
    int b = 89;
    int r = 0;
    decimal aa;
    decimal bb;
    decimal rr;

    s21_from_int_to_decimal(a, &aa);
    //s21_from_decimal_to_int(aa, &a);
    s21_from_int_to_decimal(b, &bb);
    //s21_from_decimal_to_int(bb, &b);
    s21_mul(aa, bb, &rr);

    printDecimal(&aa);
    printDecimal(&bb);
    printDecimal(&rr);
    s21_from_decimal_to_int(rr, &r);
    printf("a - %d\n", a);
    printf("b - %d\n", b);
    printf("r - %d\n", r);
}
