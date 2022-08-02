//
// Created by Gladis Ariane on 7/3/22.
//

#include "s21_decimal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

int getDecimalExp(decimal d) {
    return (d.bits[3] << 1) >> 17;
}

void setDecimalExp(decimal *d, int exp) {
    setBits(&d->bits[3], exp, 16, 8);
}
int getDecimalSign(decimal d) {
    return getBits(&d.bits[3], 31, 1);
}
void setDecimalSign(decimal *d, int sign) {
    setBits(&d->bits[3], sign, 31, 1);
}
void printDecimalValue(s21_decimal d) {
    char sign = getDecimalSign(d) ? '-' : '+';
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
        for (j = 31; j >= 0; j--) {
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
    if (n > 32 * size) {
        puts("dont shift more than arr size");
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        shiftl1(object, size);
    }
    return 0;
}

int shiftr(void *object, size_t size, int n) {
    if (n > 32 * size) {
        puts("fuck you from shiftr");
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        shiftr1(object, size);
    }
    return 0;
}

void shiftl1(uint32_t *arr, size_t size) {
    for (int i = size; i >= 1; --i) {
        arr[i] <<= 1;
        /* arr[i] += (arr[i - 1] & (1u << 31)) >> 31; */
	arr[i] += (arr[i - 1] & 0x80000000) ? 1 : 0;
    }
    arr[0] <<= 1;
}
void shiftr1(uint32_t *arr, size_t size) {
    for (int i = 0; i < size - 1; ++i) {
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

int is_decimal_0(s21_decimal value) {
    for (int i = 0; i < 3; i++) {
	if (value.bits[i] != 0) {
	    return 1;
	}
    }
    return 0;
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

void init_0(uint32_t *arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = 0;
    }
}

int move_scale(int cycles, s21_decimal *num) {
    uint32_t x[4] = {0};
    for (int i = 0; i < cycles; ++i) {
        //init_0(tmp, 4);
        //copyArray(num->bits, x, 4);
        //shiftl(x, 4, 1);
        //bit_add_arr(tmp, x, 4);
        //shiftl(x, 4, 2);
        //bit_add_arr(tmp, x, 4);
        //copyArray(tmp, num->bits, 3);
        mul10(x, 4);
    }
    int new_scale = getDecimalExp(*num) + cycles;
    if (x[3] > 0 || new_scale > 28) {
        return CE;
    }
    copyArray(x, (uint32_t *) num->bits, 3);
    setDecimalExp(num, new_scale);
    return OK;
}

void move_scale_arr(int cycles, uint32_t *arr, size_t size) {
    for (size_t i = 0; i < cycles; ++i) {
        mul10(arr, size);
    }
}

int eq_scale(decimal *x, decimal *y) {
    int scale1 = getDecimalExp(*x);
    int scale2 = getDecimalExp(*y);
    int ret = 0;
    if (scale1 > scale2) {
        ret = move_scale(scale1 - scale2, y);
    } else if (scale2 > scale1) {
        ret = move_scale(scale2 - scale1, x);
    }
    return ret;
}
int eq_scale_arr(uint32_t *x, uint32_t *y, int scalex, int scaley, size_t size) {
    int maxscale = 0;
    if (scalex > scaley) {
        maxscale = scalex;
        move_scale_arr(scalex - scaley, y, size);
    } else if (scaley > scalex) {
        maxscale = scaley;
        move_scale_arr(scaley - scalex, x, size);
    }
    return maxscale;
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
    if (getDecimalSign(src)) {
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
    for (int i = strlen(ch) - exp - 1; (size_t) i < strlen(ch); ++i) {
        ch[i] = ch[i + 1];
    }
    int res = atoi(ch);
    s21_from_int_to_decimal(res, dst);
    setBits(&(dst->bits[3]), exp, 16, 8);
    if (sign)
        setBits(&(dst->bits[3]), sign, 31, 1);

    return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    // decimal d
    // t = truncate(d)
    // res +=t
    // d -= t
    // res +=d
}

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
//unsigned divu10(unsigned n) {
//    unsigned q, r;
//    q = (n >> 1) + (n >> 2);
//    q = q + (q >> 4);
//    q = q + (q >> 8);
//    q = q + (q >> 16);
//    q = q >> 3;
//    r = n - (((q << 2) + q) << 1);
//    return q + (r > 9);
//}
uint32_t *make_arr(size_t size) {
    uint32_t *q = malloc(sizeof(uint32_t) * size);
    memset(q, 0, sizeof(uint32_t) * size);
    return q;
}
void div10(uint32_t *x, int size) {
    uint32_t *q = make_arr(size);
    uint32_t *q4 = make_arr(size);
    uint32_t *q8 = make_arr(size);
    uint32_t *q16 = make_arr(size);
    uint32_t *q3 = make_arr(size);
    uint32_t *r = make_arr(size);
    uint32_t *x1 = make_arr(size);
    uint32_t *x2 = make_arr(size);
    copyArray(x, x1, size);
    copyArray(x, x2, size);
    shiftr(x1, size, 1);
    shiftr(x2, size, 2);
    bit_add_arr(x2, x1, size);
    copyArray(x2, q, size);
    copyArray(q, q4, size);
    shiftr(q4, size, 4);
    bit_add_arr(q, q4, size);
    copyArray(q, q8, size);
    shiftr(q8, size, 8);
    bit_add_arr(q, q8, size);
    copyArray(q, q16, size);
    shiftr(q16, size, 16);
    bit_add_arr(q, q16, size);
    copyArray(q, q3, size);
    shiftr(q3, size, 3);
    bit_add_arr(q, q3, size);
    //TODO bit sub
    //printBits(4*size,q,4);
    //printBits(4*size,r,4);

}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) { // TODO вызывать sub когда надо
    init_0((uint32_t *) result->bits, 4);
    int s1 = getDecimalSign(value_1);
    int s2 = getDecimalSign(value_2);
    int sign_diff = 0;
    if (!s1 && s2) {
        sign_diff = 1;
        s21_negate(value_2, &value_2);
    }
    if (s1 && !s2) {
        sign_diff = 1;
        s21_negate(value_1, &value_1);
    }
    if (sign_diff) {
        if (s21_is_greater(value_1, value_2)) {
            return s21_sub(value_1, value_2, result);
        } else if (s21_is_greater(value_2, value_1)) {
            return s21_sub(value_2, value_1, result);
        }
    }
    uint32_t x[7] = {0};
    uint32_t y[7] = {0};

    copyArray((uint32_t *) value_1.bits, x, 3);
    copyArray((uint32_t *) value_2.bits, y, 3);
    int exp_x = getBits(&value_1.bits[3], 23, 8);
    int exp_y = getBits(&value_2.bits[3], 23, 8);
    int max_scale = eq_scale_arr(x, y, exp_x, exp_y, 7);
    // TODO scale reduction
    bit_add_arr(x, y, 3);
    setDecimalSign(result, s1 & s2);
    if (max_scale > 0b11111111 || x[3] > 0) {
        if (getDecimalSign(*result)) {
            return TOOSMALL;
        } else {
            return TOOLARGE;
        }
    } else {
        for (int i = 0; i < 3; ++i) result->bits[i] = x[i];
        setBits(&result->bits[3], max_scale, 23, 8);
    }

    return OK;
}

//int reduce_scale(decimal *d) {
//    int exp = getDecimalExp(*d);
//
//    return OK;
//}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int ret = 0;
    init_0((uint32_t *) result->bits, 4);
    if (getDecimalSign(value_1) == 0 && getDecimalSign(value_2) == 0) { // одинаковый +
        if (s21_is_equal(value_1, value_2)) {
            return 0;
        } else if (s21_is_greater(value_1, value_2)) {
            // https://iq.opengenus.org/bitwise-subtraction/

            uint32_t borrow[7] = {0};
            uint32_t x[7] = {0};
            uint32_t y[7] = {0};
            uint32_t tmp[7] = {0};
            copyArray((uint32_t *) value_1.bits, x, 3);
            copyArray((uint32_t *) value_2.bits, y, 3);
            int max_scale = eq_scale_arr(x, y,
                                         getDecimalExp(value_1),
                                         getDecimalExp(value_2),
                                         7);
            while (!is_0(y, 7)) {
                // step 1: get the borrow bit
                NOT(x, tmp, 7);
                AND(tmp, y, borrow, 7);
                // step 2: get the difference using XOR
                XOR(x, y, x, 7);
                // step 3: left shift borrow by 1
                copyArray(borrow, y, 7);
                shiftl(y, 7, 1);
            }
            copyArray(x, (uint32_t *) result->bits, 3); // result!!
            setDecimalExp(result, max_scale); // result!!
            return ret;
            // TODO scale reduction
            // TODO error codes
        } else {
            ret = s21_sub(value_2, value_1, result);
            s21_negate(*result, result);
        }
    } else if (getDecimalSign(value_1) == 1 && getDecimalSign(value_2) == 1) { //одинаковый -
        // -1 - (-10)
        if (s21_is_greater(value_1, value_2)) {
            s21_negate(value_2, &value_2);
            s21_negate(value_1, &value_1);
            ret = s21_sub(value_2, value_1, result);
        } else if (s21_is_greater(value_2, value_1)) { // -10 - (-1)
            s21_negate(value_2, &value_2);
            s21_negate(value_1, &value_1);
            ret = s21_sub(value_1, value_2, result);
            setDecimalSign(result, 1);
        }
    } else if (getDecimalSign(value_1) && !getDecimalSign(value_2)) { //отрицательное, минус положительное
        s21_negate(value_1, &value_1);
        ret = s21_add(value_1, value_2, result);
        s21_negate(*result, result);
    } else { //положительное минус отрицательное
        s21_negate(value_2, &value_2);
        ret = s21_add(value_1, value_2, result);
    }
    if (ret != 0)
        ret = getDecimalSign(*result) ? 2 : 1;
    return ret;
}
int s21_negate(s21_decimal value, s21_decimal *result) {
    setDecimalSign(result, !getDecimalSign(value));
    return OK;
}

int s21_is_equal(s21_decimal num1, s21_decimal num2) {
    s21_decimal tmp_num1 = num1, tmp_num2 = num2;
    int ret;
    int is_01 = is_0(num1.bits, 3);
    int is_02 = is_0(num2.bits, 3);
    if (is_01 && is_02) {
        ret = 1;
    } else if (is_01 || is_02) {
        ret = 0;
    } else {
        ret = tmp_num1.bits[0] != tmp_num2.bits[0] ? 0 :
              tmp_num1.bits[1] != tmp_num2.bits[1] ? 0 :
              tmp_num1.bits[2] != tmp_num2.bits[2] ? 0 :
              tmp_num1.bits[0] != tmp_num2.bits[0] ? 0 : 1;
    }
    return ret;
}

int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
    return s21_is_equal(num1, num2) ? 0 : 1;
}

int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2) {
    return s21_is_equal(num1, num2) || s21_is_greater(num1, num2);
}

