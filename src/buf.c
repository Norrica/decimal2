

#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "s21_arrays.h"
#include "s21_decimal.h"

void Foo() {
  decimal a = {1, 0, 0, 0};
  setDecimalExp(&a, 0);
  decimal b = {1, 0, 0, 0};
  setDecimalExp(&b, 2);
  decimal r = {0, 0, 0, 0};
  time_t t = clock();

  s21_div(a, b, &r);

  time_t t2 = clock();
  printf("%lf\n", (double)(t2 - t) / CLOCKS_PER_SEC);

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
  uint32_t a[5] = {0, 10, 0, 0, 10};
  printf("%d\n", is_0(&a[3], 2));
}
void leaks() {
  decimal a = {1, 0, 0, 0};
  decimal b = {1, 0, 0, 0};
  decimal r = {0, 0, 0, 0};
  s21_add(a, b, &r);
  s21_sub(a, b, &r);
  s21_mul(a, b, &r);
  s21_div(a, b, &r);
  s21_mod(a, b, &r);
  s21_truncate(a, &r);
  s21_floor(a, &r);
  s21_round(a, &r);
  s21_negate(a, &r);
}
void test() {
  // float f = 18446744073709551616.0f;
  float f = 0.9f;
  float check = 0;
  decimal res;
  time_t t = clock();

  int err = s21_from_float_to_decimal(f, &res);

  time_t t2 = clock();
  printf("%lf\n", (double)(t2 - t) / CLOCKS_PER_SEC);
  printf("%d\n", err);
  printBits(16, &res, 12);

  err = s21_from_decimal_to_float(res, &check);
  printf("%d\n", err);

  printf("%20f\t%20f\n", f, check);
  // printf("%20f\n", f- check);
  // printBits(16,&res,12);
  // printBits(4,&f,4);
  // printBits(4,&check,4);
}

void speed() {
  uint32_t a[10] = {UINT32_MAX, 0};
  uint32_t b[10] = {UINT32_MAX, 0};
  clock_t st = clock();
  div10(a, 10);
  clock_t end = clock();
  double time_spent = (double)(end - st) / CLOCKS_PER_SEC;
  printf("%lf\n", time_spent);
  st = clock();
  div10(a, 10);
  end = clock();
  time_spent = (double)(end - st) / CLOCKS_PER_SEC;
  printf("%lf\n", time_spent);
}

void foo() {
  decimal a = {100, 0, 0, 0};
  decimal b = {50, 0, 0, 0};
  // setDecimalExp(&a,28);
  decimal r = {0, 0, 0, 0};
  int err = s21_div(a, b, &r);
  printBits(16, &r, 12);
  printf("%d\n", err);
}
#include <math.h>  // for testing only

typedef union {
  float value;
  unsigned int bits;  // assuming 32 bit large ints (better: uint32_t)
} ieee_754_float;

// clang -g3 -O3 -W -Wall -Wextra -Wpedantic -Weverything -std=c11 -o
// testthewest testthewest.c -lm
void ttttt() {
  unsigned int m, num;
  int exp;  // the exponent can be negative
  float n, mantissa;
  ieee_754_float uf;

  // neither checks nor balances included!

  n = 1.5f;

  uf.value = n;
  num = uf.bits;
  m = num &
      0x807fffff;  // extract mantissa (i.e.: get rid of sign bit and exponent)
  num = num & 0x7fffffff;   // full number without sign bit
  exp = (num >> 23) - 126;  // extract exponent and subtract bias
  m |= 0x3f000000;          // normalize mantissa (add bias)
  uf.bits = m;
  mantissa = uf.value;
  printf("n = %g, mantissa = %g, exp = %d, check %g\n", n, mantissa, exp,
         mantissa * powf(2, exp));
}

void afof() {
  char ch[100];
  float f = 123456789.f;
  snprintf(ch, sizeof(ch), "%.6f", f);
  float res = atof(ch);
  puts(ch);
  printBits(4, &f, 16);
  printBits(4, &res, 16);
  printf("%.10f\n", f);
  printf("%.10f\n", res);
}

int main() { test(); }
