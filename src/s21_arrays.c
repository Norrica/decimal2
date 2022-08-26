//
// Created by Gladis Ariane on 8/26/22.
//

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "s21_decimal.h"
#include "s21_arrays.h"


/*Поможет смотреть внутрь массива*/
void printBits(const size_t size, const void *ptr, int sep_n) {
    unsigned char *b = (unsigned char *) ptr;
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
int s21_get_bit(s21_decimal d, int i) {
    int bit = 0;
    unsigned int mask = 1u << (i % 32);
    if ((d.bits[i / 32] & mask) != 0)
        bit = 1;
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
    uint32_t num = *(uint32_t *) ptr;
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
    bits &= mask;  //  Зануляет ненужные биты
    *(uint32_t *) dest &= ~mask;
    *(uint32_t *) dest |= bits;
}
void flipBits(uint32_t *i) {
    uint32_t j = 0;
    for (int k = 0; k < 32; k++) {
        j |= ((*i >> k) & 0b1) << (31 - k);
    }
    *i = j;
}
int shiftl(void *object, size_t size, int n) {
    if ((size_t) n > 32 * size) {
        puts("dont shift more than arr size");
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        shiftl1(object, size);
    }
    return 0;
}
int shiftr(void *object, size_t size, int n) {
    if ((size_t) n > (size * 32)) {
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
    uint32_t *a1 = (uint32_t *) arr1;
    uint32_t *a2 = (uint32_t *) arr2;
    uint32_t *r = (uint32_t *) res;
    for (size_t i = 0; i < size; i++) {
        r[i] = a1[i] | a2[i];
    }
}
void XOR(void *arr1, void *arr2, void *res, size_t size) {
    uint32_t *a1 = (uint32_t *) arr1;
    uint32_t *a2 = (uint32_t *) arr2;
    uint32_t *r = (uint32_t *) res;
    for (size_t i = 0; i < size; i++) {
        r[i] = a1[i] ^ a2[i];
    }
}
void AND(void *arr1, void *arr2, void *res, size_t size) {
    uint32_t *a1 = (uint32_t *) arr1;
    uint32_t *a2 = (uint32_t *) arr2;
    uint32_t *r = (uint32_t *) res;
    for (size_t i = 0; i < size; i++) {
        r[i] = a1[i] & a2[i];
    }
}
void NOT(void *arr, void *res, size_t size) {
    uint32_t *a = (uint32_t *) arr;
    uint32_t *r = (uint32_t *) res;
    for (size_t i = 0; i < size; i++) {
        r[i] = ~a[i];
    }
}
int is_0(void *arr, size_t size) {
    uint32_t *a = (uint32_t *) arr;
    for (size_t i = 0; i < size; i++) {
        if (a[i] > 0) {
            return 0;
        }
    }
    return 1;
}
void bit_add(void *value_1, uint32_t number, size_t arr_size) {
    uint32_t *x = (uint32_t *) value_1;
    uint32_t *y = malloc(sizeof(uint32_t) * arr_size);
    memset(y, 0, arr_size * sizeof(*y));
    y[0] = number;

    uint32_t *sum = malloc(sizeof(uint32_t) * arr_size);
    uint32_t *carry = malloc(sizeof(uint32_t) * arr_size);
    memset(sum, 0, arr_size * sizeof(*y));
    memset(carry, 0, arr_size * sizeof(*y));
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
        // printf("%d\n",is_0(carry, arr_size));
    }
    for (size_t i = 0; i < arr_size; i++) {
        x[i] = sum[i];
    }
    free(carry);
    free(sum);
    free(y);
}
void bit_add_arr(void *res_arr, void *number, size_t arr_size) {
    uint32_t *x = (uint32_t *) res_arr;
    uint32_t *y = (uint32_t *) calloc(arr_size, sizeof(uint32_t));
    copyArray(number, y, arr_size);
    uint32_t *sum = (uint32_t *) calloc(arr_size, sizeof(uint32_t));
    uint32_t *carry = (uint32_t *) calloc(arr_size, sizeof(uint32_t));

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
        // printf("%d\n",is_0(carry, arr_size));
    }
    for (size_t i = 0; i < arr_size; i++) {
        x[i] = sum[i];
    }
    free(y);
    free(carry);
    free(sum);
}
void bit_sub_arr(uint32_t *res_arr, uint32_t *number, size_t arr_size) {
    uint32_t *x = (uint32_t *) res_arr;
    uint32_t *y = (uint32_t *) calloc(arr_size, sizeof(uint32_t));
    copyArray(number, y, arr_size);
    uint32_t *borrow = (uint32_t *) calloc(arr_size, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *) calloc(arr_size, sizeof(uint32_t));
    while (!is_0(y, arr_size)) {
        //  https:// iq.opengenus.org/bitwise-subtraction/
        //  step 1: get the borrow bit
        NOT(x, tmp, arr_size);
        AND(tmp, y, borrow, arr_size);
        //  step 2: get the difference using XOR
        XOR(x, y, x, arr_size);
        //  step 3: left shift borrow by 1
        copyArray(borrow, y, arr_size);
        shiftl(y, arr_size, 1);
    }
    free(y);
    free(borrow);
    free(tmp);
}
int cmp(const uint32_t *a, const uint32_t *b, size_t size) {
    //  1 - >
    //  0 - ==
    //  -1 - <
    for (int i = size - 1; i >= 0; i--) {
        if (a[i] == b[i] && i > 0)
            continue;
        if (a[i] > b[i])
            return 1;
        if (a[i] < b[i])
            return -1;
    }
    return 0;
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
}
void bit_div_arr(uint32_t *arr1, uint32_t *arr2, uint32_t *res, size_t size) {
    /*unsigned int fun2 ( unsigned int a, unsigned int b )
{
    unsigned int res;
    unsigned int acc;
    unsigned int rb;

    res=0;
    acc=0;
    for(rb=0x80000000;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&a) acc|=1;
        if(acc>=b)
        {
            acc-=b;
            res|=rb;
        }
    }
    return(res);
}*/
    uint32_t *a2 = calloc(size, sizeof(uint32_t));
    copyArray(arr2, a2, size);
    uint32_t *acc = calloc(size, sizeof(uint32_t));
    uint32_t *rb = calloc(size, sizeof(uint32_t));
    uint32_t *buf = calloc(size, sizeof(uint32_t));
    uint32_t *one = calloc(size, sizeof(uint32_t));
    one[0] = 1;
    init_0(res, size);
    setBits(&(rb[size - 1]), 1, 31, 1);  /*rb = 0x80000000*/
    while (!is_0(rb, size)) { /*for*/
        shiftl1(acc, size);
        AND(rb, arr1, buf, size);
        if (!is_0(buf, size))
            OR(acc, one, acc, size);
        int cmp_res = cmp(acc, a2, size);
        if (cmp_res == 1 || cmp_res == 0) {
            bit_sub_arr(acc, a2, size);
            OR(res, rb, res, size);
        }
        shiftr1(rb, size); /*rb >>= 1*/
    }
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
}
void bit_div_mod_arr(uint32_t *arr1, uint32_t *arr2, uint32_t *div, uint32_t *mod, size_t size) {
    // uint32_t *div = calloc(size, sizeof(uint32_t));
    uint32_t *mul = calloc(size, sizeof(uint32_t));
    uint32_t *a1 = calloc(size, sizeof(uint32_t));
    copyArray(arr1, a1, size);
    uint32_t *a2 = calloc(size, sizeof(uint32_t));
    copyArray(arr2, a2, size);

    bit_div_arr(a1, a2, div, size);
    bit_mul_arr(a2, div, mul, size);
    bit_sub_arr(a1, mul, size);
    copyArray(a1, mod, size);
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
    //  не работает
    //   10001010110001110010001100000100 10001001111010000000000000000000 - делится на 10
    //   00000000000000000000000000000000 10001001111010000000000000000000 - не делится на 10
    //  while (arr[0] % 10 == 0 && *scale > 0) {
    //      div10(arr, size);
    //      (*scale)--;
    //  }
    //  return 0;
    //  по-тупому
    uint32_t *buf = calloc(size, sizeof(uint32_t));
    uint32_t *ten = calloc(size, sizeof(uint32_t));
    ten[0] = 10;
    copyArray(arr, buf, size);
    while (*scale > 0) {
        // printf("%d\t", *scale);
        // printBits(size * 4, arr, 8);
        bit_div_arr(arr, ten, buf, size);
        mul10(buf, size);
        if (cmp(arr, buf, size) == 0) {
            bit_div_arr(buf, ten, arr, size);
            (*scale)--;
        } else {
            break;
        }
    }
    return OK;
}
int eq_scale_arr(uint32_t *x, uint32_t *y, int scalex, int scaley, size_t size) {
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
// unsigned divu10(unsigned n) {
//     unsigned q, r;
//     q = (n >> 1) + (n >> 2);
//     q = q + (q >> 4);
//     q = q + (q >> 8);
//     q = q + (q >> 16);
//     q = q >> 3;
//     r = n - (((q << 2) + q) << 1);
//     return q + (r > 9);
// }
uint32_t *make_arr(size_t size) {
    uint32_t *q = malloc(sizeof(uint32_t) * size);
    memset(q, 0, sizeof(uint32_t) * size);
    return q;
}
void div10(uint32_t *x, size_t size) {
    //  use bit_div_arr
    //     unsigned q, r,tmp;
    //     q = (x >> 1) + (x >> 2);
    //     q += (q >> 4);
    //     q += (q >> 8);
    //     q += (q >> 16);
    //     q = q >> 3;
    //     tmp = q + (q << 2);
    //     r = x - (tmp << 1);
    //     // printf("%u\n",r);
    //     return q + (r > 9);

    uint32_t *q = make_arr(size);
    uint32_t *q2 = make_arr(size);
    uint32_t *q4 = make_arr(size);
    uint32_t *q8 = make_arr(size);
    uint32_t *q16 = make_arr(size);
    uint32_t *q3 = make_arr(size);
    uint32_t *r = make_arr(size);
    uint32_t *x1 = make_arr(size);
    uint32_t *x2 = make_arr(size);
    //  q = (n >> 1) + (n >> 2);
    copyArray(x, x1, size);
    copyArray(x, x2, size);
    shiftr(x1, size, 1);
    shiftr(x2, size, 2);
    bit_add_arr(x2, x1, size);
    copyArray(x2, q, size);
    //  q += (q >> 4);
    copyArray(q, q4, size);
    shiftr(q4, size, 4);
    bit_add_arr(q, q4, size);
    //  q += (q >> 8);
    copyArray(q, q8, size);
    shiftr(q8, size, 8);
    bit_add_arr(q, q8, size);
    //  q += (q >> 16);
    copyArray(q, q16, size);
    shiftr(q16, size, 16);
    bit_add_arr(q, q16, size);
    //  q = q >> 3;
    shiftr(q, size, 3);
    //  tmp = q + (q << 2);
    uint32_t *tmp = make_arr(size);
    copyArray(q, q2, size);
    shiftl(q2, size, 2);
    bit_add_arr(tmp, q, size);
    bit_add_arr(tmp, q2, size);
    //  r = x - (tmp << 1);
    uint32_t *tmp1l = make_arr(size);
    copyArray(tmp, tmp1l, size);
    shiftl1(tmp1l, size);
    copyArray(x, r, size);
    bit_sub_arr(r, tmp1l, size);
    // return q + (r > 9);
    int more9 = 0;
    for (size_t i = size - 1; i >= 1; --i) {
        if (r[i] > 0) {
            more9 = 1;
        }
    }
    if (r[0] > 9) {
        more9 = 1;
    }
    if (more9)
        bit_add(q, 1, size);
    copyArray(q, x, size);
    free(q);
    free(q2);
    free(q4);
    free(q8);
    free(q16);
    free(q3);
    free(r);
    free(x1);
    free(x2);
    free(tmp);
    free(tmp1l);
}