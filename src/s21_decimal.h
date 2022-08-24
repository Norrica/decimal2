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

//#ifndef uint32_t
//#define uint32_t unsigned int
//#endif

#define signMask INT32_MIN
/*#define signMaskPos 0*/

#define OK 0
#define CE 1  // Conversion Error

#define TOOLARGE 1
#define TOOSMALL 2
#define DIVBY0 3
typedef union {
  uint32_t bits[4];
  struct {
    uint32_t mntsa[3];  // 96bit
    uint32_t zeros : 16;
    uint32_t exp : 8;  // mntsa*10^-exp
    uint32_t zeros2 : 7;
    uint32_t sign : 1;
  };
} s21_decimal;

#define decimal s21_decimal
#define printDecimal(D) (printBits(sizeof(int) * 4, D,4))
int getDecimalSign(decimal d);

int getDecimalExp(decimal d);

void setDecimalExp(decimal *d, int exp);

void setDecimalSign(decimal *d, int sign);

void init_0(uint32_t *arr, int size);

void printDecimalValue(s21_decimal d);

void printBits(size_t size, const void *ptr, int sep_n);

uint32_t getBits(const void *ptr, int offset, int n);

void setBits(const void *dest, uint32_t bits, int offset, int n);

void copyBits(const void *dest, const void *src, int start, int end);

void mul10(uint32_t *x, int size);

void div10(uint32_t *x, size_t size);

void copyArray(uint32_t *from, uint32_t *to, size_t len);

int move_scale(int cycles, s21_decimal *num);

void move_scale_arr(int cycles, uint32_t *arr, size_t size);

int eq_scale(decimal *x, decimal *y);

int reduce_scale(decimal *x);

int reduce_scale_arr(uint32_t *arr, size_t size, int *scale);

int eq_scale_arr(uint32_t *x, uint32_t *y, int scalex, int scaley, size_t size);

void flipBits(uint32_t *i);

int shiftl(void *object, size_t size, int n);

int shiftr(void *object, size_t size, int n);

void shiftl1(uint32_t *object, size_t size);

void shiftr1(uint32_t *object, size_t size);

int is_0(void *arr, size_t size);

int cmp(uint32_t *a, uint32_t *b, size_t size);

void bit_add(void *value_1, uint32_t number, size_t arr_size);

void bit_add_arr(void *res_arr, void *number, size_t arr_size);

void bit_sub_arr(uint32_t *res_arr, uint32_t *number, size_t arr_size);

void bit_mul_arr(uint32_t *val1, uint32_t *val2, uint32_t *res, size_t size);

void bit_div_arr(uint32_t *arr1, uint32_t *arr2, uint32_t *res, size_t size);

void OR(void *arr1, void *arr2, void *res, size_t size);

void XOR(void *arr1, void *arr2, void *res, size_t size);

void AND(void *arr1, void *arr2, void *res, size_t size);

void NOT(void *arr, void *res, size_t size);

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
