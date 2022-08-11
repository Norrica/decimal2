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

void tofromint() {
    decimal d;
    int check;
    for (int i = INT32_MIN; i <= INT32_MAX; i += 1000) {
        s21_from_int_to_decimal(i, &d);
        s21_from_decimal_to_int(d, &check);
        if (check != i) {
            printf("%d != %d\n", check, i);
        }
    }
}

void Bar() {
    s21_decimal test1 = {{1, 0, 0, 0}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF-1, 0xFFFFFFFF, 1u << 31}};
    s21_decimal result;

    //init_0((uint32_t *) test1.bits, 3);
    //init_0((uint32_t *) test2.bits, 3);
    //test1.bits[0] = 1;
    //test1.bits[1] = 0;
    //test1.bits[2] = 0;
    //setDecimalSign(&test1,1);
    //
    //test2.bits[1] = 1;
    //test2.bits[2] = 1;
    //test2.bits[3] = 0;
    //setDecimalSign(&test2,1);
    //test2.bits[3] = 0;
    //test1.bits[3] = 0;
    printDecimalValue(test1);
    printDecimalValue(test2);
    printBits(16, &test1, 4);
    printBits(16, &test2, 4);
    s21_add(test1, test2, &result);
    printDecimalValue(result);
    printBits(16, &result, 4);
}

void Baz(){
    int s = -1;
    uint32_t u = UINT32_MAX;
    printf("%d\n", s==u);
    //printf("%d\n", s);
    //printf("%u\n", u);
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
