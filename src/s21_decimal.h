//
// Created by Gladis Ariane on 7/3/22.
//

#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// #ifndef uint32_t
// typedef unsigned int uint32_t;
// #endif


#define OK 0
#define CE 1  // Conversion Error

#define TOOLARGE 1
#define TOOSMALL 2
#define DIVBY0 3
// typedef union {
//   uint32_t bits[4];
//   struct {
//     uint32_t mntsa[3];  // 96bit
//     uint32_t zeros : 16;
//     uint32_t exp : 8;  // mntsa*10^-exp
//     uint32_t zeros2 : 7;
//     uint32_t sign : 1;
//   };
// } s21_decimal;

typedef struct {
  uint32_t bits[4];
} s21_decimal;

#define decimal s21_decimal
#define printDecimal(D) (printBits(sizeof(int) * 4, D, 4))

int getDecimalSign(decimal d);

int getDecimalExp(decimal d);

void setDecimalExp(decimal *d, int exp);

void setDecimalSign(decimal *d, int sign);

void printDecimalValue(s21_decimal d);

void printBits(size_t size, const void *ptr, int sep_n);

int move_scale(int cycles, s21_decimal *num);

int eq_scale(decimal *x, decimal *y);

int reduce_scale(decimal *x);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_bank_round(s21_decimal *value);

int s21_is_less(s21_decimal value_1, s21_decimal value_2);

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);

int s21_is_greater(s21_decimal value_1, s21_decimal value_2);

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);

int s21_is_equal(s21_decimal value_1, s21_decimal value_2);

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);

int s21_from_float_to_decimal(float src, s21_decimal *dst);

int s21_from_decimal_to_int(s21_decimal src, int *dst);

int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);

// 0.5 -> 0
int s21_round(s21_decimal value, s21_decimal *result);

// 0.4 -> 0, 0.6 ->1
int s21_truncate(s21_decimal value, s21_decimal *result);

// 0.5 -> 0
int s21_negate(s21_decimal value, s21_decimal *result);
// 0.5 -> -0.5

#endif  // SRC_S21_DECIMAL_H_
