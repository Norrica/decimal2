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

void tofromint(){
    decimal d;
    int check;
    for (int i = INT32_MIN; i <= INT32_MAX; i+=1000) {
        s21_from_int_to_decimal(i, &d);
        s21_from_decimal_to_int(d, &check);
        if (check != i) {
            printf("%d != %d\n", check, i);
        }
    }
}

void Bar(){
    int i = INT32_MAX;
    printBits(4,&i,4);
    decimal d;
    int check;
    s21_from_int_to_decimal(i, &d);
    s21_from_decimal_to_int(d, &check);
    if (check != i) {
        printf("%d != %d\n", check, i);
    }
}

int main() {
    //check();
    //sizes();
    //t_ds();
    //tofromint();
    Bar();
    //Foo();
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