int s21_is_greater(s21_decimal x, s21_decimal y) {
    int res;
    eq_scale(&x, &y);
    if (getDecimalSign(x) == 1 && getDecimalSign(y) == 0) {
        res = 0;
    } else if (getDecimalSign(x) == 0 && getDecimalSign(y) == 1) {
        res = 1;
    } else {
        for (int i = 2; i >= 0; i--) {
            if (x.bits[i] > y.bits[i]) {
                res = getDecimalSign(x) ? 0 : 1;
                break;
            } else if (y.bits[i] > x.bits[i]) {
                res = getDecimalSign(y) ? 1 : 0;
                break;
            }
        }
    }
    return res;
}

int s21_is_less(s21_decimal num1, s21_decimal num2) {
    return !s21_is_greater_or_equal(num1, num2);
}

int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2) {
    return !s21_is_greater(num1, num2);
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    int exp = getDecimalExp(value);
    int sign = getDecimalSign(value);
    *result = value;
    if (exp != 0) {
        uint64_t u_num;  // 18,446,744,073,709,551,615
        int tmp_int = 0;
        for (int i = 0; i < exp; i++) {
            u_num = result->bits[2];
            for (int j = 2; j >= 0; j--) {
                if (j == 0) {
                    result->bits[j] = u_num / 10;
                } else {
                    tmp_int = u_num % 10;
                    result->bits[j] = u_num / 10;
                    u_num = tmp_int * (4294967296) + result->bits[j - 1];
                }
            }
        }
        result->bits[3] = 0;
    }

    if (sign) setDecimalSign(result, 1);
    return OK;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
    int exp = getDecimalExp(value);
    *result = value;
    if (exp > 0) {
        int sign = getDecimalSign(*result);
        if (sign) setDecimalSign(result, 0);
        s21_truncate(*result, result);
        if (sign) {
            s21_decimal tmp = {{1, 0, 0, 0}};
            s21_add(*result, tmp, result);
        }
        if (sign) setDecimalSign(result, 0);
    }
    return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
    int exp = getDecimalExp(value);
    *result = value;
    if (exp > 0) {
        int sign = getDecimalSign(*result);
        if (sign) setDecimalSign(result, 0);
        result->bits[3] -= 1 << 15;
        s21_truncate(*result, result);
        if (result->bits[0] & 1) {
            result->bits[3] += 1 << 15;
            s21_truncate(*result, result);
            s21_decimal tmp = {{1, 0, 0, 0}};
            s21_add(*result, tmp, result);
        }
        if (sign) setDecimalSign(result, 0);
    }
    return 0;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int exp = 0;
    s21_decimal numerator = value_1;
    s21_decimal separator = value_2;
    int n = 0;
    while ((exp < 28) || (is_decimal_0(value_1))) {
	n = 0;
	separator = value_2;
	if (s21_is_less(numerator, separator)) {
	    while (s21_is_less(numerator, shiftl(separator))) {
		
	    }
	} else {
	    while (s21_is_less(numerator, shiftl1(separator))) {
		n++;
	    }
	    shiftr1(separator);
	    n--;
	}
	s21_sub(numerator, separator, numerator);
	s21_set_bit(result, n, 1);
    }
}
