//
//  Created by Gladis Ariane on 7/31/22.
//
#include <check.h>
#include <stdio.h>
#include "s21_decimal.h"
#include "s21_arrays.h"

START_TEST(SUB_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
    s21_decimal result;

    int res = s21_sub(test1, test2, &result);
    ck_assert_int_eq(res, 0);

    test2.bits[3] = 0;
    // test1.bits[3] = 0;
    res = s21_sub(test1, test2, &result);
    ck_assert_int_eq(res, 2);

    res = s21_sub(test2, test1, &result);
    ck_assert_int_eq(res, 1);

    init_0((uint32_t *) test1.bits, 3);
    init_0((uint32_t *) test2.bits, 3);
    test1.bits[3] = 0;
    test2.bits[3] = 0;
    test1.bits[0] = 1;
    test2.bits[2] = 1;

    //printBits(16,&test2,4);
    //printBits(16,&test1,4);
    s21_sub(test2, test1, &result);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);

    s21_sub(test1, test2, &result);
    //printBits(16,&result,4);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[3], 1u << 31);

    for (int i = -100; i < 100; ++i) {
        for (int j = -100; j < 100; ++j) {
            int a = i;
            int b = j;
            int r;
            decimal aa;
            decimal bb;
            decimal rr;

            s21_from_int_to_decimal(a, &aa);
            // s21_from_decimal_to_int(aa, &a);
            s21_from_int_to_decimal(b, &bb);
            // s21_from_decimal_to_int(bb, &b);
            s21_sub(aa, bb, &rr);
            s21_from_decimal_to_int(rr, &r);
            if (r != a - b) {
                //printf("i=%d\tj=%d\n", i, j);
                //printBits(16,&aa,12);
                //printBits(16,&bb,12);
                //printf("r=%d\n", r);
                printf("res : %d != (%d) - (%d) == (%d)\n", r, a, b, a - b);
                ck_assert_int_eq(r, a - b);
            }
        }
    }

    for (float i = -100; i < 100; i += 0.3) {
        for (float j = -100; j < 100; j += 0.3) {
            float a = i;
            float b = j;
            float r;
            decimal aa;
            decimal bb;
            decimal rr;

            s21_from_float_to_decimal(a, &aa);
            // s21_from_decimal_to_float(aa, &a);
            s21_from_float_to_decimal(b, &bb);
            // s21_from_decimal_to_float(bb, &b);
            s21_sub(aa, bb, &rr);
            s21_from_decimal_to_float(rr, &r);
            if (fabsf(r - (a - b)) > 0.0001) {
                printf("%f != (%f) - (%f) == (%f)\n", r, a, b, a - b);
                ck_assert_float_eq_tol(r, a - b, 0.0001);
            }
            // printf("a - %d\n",a);
            // printf("b - %d\n",b);
            // printf("r - %d\n",r);
        }
    }
}

END_TEST

START_TEST(ADD_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
    s21_decimal result;
    int res = s21_add(test1, test2, &result);
    ck_assert_int_eq(res, 2);

    test2.bits[3] = 0;
    s21_add(test1, test2, &result);
    for (int i = 0; i < 3; ++i) ck_assert_int_eq(result.bits[i], 0);

    test1.bits[3] = 0;
    res = s21_add(test1, test2, &result);
    ck_assert_int_eq(res, 1);

    init_0((uint32_t *) test1.bits, 3);
    init_0((uint32_t *) test2.bits, 3);
    test1.bits[0] = 1;
    test1.bits[3] = 1u << 31;
    test2.bits[2] = 1;
    s21_add(test2, test1, &result);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);

    s21_negate(test1, &test1);
    s21_negate(test2, &test2);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[3], 1u << 31);
}

END_TEST

START_TEST(TO_FROM_INT) {
    decimal d;
    int check;
    for (int i = -10; i < 10; i++) {
        s21_from_int_to_decimal(i, &d);
        s21_from_decimal_to_int(d, &check);
        if (check != i) {
            printf("%d != %d\n", check, i);
            ck_assert_int_eq(check, i);
        }
    }
    int max = INT32_MAX;
    int min = INT32_MIN;
    s21_from_int_to_decimal(max, &d);
    s21_from_decimal_to_int(d, &check);
    if (check != max) {
        printf("%d != %d\n", check, max);
        ck_assert_int_eq(check, max);
    }
    s21_from_int_to_decimal(min, &d);
    s21_from_decimal_to_int(d, &check);
    if (check != min) {
        printf("%d != %d\n", check, min);
        ck_assert_int_eq(check, min);
    }
}

