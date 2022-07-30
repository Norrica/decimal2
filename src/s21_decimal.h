//
// Created by Gladis Ariane on 7/3/22.
//

#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdint.h>
#include <stdlib.h>

#define signMask INT32_MIN
/*#define signMaskPos 0*/

#define OK 0
#define CE 1  // Conversion Error

typedef struct s21_decimal {
    int bits[4];
} s21_decimal;

typedef struct bits1 {
    uint32_t bits0;
    uint32_t bits1;
    uint32_t bits2;
} bits32;

typedef struct bits2 {
    uint64_t bits0;
    uint32_t bits1;
} bits64_32;

typedef struct bits3 {
    uint32_t bits0;
    uint64_t bits1;
} bits32_64;

typedef union {
    uint32_t bits[3];
    s21_decimal d;
    bits32 b32;
    bits32_64 b32_64;
    bits64_32 b64_32;
} ultimate;

typedef union {
    float f;
    int i;
    struct {
        uint32_t mantisa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    } parts;
} float_cast;

#define decimal s21_decimal

void printDecimalValue(s21_decimal d);

void printBits(const size_t size, const void *ptr, int sep_n);
uint32_t getBits(const void *ptr, int offset, int n);
void setBits(const void *dest, uint32_t bits, int offset, int n);
void copyBits(const void *dest, const void *src, int start, int end);

uint64_t getBits64(const void *ptr, int offset, int n);
void copyBits64(const void *dest, const void *src, int offset, int n);
void setBits64(const void *dest, uint64_t bits, int offset, int n);

void flipBits(uint32_t *i);
int shiftl(void *object, size_t size, int n);
int shiftr(void *object, size_t size, int n);
int is_0(void *arr, size_t size);
void bit_add(void *value_1, uint32_t number, size_t arr_size);
void OR(void *arr1, void *arr2, void *res, size_t size);
void XOR(void *arr1, void *arr2, void *res, size_t size);
void AND(void *arr1, void *arr2, void *res, size_t size);
void NOT(void *arr, void *res, size_t size);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

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

int s21_floor(s21_decimal value, s21_decimal *result);     // 0.5 -> 0
int s21_round(s21_decimal value, s21_decimal *result);     // 0.5 -> 0, 0.6 ->1
int s21_truncate(s21_decimal value, s21_decimal *result);  // 0.5 -> 1
int s21_negate(s21_decimal value, s21_decimal *result);    // 0.5 -> -0.5

#endif  // SRC_S21_DECIMAL_H_