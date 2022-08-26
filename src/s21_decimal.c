//
//  Created by Gladis Ariane on 7/3/22.
//

#include "s21_decimal.h"
#include "s21_arrays.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

int getDecimalExp(decimal d) {
    int i = d.bits[3] << 1;
    int j = i >> 17;
    return j;
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

int move_scale(int cycles, s21_decimal *num) {
    uint32_t x[4] = {0};
    copyArray((uint32_t *) num->bits, x, 3);
    for (int i = 0; i < cycles; ++i) {
        // init_0(tmp, 4);
        // copyArray(num->bits, x, 4);
        // shiftl(x, 4, 1);
        // bit_add_arr(tmp, x, 4);
        // shiftl(x, 4, 2);
        // bit_add_arr(tmp, x, 4);
        // copyArray(tmp, num->bits, 3);
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

int reduce_scale(decimal *x) {
    int scale = getDecimalExp(*x);
    uint32_t buf[3];
    copyArray((uint32_t *) x->bits, buf, 3);

    reduce_scale_arr(buf, 3, &scale);
    copyArray(buf, (uint32_t *) x->bits, 3);
    setDecimalExp(x, scale);
    return 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    memset(dst, 0, sizeof(s21_decimal));  //   Иначе забивается мусор

    if (src < 0) {
        dst->bits[3] = INT32_MIN;  //  Задаст 31й бит в 1
    }
    dst->bits[0] = abs(src);

    return OK;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    if (src.bits[1] || src.bits[2]) {
        return CE;
    }
    int value = src.bits[0];
    if (getDecimalSign(src)) {
        if (value == INT32_MIN)
            *dst = INT32_MIN;
        else
            *dst = -1 * value;
    } else {
        if (getBits(src.bits, 31, 1)) {  //  31й хранит знак(в int), если там не 0 значит у нас оверфлоу.
            return CE;
        }
        *dst = value;
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
        //   Проверить что использовать 3 вместо 4 можно
        bit_add(dst->bits, result, 3);
        bin_exp--;
    }
    // result = 0;
    int last_whole_bit_pos = curr_bit_pos;
    int last_dec_bit_pos = curr_bit_pos;
    int dec_exp;
    double buf_res;
    int power = 10;
    for (; curr_bit_pos <= 23; ++curr_bit_pos) { //  Проверить , < or <=.
        uint32_t bit = getBits(&mant, 23 - curr_bit_pos, 1);
        if (bit == 1) {
            last_dec_bit_pos = curr_bit_pos;
        }
        buf_res = bit * pow(2, bin_exp);
        buf_res *= power;  //   10,100,1000...
        result *= 10;  //   10,100,1000...

        result += (uint64_t) buf_res;
        printf("%lu\n", result);
        power *= 10;
        bin_exp--;
    }
    // bit_add(dst->bits, result, 3);
    dec_exp = last_dec_bit_pos - last_whole_bit_pos;
    bit_add(dst->bits, result, 3);
    setBits(&dst->bits[3], dec_exp, 16, 8);  //    - Вынести это в отдельные функции
    setBits(&dst->bits[3], sign, 31, 1);  //    - Вынести это в отдельные функции
    return OK;*/
    int sign = getBits(&src, 31, 1);
    if (sign) {
        src *= -1;
    }
    char ch[100];

    snprintf(ch, sizeof(ch), "%.6f", src);
    if (strcmp("inf", ch) == 0 || strcmp("nan", ch) == 0 || strcmp("-inf", ch) == 0)
        return CE;
    int exp = strlen(ch) - (strchr(ch, '.') - ch) - 1;  //   проверить нужен ли -1
    for (int i = strlen(ch) - exp - 1; (size_t) i < strlen(ch); ++i) {
        ch[i] = ch[i + 1];
    }
    //  exponent-- ch/=10
    for (size_t i = strlen(ch) - 1; i > 0 && ch[i] == '0' && exp > 0; --i) {
        exp--;
        ch[i] = '\0';
    }
    if (exp > 28 || fabsf(src) > powf(2, 96) - 1)
        return CE;
    char *end;
    int res = strtol(ch, &end, 10);
    s21_from_int_to_decimal(res, dst);
    setBits(&(dst->bits[3]), exp, 16, 8);
    if (sign)
        setBits(&(dst->bits[3]), sign, 31, 1);

    return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    uint32_t x[3];
    copyArray((uint32_t *) src.bits, x, 3);
    int p = 0;
    double res = 0;
    while (!is_0(x, 3)) {
        float d = getBits(x, 0, 1) * powf(2, p);
        res += d;
        shiftr1(x, 3);
        if (res >= MAXFLOAT && !is_0(x, 3)) {
            *dst = 0;
            return getDecimalSign(src) ? TOOSMALL : TOOLARGE;
        }
        p++;
    }
    // res -= 1;  //   я не знаю почему, но так надо. Но не всегда.
    int _exp = getDecimalExp(src);
    res /= pow(10.0, _exp);
    if (res > MAXFLOAT) {
        *dst = 0;
        return getDecimalSign(src) ? TOOSMALL : TOOLARGE;
    }
    res *= getDecimalSign(src) ? -1 : 1;
    *dst = (float) res;
    //  decimal d
    //  t = truncate(d)
    //  res +=t
    //  d -= t
    //  res +=d
    return OK;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {  //  вызывать sub когда надо
    eq_scale(&value_1, &value_2);
    init_0((uint32_t *) result->bits, 4);
    int s1 = getDecimalSign(value_1);
    int s2 = getDecimalSign(value_2);
    int sign_diff = 0;
    int res = OK;
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
            res = s21_sub(value_1, value_2, result);
            s21_negate(*result, result);
            return res;
        } else if (s21_is_greater(value_2, value_1)) {
            res = s21_sub(value_2, value_1, result);
            s21_negate(*result, result);
            return res;
        } else {
            init_0((uint32_t *) result->bits, 4);
            return OK;
        }
    }
    uint32_t x[7] = {0};
    uint32_t y[7] = {0};

    copyArray((uint32_t *) value_1.bits, x, 3);
    copyArray((uint32_t *) value_2.bits, y, 3);
    int exp_x = getDecimalExp(value_1);
    int exp_y = getDecimalExp(value_2);
    int max_scale = eq_scale_arr(x, y, exp_x, exp_y, 7);
    //   scale reduction
    bit_add_arr(x, y, 7);
    reduce_scale_arr(x, 7, &max_scale);
    setDecimalSign(result, s1 & s2);
    if (max_scale > 0b11111111 || x[3] > 0) {
        if (getDecimalSign(*result)) {
            return TOOSMALL;
        } else {
            return TOOLARGE;
        }
    } else {
        copyArray(x, (uint32_t *) result->bits, 3);
        // for (int i = 0; i < 3; ++i)
        //     result->bits[i] = x[i];
        setDecimalExp(result, max_scale);
    }

    return res;
}

// int reduce_scale(decimal *d) {
//     int exp = getDecimalExp(*d);

//     return OK;
// }

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int ret = 0;
    init_0((uint32_t *) result->bits, 4);
    if (getDecimalSign(value_1) == 0 && getDecimalSign(value_2) == 0) {  //  одинаковый +
        if (s21_is_equal(value_1, value_2)) {
            return 0;
        } else if (s21_is_greater(value_1, value_2)) {
            uint32_t x[7] = {0};
            uint32_t y[7] = {0};
            copyArray((uint32_t *) value_1.bits, x, 3);
            copyArray((uint32_t *) value_2.bits, y, 3);
            int max_scale = eq_scale_arr(x, y,
                                         getDecimalExp(value_1),
                                         getDecimalExp(value_2),
                                         7);
            bit_sub_arr(x, y, 7);
            reduce_scale_arr(x, 7, &max_scale);
            copyArray(x, (uint32_t *) result->bits, 3);  //   result!!
            setDecimalExp(result, max_scale);  //   result!!
            // reduce_scale(result);
            return ret;
        } else {
            ret = s21_sub(value_2, value_1, result);
            s21_negate(*result, result);
        }
    } else if (getDecimalSign(value_1) == 1 && getDecimalSign(value_2) == 1) {  // одинаковый -
        if (s21_is_greater(value_1, value_2)) {  //  -1 - (-10)
            s21_negate(value_2, &value_2);
            s21_negate(value_1, &value_1);
            ret = s21_sub(value_2, value_1, result);
        } else if (s21_is_greater(value_2, value_1)) {  //  -10 - (-1)
            s21_negate(value_2, &value_2);
            s21_negate(value_1, &value_1);
            ret = s21_sub(value_1, value_2, result);
            setDecimalSign(result, 1);
        }
    } else if (getDecimalSign(value_1) && !getDecimalSign(value_2)) {  // отрицательное, минус положительное
        s21_negate(value_1, &value_1);
        ret = s21_add(value_1, value_2, result);
        s21_negate(*result, result);
    } else {  // положительное минус отрицательное
        s21_negate(value_2, &value_2);
        ret = s21_add(value_1, value_2, result);
        s21_negate(*result, result);
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
    } else if (getDecimalSign(num1) != getDecimalSign(num2)) {
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
    int res = 0;
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
        int tmp_int;
        for (int i = 0; i < exp; i++) {
            uint64_t u_num;  //  18,446,744,073,709,551,615
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
        if (sign)
            setDecimalSign(result, 0);
        s21_truncate(*result, result);
        if (sign) {
            s21_decimal tmp = {{1, 0, 0, 0}};
            s21_add(*result, tmp, result);
        }
        if (sign)
            setDecimalSign(result, 0);
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

int s21_bank_round(decimal *value) {
    int res = OK;
    //  Банковское только для ровно х.5
    //   случаи с 0.5000 и т.д. Пока что использовать только после reduce_scale
    if (value->bits[0] % 5 == 0 && getDecimalExp(*value) == 1) {
        res = s21_truncate(*value, value);
        if (value->bits[0] % 2 == 1 && !res) {
            decimal one = {{1, 0, 0, 0}};
            res = s21_add(*value, one, value);
        }
    } else {
        res = s21_round(*value, value);
    }
    return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    init_0((uint32_t *) result->bits, 4);
    uint32_t val1[7] = {0};
    uint32_t val2[7] = {0};
    uint32_t res[7] = {0};
    copyArray((uint32_t *) value_1.bits, val1, 3);
    copyArray((uint32_t *) value_2.bits, val2, 3);
    bit_mul_arr(val1, val2, res, 7);
    if (getDecimalSign(value_1) != getDecimalSign(value_2))
        setDecimalSign(result, 1);
    if (res[3] || res[4] || res[5] || res[6]) {
        return getDecimalSign(*result) ? TOOSMALL : TOOLARGE;
    } else {
        copyArray(res, (uint32_t *) result->bits, 3);
    }
    int exp = getDecimalExp(value_1) + getDecimalExp(value_2);
    if (exp > 28) {
        return getDecimalSign(*result) ? TOOSMALL : TOOLARGE;
    } else {
        setDecimalExp(result, exp);
    }
    return OK;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (is_0(value_2.bits, 3)) {
        return DIVBY0;
    }
    size_t size = 12;
    //  использовать статические, когда определимся с нужным значением
    uint32_t *a1 = calloc(size, sizeof(uint32_t));
    copyArray((uint32_t *) &(value_1.bits), a1, 3);
    uint32_t *a2 = calloc(size, sizeof(uint32_t));
    copyArray((uint32_t *) &(value_2.bits), a2, 3);

    uint32_t *res = calloc(size, sizeof(uint32_t));
    uint32_t *mod = calloc(size, sizeof(uint32_t));
    uint32_t *div = calloc(size, sizeof(uint32_t));
    eq_scale_arr(a1, a2, getDecimalExp(value_1), getDecimalExp(value_2), size);
    //  алгоритм
    /*
    st = 0
    for i in range(29):
        d = a //  b
        m = a % b
        a = m*10
        st += d
        st *= 10
    res = st
    reduce scale если нужно
    */

    int res_exp = 0;
    while (cmp(a1, a2, size) < 0) {
        mul10(a1, size);
        res_exp++;
    }

    int count = 0;
    do {  //  Это исторический момент, мне впервые пригодился do while
        bit_div_mod_arr(a1, a2, div, mod, size);
        mul10(mod, size);
        copyArray(mod, a1, size);
        bit_add_arr(res, div, size);
        mul10(res, size);
        res_exp++;
        count++;
    } while (!is_0(mod, size) && count < 28);

    reduce_scale_arr(res, size, &res_exp);
    int ret = 0;
    if (!res[3] && res_exp <= 28) {
        copyArray(res, result->bits, 3);
        setDecimalExp(result, res_exp);
        setDecimalSign(result, getDecimalSign(value_1) ^ getDecimalSign(value_2));
        ret = OK;
    } else {
        ret = TOOSMALL;  //  toolarge
    }
    free(a1);
    free(a2);
    free(res);
    free(mod);
    free(div);
    return ret;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (is_0(value_2.bits, 3)) {
        return DIVBY0;
    }
    // забить на отрицательные, вертеру насрать
    int s2 = getDecimalExp(value_2);
    int s1 = getDecimalExp(value_1);
    int scale = s1 > s2 ? s1 : s2;
    bit_mod_arr(value_1.bits, value_2.bits, result->bits, 3);
    setDecimalExp(result, scale);
    return OK;
}
