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
    float f = MAXFLOAT / 2;
    //for (int i = 0; i < 100; ++i) {
    printf("%f\n", f);
    printBits(4, &f, 4);
    f *= 2;
    printf("%f\n", f);
    printBits(4, &f, 4);
    f *= 2;
    printf("%f\n", f);
    printBits(4, &f, 4);
    //}
    float e = INFINITY;
    printf("%d\n", f > e);
    printf("%d\n", f < e);
}

void sizes(){
    double d =DBL_MAX;
    long  double dd = LDBL_MAX;
    __uint128_t lli = (ULLONG_MAX<<32)+ULLONG_MAX;
    printf("%lu\t%lu\n", sizeof(d), sizeof(dd));
    //printBits(sizeof(d),&d,4);
    //printBits(sizeof(dd),&dd,4);
    printBits(sizeof(lli),&lli,4);

}
typedef union {
  decimal d;
  __uint128_t i;
}dcml;

void check(){
    dcml dc;
    dc.i = LONG_LONG_MAX*(1u<<31) +INT32_MAX;
    dc.i>>=1;
    printBits(16,&dc,4);
}

void t_ds() {
    double ff = 1.0/28;
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

int main() {
    check();
    //sizes();
    //t_ds();
    //Foo();
    //t_f();
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
