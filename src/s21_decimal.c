//
// Created by Gladis Ariane on 7/3/22.
//

#include "s21_decimal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define printDecimal (D)(printBits(sizeof(int) * 4, D))

#define getDecimalExp(D) (getBits(&D[3], 16, 8))
#define getDecimalSign(D) (getBits(&D[3], 31, 1))

void printDecimalValue(s21_decimal d) {
    char sign = getDecimalSign(d.bits) ? '-' : '+';
    printf("%c", sign);
    printf("%u+%u*4294967296+%u*4294967296**2/%ld\n", d.bits[0], d.bits[1], d.bits[2],
           (long int) pow(10, getBits(&d.bits[3], 16, 8)));
}
// 79228162514264337593543950335
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
// Возвращает новый срез битов из 32х других,
// offset - место начала среза
// n = длина среза
//
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
    bits &= mask;  // Зануляет ненужные биты
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
    size_t len = size * 32;
    if (n >= len) {
        return CE;
    }
    uint32_t *arr = (uint32_t *) object;
    uint32_t *tmp = (uint32_t *) malloc(len);
    memset(tmp, 0, len);
    for (int i = 0; i < len - n; ++i) {
        int bit = getBits(&arr[i / 32], i % 32, 1);
        setBits(&tmp[(i + n) / 32], bit, (i + n) % 32, 1);
    }
    for (int i = 0; i < size; ++i) {
        arr[i] = tmp[i];
    }
    free(tmp);
    return OK;
}

int shiftr(void *object, size_t size, int n) {
    size_t len = size * 32;
    if (n >= len)
        return CE;
    uint32_t *arr = (uint32_t *) object;
    uint32_t *tmp = (uint32_t *) malloc(len);
    memset(tmp, 0, sizeof(*tmp) * size);
    for (size_t i = len - 1; i >= n; --i) {
        int bit = getBits(&arr[i / 32], i % 32, 1);
        setBits(&tmp[(i - n) / 32], bit, (i - n) % 32, 1);
    }
    for (int i = 0; i < size; ++i) {
        arr[i] = tmp[i];
    }
    free(tmp);
    return OK;
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
        //printf("%d\n",is_0(carry, arr_size));
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
    uint32_t *y = (uint32_t *) number;

    uint32_t *sum = (uint32_t *) malloc(sizeof(uint32_t) * arr_size);
    uint32_t *carry = (uint32_t *) malloc(sizeof(uint32_t) * arr_size);

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
        //printf("%d\n",is_0(carry, arr_size));
    }
    for (size_t i = 0; i < arr_size; i++) {
        x[i] = sum[i];
    }
    free(carry);
    free(sum);
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    memset(dst, 0, sizeof(s21_decimal)); // Иначе забивается мусор

    if (src < 0) {
        dst->bits[3] = signMask;  // Задаст 31й бит в 1
        if (src == INT32_MIN) {   // abs не работает на минимальном значении.
            src = 0;  // Поэтому убираем знаковый бит. остальные итак 0
        }
    }
    dst->bits[0] = abs(src);

    return OK;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    if (src.bits[1] || src.bits[2]) {
        return CE;
    }
    if (getBits(src.bits, 31, 1)) {  // 31й хранит знак(в int), если там не 0 значит у нас оверфлоу.
        return CE;
    }
    if (getDecimalSign(src.bits)) {
        *dst = -1 * src.bits[0];
    } else {
        *dst = src.bits[0];
    }

    return OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    /*memset(dst, 0, sizeof(s21_decimal));
    uint32_t mant = getBits(&src, 0, 23);
    uint32_t exp = getBits(&src, 23, 8);
    uint32_t sign = getBits(&src, 31, 1);
    int bin_exp = exp - 127;
    if (bin_exp > 96 || bin_exp < -96) {
        return CE;
    }
    int curr_bit_pos = 0;
    uint64_t result;

    result = 1 * pow(2, bin_exp--);
    printf("%lu\n", result);
    bit_add(dst->bits, result, 3);
    while (bin_exp >= 0) {
        uint32_t bit = getBits(&mant, 22 - curr_bit_pos++, 1);
        result = bit * pow(2, bin_exp);
        printf("%lu\n", result);
        // TODO Проверить что использовать 3 вместо 4 можно
        bit_add(dst->bits, result, 3);
        bin_exp--;
    }
    //result = 0;
    int last_whole_bit_pos = curr_bit_pos;
    int last_dec_bit_pos = curr_bit_pos;
    int dec_exp;
    double buf_res;
    int power = 10;
    for (; curr_bit_pos <= 23; ++curr_bit_pos) { // Проверить , < or <=.
        uint32_t bit = getBits(&mant, 23 - curr_bit_pos, 1);
        if (bit == 1) {
            last_dec_bit_pos = curr_bit_pos;
        }
        buf_res = bit * pow(2, bin_exp);
        buf_res *= power; //Todo 10,100,1000...
        result *= 10; //Todo 10,100,1000...

        result += (uint64_t) buf_res;
        printf("%lu\n", result);
        power *= 10;
        bin_exp--;
    }
    //bit_add(dst->bits, result, 3);
    dec_exp = last_dec_bit_pos - last_whole_bit_pos;
    bit_add(dst->bits, result, 3);
    setBits(&dst->bits[3], dec_exp, 16, 8); // Todo - Вынести это в отдельные функции
    setBits(&dst->bits[3], sign, 31, 1); // Todo - Вынести это в отдельные функции
    return OK;*/
    int sign = getBits(&src, 31, 1);
    if (sign) {
        src *= -1;
    }
    char ch[100];
    sprintf(ch, "%.5f", src);
    int exp = strlen(ch) - (strchr(ch, '.') - ch) - 1; //TODO проверить нужен ли -1
    if (exp > 28)
        return CE;
    for (int i = strlen(ch) - exp - 1; i < strlen(ch); ++i) {
        ch[i] = ch[i + 1];
    }
    int res = atoi(ch);
    s21_from_int_to_decimal(res, dst);
    setBits(&(dst->bits[3]), exp, 16, 8);
    if (sign)
        setBits(&(dst->bits[3]), sign, 31, 1);

    return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) { return 0; }

void copyArray(uint32_t *from, uint32_t *to, size_t len) {
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
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    uint32_t x[7] = {0};
    uint32_t y[7] = {0};

    copyArray(value_1.bits, x, 3);
    copyArray(value_2.bits, y, 3);
    int exp_x = getBits(&value_1.bits[3], 23, 8);
    int exp_y = getBits(&value_2.bits[3], 23, 8);
    int exp_delta = exp_x > exp_y ? exp_x - exp_y : exp_y - exp_x;
    if (exp_x < exp_y) {
        for (int i = 0; i < exp_delta; ++i) { // x * 10
            mul10(x, 7);
        }
    } else {
        for (int i = 0; i < exp_delta; ++i) { // y * 10
            mul10(y, 7);
        }
    }
    bit_add_arr(x, y, 3);
    while (exp_delta > 0 && !getBits(x, 0, 1)) { //TODO optimize(jarrusab)
        shiftr(x, 7, 1);
        exp_delta--;
    }

    if (x[3] > 0) return CE;

    for (int i = 0; i < 3; ++i) result->bits[i] = x[i];

    if (exp_delta > 0b11111111) {
        return CE;
    } else {
        setBits(&result->bits[3], exp_delta, 23, 8);
    }

    return OK;
}
