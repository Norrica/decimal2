//
// Created by Gladis Ariane on 8/26/22.
//

#include "s21_arrays.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"

void printBits(const size_t size, const void *ptr,
               int sep_n) { /*Поможет смотреть внутрь массива*/
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
    if (!(i % sep_n)) printf(" ");
  }
  puts("");
}

void printDec_an(s21_decimal dec) {
  for (int i = 128; i > 96; i--) printf("%d", s21_get_bit(dec, i));
  printf(" ");
  for (int i = 96; i > -1; i--) printf("%d", s21_get_bit(dec, i));
}

int s21_get_bit(s21_decimal d, int i) {
  int bit = 0;
  unsigned int mask = 1u << (i % 32);
  if ((d.bits[i / 32] & mask) != 0) bit = 1;
  return bit;
}

void s21_set_bit(s21_decimal *value, int place, int bit) {
  unsigned int mask = 1u << (place % 32);
  if (bit) {
    value->bits[place / 32] |= mask;
  } else {
    value->bits[place / 32] &= ~mask;
  }
}

uint32_t getBits(const void *ptr, int offset, int n) {
  uint32_t num = *(uint32_t *)ptr;
  uint32_t buf = num;
  uint32_t mask = 0xFFFFFFFF >> (32 - n);
  mask <<= offset;
  buf &= mask;
  return buf >> offset;
}

void copyBits(const void *dest, const void *src, int offset, int n) {
  uint32_t src_num = getBits(src, offset, n);
  setBits(dest, src_num, offset, n);
}

void setBits(const void *dest, uint32_t bits, int offset, int n) {
  uint32_t mask = 0xFFFFFFFF >> (32 - n);
  mask <<= offset;
  bits <<= offset;
  bits &= mask;
  *(uint32_t *)dest &= ~mask;
  *(uint32_t *)dest |= bits;
}

void flipBits(uint32_t *i) {
  uint32_t j = 0;
  for (int k = 0; k < 32; k++) j |= ((*i >> k) & 0b1) << (31 - k);
  *i = j;
}

int shiftl(void *object, size_t size, int n) {
  if ((size_t)n > 32 * size) {
    puts("dont shift more than arr size");
    return 1;
  }
  for (int i = 0; i < n; ++i) shiftl1(object, size);
  return 0;
}

int shiftr(void *object, size_t size, int n) {
  if ((size_t)n > (size * 32)) {
    puts("fuk you from shiftr");
    return 1;
  }
  for (int i = 0; i < n; ++i) {
    shiftr1(object, size);
  }
  return 0;
}

void shiftl1(uint32_t *arr, size_t size) {
  for (size_t i = size - 1; i >= 1; --i) {
    arr[i] <<= 1;
    arr[i] += (arr[i - 1] & (1u << 31)) >> 31;
  }
  arr[0] <<= 1;
}

void shiftr1(uint32_t *arr, size_t size) {
  for (size_t i = 0; i < size - 1; ++i) {
    arr[i] >>= 1;
    arr[i] += (arr[i + 1] & (1u)) << 31;
  }
  arr[size - 1] >>= 1;
}

void OR(void *arr1, void *arr2, void *res, size_t size) {
  uint32_t *a1 = (uint32_t *)arr1;
  uint32_t *a2 = (uint32_t *)arr2;
  uint32_t *r = (uint32_t *)res;
  for (size_t i = 0; i < size; i++) r[i] = a1[i] | a2[i];
}

void XOR(void *arr1, void *arr2, void *res, size_t size) {
  uint32_t *a1 = (uint32_t *)arr1;
  uint32_t *a2 = (uint32_t *)arr2;
  uint32_t *r = (uint32_t *)res;
  for (size_t i = 0; i < size; i++) r[i] = a1[i] ^ a2[i];
}

void AND(void *arr1, void *arr2, void *res, size_t size) {
  uint32_t *a1 = (uint32_t *)arr1;
  uint32_t *a2 = (uint32_t *)arr2;
  uint32_t *r = (uint32_t *)res;
  for (size_t i = 0; i < size; i++) {
    r[i] = a1[i] & a2[i];
  }
}

