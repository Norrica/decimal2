//
// Created by Gladis Ariane on 7/30/22.
//

#include <stdlib.h>
#include "s21_decimal.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

int main() {
    int a = -39;
    int b = -7;
    int r = 0;
    decimal aa;
    decimal bb;
    decimal rr;
    //init_0(&rr.bits,4);
    //printBits(sizeof(decimal),&rr,4);
    //setDecimalExp(&rr,0b11111111);
    //printBits(sizeof(decimal),&rr,4);
    //int exp = getDecimalExp(rr);
    //printBits(4,&exp,4);
    s21_from_int_to_decimal(a, &aa);
    s21_from_decimal_to_int(aa, &a);
    s21_from_int_to_decimal(b, &bb);
    s21_from_decimal_to_int(bb, &b);
    s21_sub(aa,bb,&rr);


    printDecimal(&aa);
    printDecimal(&bb);
    printDecimal(&rr);
    s21_from_decimal_to_int(rr, &r);
    printf("a - %d\n",a);
    printf("b - %d\n",b);
    printf("r - %d\n",r);
}