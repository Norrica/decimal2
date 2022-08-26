

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
    setDecimalExp(&a, 1);
    decimal b = {2, 0, 0, 0};
    setDecimalExp(&b, 1);
    decimal r = {0, 0, 0, 0};
    time_t t = clock();

    s21_mod(a, b, &r);

    time_t t2 = clock();
    printf("%lf\n", (double) (t2 - t) / CLOCKS_PER_SEC);

    printBits(16, &r, 4);
    printf("%d\n", getDecimalExp(r));
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
    uint32_t a = UINT32_MAX - 1;
    printf("%u\n", a);
    printBits(4, &a, 8);
    a /= 2;
    printf("%u\n", a);
    printBits(4, &a, 8);
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
int main() {
    leaks();
}