void NOT(void *arr, void *res, size_t size) {
  uint32_t *a = (uint32_t *)arr;
  uint32_t *r = (uint32_t *)res;
  for (size_t i = 0; i < size; i++) r[i] = ~a[i];
}

int is_0(void *arr, size_t size) {
  uint32_t *a = (uint32_t *)arr;
  for (size_t i = 0; i < size; i++) {
    if (a[i] > 0) return 0;
  }
  return 1;
}

void bit_add(void *value_1, uint32_t number, size_t arr_size) {
  uint32_t *x = (uint32_t *)value_1;
  uint32_t *y = calloc(arr_size, sizeof(uint32_t));
  y[0] = number;
  uint32_t *sum = calloc(arr_size, sizeof(uint32_t));
  uint32_t *carry = calloc(arr_size, sizeof(uint32_t));
  XOR(x, y, sum, arr_size);
  AND(x, y, carry, arr_size);
  while (!is_0(carry, arr_size)) {
    shiftl(carry, arr_size, 1);
    for (size_t i = 0; i < arr_size; i++) {
      x[i] = sum[i];
      y[i] = carry[i];
    }
    XOR(x, y, sum, arr_size);
    AND(x, y, carry, arr_size);
  }
  for (size_t i = 0; i < arr_size; i++) {
    x[i] = sum[i];
  }
  free(carry);
  free(sum);
  free(y);
}

void bit_add_arr(void *res_arr, void *number, size_t arr_size) {
  uint32_t *x = (uint32_t *)res_arr;
  uint32_t *y = (uint32_t *)calloc(arr_size, sizeof(uint32_t));
  copyArray(number, y, arr_size);
  uint32_t *sum = (uint32_t *)calloc(arr_size, sizeof(uint32_t));
  uint32_t *carry = (uint32_t *)calloc(arr_size, sizeof(uint32_t));

  XOR(x, y, sum, arr_size);
  AND(x, y, carry, arr_size);
  while (!is_0(carry, arr_size)) {
    shiftl(carry, arr_size, 1);
    for (size_t i = 0; i < arr_size; i++) {
      x[i] = sum[i];
      y[i] = carry[i];
    }
    XOR(x, y, sum, arr_size);
    AND(x, y, carry, arr_size);
  }
  for (size_t i = 0; i < arr_size; i++) {
    x[i] = sum[i];
  }
  free(y);
  free(carry);
  free(sum);
}

int cmp(const uint32_t *a, const uint32_t *b, size_t size) {
  //  1 - >
  //  0 - ==
  //  -1 - <
  for (int i = size - 1; i >= 0; i--) {
    if (a[i] == b[i] && i > 0) continue;
    if (a[i] > b[i]) return 1;
    if (a[i] < b[i]) return -1;
  }
  return 0;
}

void bit_sub_arr(uint32_t *res_arr, uint32_t *number, size_t arr_size) {
  uint32_t *x = (uint32_t *)res_arr;
  uint32_t *y = (uint32_t *)calloc(arr_size, sizeof(uint32_t));
  copyArray(number, y, arr_size);
  uint32_t *borrow = (uint32_t *)calloc(arr_size, sizeof(uint32_t));
  uint32_t *tmp = (uint32_t *)calloc(arr_size, sizeof(uint32_t));
  while (!is_0(y, arr_size)) {
    //  https:// iq.opengenus.org/bitwise-subtraction/
    NOT(x, tmp, arr_size);
    AND(tmp, y, borrow, arr_size);
    XOR(x, y, x, arr_size);
    copyArray(borrow, y, arr_size);
    shiftl(y, arr_size, 1);
  }
  free(y);
  free(borrow);
  free(tmp);
}

void bit_mul_arr(uint32_t *val1, uint32_t *val2, uint32_t *res, size_t size) {
  uint32_t *a1 = calloc(size, sizeof(uint32_t));
  copyArray(val1, a1, size);
  uint32_t *a2 = calloc(size, sizeof(uint32_t));
  copyArray(val2, a2, size);
  while (!is_0(a2, size)) {
    if (getBits(a2, 0, 1)) {
      bit_add_arr(res, a1, size);
    }
    shiftl1(a1, size);
    shiftr1(a2, size);
  }
  free(a1);
  free(a2);
}

