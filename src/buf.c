

#include <stdlib.h>
#include "s21_decimal.h"
#include "s21_arrays.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>

void Foo() {
    decimal a = {1, 0, 0, 0};
    setDecimalExp(&a, 0);
    decimal b = {1, 0, 0, 0};
    setDecimalExp(&b, 2);
    decimal r = {0, 0, 0, 0};
    time_t t = clock();

    s21_div(a, b, &r);

    time_t t2 = clock();
    printf("%lf\n", (double) (t2 - t) / CLOCKS_PER_SEC);

    printBits(16, &r, 4);
    printf("exp - %d\n", getDecimalExp(r));
    printf("val - %u\n", r.bits[0]);
}
void check() {
    uint32_t a[2] = {150000, 0};
    int scale = 20;
    printBits(8, &a, 8);
    reduce_scale_arr(a, 2, &scale);
    printBits(8, &a, 8);
    printf("%d\n", scale);
}
void Bar() {
    printf("%f", powf(2,95)-1);

}
void leaks(){
    decimal a = {1,0,0,0};
    decimal b = {1,0,0,0};
    decimal r = {0,0,0,0};
    s21_add(a,b,&r);
    s21_sub(a,b,&r);
    s21_mul(a,b,&r);
    s21_div(a,b,&r);
    s21_mod(a,b,&r);
    s21_truncate(a,&r);
    s21_floor(a,&r);
    s21_round(a,&r);
    s21_negate(a,&r);

}
void test(){
    //float f = 18446744073709551616.0f;
    float f = 18446744073709551616.0f*16;
    float check;
    decimal res;
    s21_from_float_to_decimal(f, &res);
    s21_from_decimal_to_float(res, &check);
    printf("%20f\t%20f\n", f, check);
    printBits(16,&res,12);
    printBits(4,&f,4);
    printBits(4,&check,4);
}
int main() {
    test();
}
