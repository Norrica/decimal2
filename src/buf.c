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

void t_f() {
    float f = MAXFLOAT;
    uint64_t ui = UINT64_MAX;
    printf("%f\n", f);
    printf("%llu\n", ui);
}

void sizes() {
    double d = DBL_MAX;
    long double dd = LDBL_MAX;
    __uint128_t lli = (ULLONG_MAX << 32) + ULLONG_MAX;
    printf("%lu\t%lu\n", sizeof(d), sizeof(dd));
    //printBits(sizeof(d),&d,4);
    //printBits(sizeof(dd),&dd,4);
    printBits(sizeof(lli), &lli, 4);
}
typedef union {
  decimal d;
  __uint128_t i;
} dcml;

void check() {
    dcml dc;
    dc.i = LONG_LONG_MAX * (1u << 31) + INT32_MAX;
    dc.i >>= 1;
    printBits(16, &dc, 4);
}

void t_ds() {
    double ff = 1.0 / 28;
    printf("%.20lf\n", ff);

    decimal d = {UINT32_MAX, UINT32_MAX >> 22, 0, 1u << 31};
    printBits(16, &d, 4);
    setDecimalExp(&d, 0);
    //printBits(16,&d,4);
    //printf("%d\n", getDecimalSign(d));
    float f = 0;
    s21_from_decimal_to_float(d, &f);
    printf("%.20f", f);
}

void test() {
    uint32_t i = 1u << 31;
    decimal d = {{0,0,0,i}};
    printDecimal(&d);
    setDecimalExp(&d,UINT32_MAX);
    printDecimal(&d);
}

int main() {
    //check();
    //sizes();
    //t_ds();
    Foo();
    //t_f();
    //test();
}

void Foo() {
    float a = 10.0039f;
    float b = -0.0020f;
    float r = 0;
    decimal aa;
    decimal bb;
    decimal rr;
    float ar = 0;
    float br = 0;

    s21_from_float_to_decimal(a, &aa);
    s21_from_decimal_to_float(aa, &ar);
    s21_from_float_to_decimal(b, &bb);
    s21_from_decimal_to_float(bb, &br);
    s21_sub(aa, bb, &rr);

    printDecimal(&aa);
    printDecimal(&bb);
    printDecimal(&rr);
    s21_from_decimal_to_float(rr, &r);
    puts("a:");
    printf("%f\n%f\n", a, ar);
    puts("b:");
    printf("%f\n%f\n", b, br);
    puts("res:");
    printf("r - %f\n", r);
}