void bit_div_arr(uint32_t *arr1, uint32_t *arr2, uint32_t *res, size_t size) {
  uint32_t *a1 = calloc(size, sizeof(uint32_t));
  copyArray(arr1, a1, size);
  uint32_t *a2 = calloc(size, sizeof(uint32_t));
  copyArray(arr2, a2, size);
  uint32_t *acc = calloc(size, sizeof(uint32_t));
  uint32_t *rb = calloc(size, sizeof(uint32_t));
  uint32_t *buf = calloc(size, sizeof(uint32_t));
  uint32_t *one = calloc(size, sizeof(uint32_t));
  one[0] = 1;
  init_0(res, size);
  setBits(&(rb[size - 1]), 1, 31, 1); /*rb = 0x80000000*/
  while (!is_0(rb, size)) {           /*for*/
    shiftl1(acc, size);
    AND(rb, a1, buf, size);
    if (!is_0(buf, size)) OR(acc, one, acc, size);
    int cmp_res = cmp(acc, a2, size);
    if (cmp_res == 1 || cmp_res == 0) {
      bit_sub_arr(acc, a2, size);
      OR(res, rb, res, size);
    }
    shiftr1(rb, size); /*rb >>= 1*/
  }
  free(a2);
  free(acc);
  free(rb);
  free(buf);
  free(one);
}

void bit_mod_arr(uint32_t *arr1, uint32_t *arr2, uint32_t *res, size_t size) {
  uint32_t *div = calloc(size, sizeof(uint32_t));
  uint32_t *mul = calloc(size, sizeof(uint32_t));
  uint32_t *a1 = calloc(size, sizeof(uint32_t));
  copyArray(arr1, a1, size);
  uint32_t *a2 = calloc(size, sizeof(uint32_t));
  copyArray(arr2, a2, size);

  bit_div_arr(a1, a2, div, size);
  bit_mul_arr(a2, div, mul, size);
  bit_sub_arr(a1, mul, size);
  copyArray(a1, res, size);

  free(div);
  free(mul);
  free(a1);
  free(a2);
}

void bit_div_mod_arr(uint32_t *arr1, uint32_t *arr2, uint32_t *div,
                     uint32_t *mod, size_t size) {
  uint32_t *mul = calloc(size, sizeof(uint32_t));
  uint32_t *a1 = calloc(size, sizeof(uint32_t));
  copyArray(arr1, a1, size);
  uint32_t *a2 = calloc(size, sizeof(uint32_t));
  copyArray(arr2, a2, size);

  bit_div_arr(a1, a2, div, size);
  bit_mul_arr(a2, div, mul, size);
  bit_sub_arr(a1, mul, size);
  copyArray(a1, mod, size);
  free(mul);
  free(a1);
  free(a2);
}

void bit_sub(uint32_t *res_arr, uint32_t number, size_t arr_size) {
  uint32_t *tmp = calloc(arr_size, sizeof(uint32_t));
  tmp[0] = number;
  bit_sub_arr(res_arr, tmp, arr_size);
  free(tmp);
}

void bit_mod(uint32_t *arr1, uint32_t number, uint32_t *res, size_t size) {
  uint32_t *tmp = calloc(size, sizeof(uint32_t));
  tmp[0] = number;
  bit_mod_arr(arr1, tmp, res, size);
  free(tmp);
}

void bit_div_mod(uint32_t *arr1, uint32_t number, uint32_t *div, uint32_t *mod,
                 size_t size) {
  uint32_t *tmp = calloc(size, sizeof(uint32_t));
  tmp[0] = number;
  bit_div_mod_arr(arr1, tmp, div, mod, size);
  free(tmp);
}

void bit_mul(uint32_t *val1, uint32_t number, uint32_t *res, size_t size) {
  uint32_t *tmp = calloc(size, sizeof(uint32_t));
  tmp[0] = number;
  bit_mul_arr(val1, tmp, res, size);
  free(tmp);
}

