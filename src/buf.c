//
// Created by Gladis Ariane on 7/30/22.
//

#include <stdlib.h>
#include "s21_decimal.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

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
    for (uint64_t i = 0; i < 1000; ++i) {


        //printf("%lu\t%lu\n", divu10(i) , i / 10);
        printf("%u\n", divu10(i) == i / 10);
    }

    //uint32_t x[7] = {0};
    //div10(x,7);
    //uint32_t a = 60;
    //a -= a >> 1;
    //a -= a >> 1;
    ////a -= a >> 1;
    //printf("%u\n", a);
    //a = a >> 1;
    //printf("%u\n", a);

    //printBits(4,&a,4);
    //shiftr1(&a,1);
    //printBits(4,&a,4);
    //int a = -39;
    //int b = -7;
    //int r = 0;
    //decimal aa;
    //decimal bb;
    //decimal rr;
    ////init_0(&rr.bits,4);
    ////printBits(sizeof(decimal),&rr,4);
    ////setDecimalExp(&rr,0b11111111);
    ////printBits(sizeof(decimal),&rr,4);
    ////int exp = getDecimalExp(rr);
    ////printBits(4,&exp,4);
    //s21_from_int_to_decimal(a, &aa);
    //s21_from_decimal_to_int(aa, &a);
    //s21_from_int_to_decimal(b, &bb);
    //s21_from_decimal_to_int(bb, &b);
    //s21_sub(aa,bb,&rr);
    //
    //
    //printDecimal(&aa);
    //printDecimal(&bb);
    //printDecimal(&rr);
    //s21_from_decimal_to_int(rr, &r);
    //printf("a - %d\n",a);
    //printf("b - %d\n",b);
    //printf("r - %d\n",r);
}