

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
    uint32_t a[5] = {0,10,0,0,10};
    printf("%d\n", is_0(&a[3],2));

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
    //float f = 81446744073709551616.0f;
    float f = 1844674407370955161628f;
    float check;
    decimal res;
    s21_from_float_to_decimal(f, &res);
    s21_from_decimal_to_float(res, &check);
    printf("%20f\t%20f\n", f, check);
    printBits(16,&res,12);
    printBits(4,&f,4);
    printBits(4,&check,4);
}

void br(){
    int scale = 2;
    uint32_t a[4] = {150,0,0,0};
    printBits(8,a,4);
    bank_round_arr(a,&scale,4);
    printBits(8,a,4);
    printf("%d\n",scale);

}
void speed(){
    uint32_t a[10] = {UINT32_MAX,0};
    uint32_t b[10] = {UINT32_MAX,0};
    clock_t st = clock();
    div10(a,10);
    clock_t end = clock();
    double time_spent = (double)(end - st) / CLOCKS_PER_SEC;
    printf("%lf\n",time_spent);
    st = clock();
    div10(a,10);
    end = clock();
    time_spent = (double)(end - st) / CLOCKS_PER_SEC;
    printf("%lf\n",time_spent);
}

void foo(){
    decimal a = {100, 0, 0, 0};
    decimal b = {50, 0, 0, 0};
    //setDecimalExp(&a,28);
    decimal r = {0, 0, 0, 0};
    int err = s21_div(a, b, &r);
    printBits(16,&r,12);
    printf("%d\n",err);

}

int main() {
    test();
    //br();
}