void bit_div(uint32_t *arr1, uint32_t number, uint32_t *res, size_t size) {
  uint32_t *tmp = calloc(size, sizeof(uint32_t));
  tmp[0] = number;
  bit_div_arr(arr1, tmp, res, size);
  free(tmp);
}

void init_0(uint32_t *arr, int size) {
  for (int i = 0; i < size; ++i) {
    arr[i] = 0;
  }
}

void move_scale_arr(int cycles, uint32_t *arr, size_t size) {
  for (int i = 0; i < cycles; ++i) {
    mul10(arr, size);
  }
}

int reduce_scale_arr(uint32_t *arr, size_t size, int *scale) {
  uint32_t *buf = calloc(size, sizeof(uint32_t));
  copyArray(arr, buf, size);
  while (*scale > 0) {
    bit_div(arr, 10, buf, size);
    mul10(buf, size);
    if (cmp(arr, buf, size) == 0) {
      bit_div(buf, 10, arr, size);
      (*scale)--;
    } else {
      break;
    }
  }
  free(buf);
  return OK;
}

int eq_scale_arr(uint32_t *x, uint32_t *y, int scalex, int scaley,
                 size_t size) {
  int maxscale;
  if (scalex > scaley) {
    maxscale = scalex;
    move_scale_arr(scalex - scaley, y, size);
  } else if (scaley > scalex) {
    maxscale = scaley;
    move_scale_arr(scaley - scalex, x, size);
  } else {
    maxscale = scalex;
  }
  return maxscale;
}

int bank_round_arr(uint32_t *arr, int *scale, size_t size) {
  uint32_t *buf = calloc(size, sizeof(uint32_t));
  uint32_t *five = calloc(size, sizeof(uint32_t));
  uint32_t *div = calloc(size, sizeof(uint32_t));
  uint32_t *mod = calloc(size, sizeof(uint32_t));
  five[0] = 5;
  copyArray(arr, buf, size);
  while (*scale > 29) {
    bit_div(buf, 10, buf, size);
    (*scale)--;
  }
  bit_div_mod(buf, 10, buf, mod, size);
  if (cmp(mod, five, size) > 0) {
    bit_add(buf, 1, size);
  } else if (cmp(mod, five, size) == 0) {
    if (mod[0] & 1) {
      bit_add(buf, 1, size);
    }
  }

  int ret = 0;
  if (!is_0(&buf[3], size - 3)) {
    ret = TOOLARGE;
  } else {
    copyArray(buf, arr, size);
    (*scale)--;
  }
  free(buf);
  free(five);
  free(div);
  free(mod);
  return ret;
}

void copyArray(const uint32_t *from, uint32_t *to, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    to[i] = from[i];
  }
}

void mul10(uint32_t *x, int size) {
  uint32_t *tmp = malloc(size * sizeof(uint32_t));
  copyArray(x, tmp, size);
  shiftl(x, size, 3);
  shiftl(tmp, size, 1);
  bit_add_arr(x, tmp, size);
  free(tmp);
}

void div10(uint32_t *x, size_t size) { bit_div(x, 10, x, size); }

void div10ret(uint32_t *x, uint32_t *res, size_t size) {
  uint32_t tmp[size];
  copyArray(x, tmp, size);
  div10(tmp, size);
  copyArray(tmp, res, size);
}

void div_mod10(uint32_t *x, size_t size, int *exp) {
  uint32_t tmp[size], tmp2[size];
  init_0(tmp, (int)size);
  init_0(tmp2, (int)size);
  // int mod = 0;
  if (!is_0(x + 3, size - 3) &&
      *exp > 0) {  // Обрубаем скейл еще(до 28) и если число не влезает в
                   // десимал после этого, отрубаем скейл дальше
    for (int i = 0; !is_0(x + 3, size - 3) && *exp > 0; i++) {
      copyArray(x, tmp, size);
      div10(x, size);
      *exp -= 1;
    }
    copyArray(tmp, x, size);
    *exp += 1;
    bank_round_arr(x, exp, size);
  } else if (*exp == 29) {
    bank_round_arr(x, exp, size);
  }
}