END_TEST

START_TEST(TO_FROM_FLOAT) {
    float f;
    float check;
    decimal res;
    f = 0.5f;
    s21_from_float_to_decimal(f, &res);
    s21_from_decimal_to_float(res, &check);
    printf("%20f\t%20f\n", f, check);
    ck_assert_float_eq_tol(f, check, 0.00005);
    f = 1;
    s21_from_float_to_decimal(f, &res);
    s21_from_decimal_to_float(res, &check);
    printf("%20f\t%20f\n", f, check);
    ck_assert_float_eq_tol(f, check, 0.00005);
    f = 18446744073709551616.0f;
    s21_from_float_to_decimal(f, &res);
    s21_from_decimal_to_float(res, &check);
    printf("%20f\t%20f\n", f, check);
    printBits(16,&res,12);
    printBits(4,&f,4);
    printBits(4,&check,4);
    ck_assert_float_eq_tol(f, check, 0.00005);
    f = 79228162514264337593543950335.0f;
    s21_from_float_to_decimal(f, &res);
    s21_from_decimal_to_float(res, &check);
    printf("%20f\t%20f\n", f, check);
    ck_assert_float_eq_tol(f, check, 0.00005);
    f = 79228162514264337593543950336.0f;
    int err = s21_from_float_to_decimal(f, &res);
    ck_assert_int_eq(err, 1);
}

START_TEST(NEGATE_TEST) {
    s21_decimal d;
    int check;
    for (int i = -10; i < 10; ++i) {
        s21_from_int_to_decimal(i, &d);
        s21_negate(d, &d);
        s21_from_decimal_to_int(d, &check);
        ck_assert_int_eq(-i, check);
    }
}

END_TEST

START_TEST(GREATER_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    int res = s21_is_greater(test1, test2);
    ck_assert_int_eq(res, 0);

    res = s21_is_greater(test2, test1);
    ck_assert_int_eq(res, 1);

    s21_negate(test1, &test1);
    res = s21_is_greater(test2, test1);
    ck_assert_int_eq(res, 0);

    decimal test3 = {11, 0, 0, 0};
    decimal test4 = {10, 0, 0, 0};

    res = s21_is_greater(test3, test4);
    ck_assert_int_eq(res, 1);

    res = s21_is_less(test4, test3);
    ck_assert_int_eq(res, 1);

    test3.bits[0] = 10;

    res = s21_is_greater(test3, test4);
    ck_assert_int_eq(res, 0);

    res = s21_is_equal(test3, test4);
    ck_assert_int_eq(res, 1);

    res = s21_is_not_equal(test3, test4);
    ck_assert_int_eq(res, 0);
}

END_TEST

START_TEST(EQUAL_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    int res = s21_is_equal(test1, test2);
    ck_assert_int_eq(res, 0);

    res = s21_is_equal(test2, test1);
    ck_assert_int_eq(res, 0);

    res = s21_is_equal(test2, test2);
    ck_assert_int_eq(res, 1);

    s21_decimal test3 = {{0, 0, 0, 1u << 31}};
    s21_decimal test4 = {{0, 0, 0, 0}};
    res = s21_is_equal(test3, test4);
    ck_assert_int_eq(res, 1);
}

END_TEST

START_TEST(MUL_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal result;
    int res = s21_mul(test1, test2, &result);
    ck_assert_int_eq(res, 2);

    s21_negate(test1, &test1);
    res = s21_mul(test1, test2, &result);
    ck_assert_int_eq(res, 1);

    init_0((uint32_t *) test2.bits, 3);
    test2.bits[0] = 1;
    setDecimalExp(&test2, 28);
    res = s21_mul(test1, test2, &result);
    ck_assert_int_eq(result.bits[3], 28 << 16);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
    ck_assert_int_eq(res, 0);

    for (int i = -100; i < 100; ++i) {
        for (int j = -100; j < 100; ++j) {
            int a = i;
            int b = j;
            int r;
            decimal aa;
            decimal bb;
            decimal rr;

            s21_from_int_to_decimal(a, &aa);
            // s21_from_decimal_to_int(aa, &a);
            s21_from_int_to_decimal(b, &bb);
            // s21_from_decimal_to_int(bb, &b);
            s21_mul(aa, bb, &rr);
            s21_from_decimal_to_int(rr, &r);
            if (r != a * b) {
                printf("%d != (%d) * (%d) == (%d)\n", r, a, b, a * b);
                ck_assert_int_eq(r, a * b);
            }
        }
    }
    for (float i = -100; i < 100; i += 0.3) {
        for (float j = -100; j < 100; j += 0.3) {
            float a = i;
            float b = j;
            float r;
            decimal aa;
            decimal bb;
            decimal rr;

            s21_from_float_to_decimal(a, &aa);
            // s21_from_decimal_to_float(aa, &a);
            s21_from_float_to_decimal(b, &bb);
            // s21_from_decimal_to_float(bb, &b);
            s21_mul(aa, bb, &rr);
            s21_from_decimal_to_float(rr, &r);
            if (fabsf(r - (a * b)) > 0.001) {
                printf("%f != (%f) * (%f) == (%f)\n", r, a, b, a * b);
                ck_assert_float_eq_tol(r, a * b, 0.001);
            }
            // printf("a - %d\n",a);
            // printf("b - %d\n",b);
            // printf("r - %d\n",r);
        }
    }
}

END_TEST

START_TEST(DIV_TEST) {
    decimal a = {1, 0, 0, 0};
    decimal b = {1, 0, 0, 0};
    decimal r = {0, 0, 0, 0};

    // check exp/scale is correct
    for (int i = 0; i < 29; i++) {
        setDecimalExp(&a, i);
        int err = s21_div(a, b, &r);
        if (!err) {
            ck_assert_int_eq(getDecimalExp(r), i);
        } else {
            fail("FAIL div exp err");
        }
    }
    setDecimalExp(&a, 0);
    setDecimalExp(&a, 0);
    for (int i = 0; i < 29; i++) {
        setDecimalExp(&b, i);
        int err = s21_div(a, b, &r);
        if (!err) {
            ck_assert_int_eq(getDecimalExp(r), 0);
            //printBits(16, &r, 4);
            //printf("%d\n", getDecimalExp(r));
        } else {
            fail("FAIL div exp err");
        }
    }
    // check signs are correct
    int signs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1},};
    for (int i = 0; i < 4; ++i) {
        int s1 = signs[i][0];
        int s2 = signs[i][1];
        setDecimalSign(&a, s1);
        setDecimalSign(&b, s2);
        int err = s21_div(a, b, &r);
        if (!err) {
            ck_assert_int_eq(getDecimalSign(r), s1 != s2);
        } else {
            fail("FAIL div sign err");
        }
    }
}

END_TEST

Suite *f_example_suite_create() {
    Suite *s1 = suite_create("Test_decimal");

    TCase *p_case = tcase_create("Core");

    tcase_set_timeout(p_case, 0);
    //tcase_add_test(p_case, SUB_TEST);
    //tcase_add_test(p_case, ADD_TEST);
    //tcase_add_test(p_case, TO_FROM_INT);
    tcase_add_test(p_case, TO_FROM_FLOAT);
    //tcase_add_test(p_case, NEGATE_TEST);
    //tcase_add_test(p_case, GREATER_TEST);
    //tcase_add_test(p_case, EQUAL_TEST);
    //tcase_add_test(p_case, MUL_TEST);
    //tcase_add_test(p_case, DIV_TEST);
//    tcase_add_test(p_case, MOD_TEST);

    suite_add_tcase(s1, p_case);
    return s1;
}

int main() {
    Suite *s1 = f_example_suite_create();
    //  Runner
    SRunner *runner = srunner_create(s1);
    int number_failed;
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed == 0 ? 0 : 1;
}
