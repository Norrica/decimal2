//
// Created by Gladis Ariane on 7/31/22.
//
#include <check.h>
#include <stdio.h>
#include "s21_decimal.h"

START_TEST(SUB_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1<<31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1<<31}};
    s21_decimal result;

    int res = s21_sub(test1, test2, &result);
    ck_assert_int_eq(res, 1);

    test2[3] = 0;
    res = s21_sub(test1, test2, &result);
    ck_assert_int_eq(res, 2);

    test1[3] = 0;
    s21_sub(test2, test1, &result);
    s21_from_int_to_decimal(res, &result);
    ck_assert_int_eq(res, 0);

    init_0((uint32_t*)test1.bits, 3);
    init_0((uint32_t*)test2.bits, 3);
    test1[0] = 1;
    test2[2] = 1;
    s21_sub(test2, test1, &result);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);

    s21_sub(test1, test2, &result);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF - 1);
    ck_assert_int_eq(result.bits[3], 1 << 31);


        for (int i = -100; i < 100; ++i) {
        for (int j = -100; j < 100; ++j) {
            int a = i;
            int b = j;
            int r;
            decimal aa;
            decimal bb;
            decimal rr;

            s21_from_int_to_decimal(a, &aa);
            //s21_from_decimal_to_int(aa, &a);
            s21_from_int_to_decimal(b, &bb);
            //s21_from_decimal_to_int(bb, &b);
            s21_sub(aa, bb, &rr);
            s21_from_decimal_to_int(rr, &r);
            if (r != a - b) {
                printf("%d != (%d) - (%d) == (%d)\n", r, a, b, a - b);
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
            //s21_from_decimal_to_float(aa, &a);
            s21_from_float_to_decimal(b, &bb);
            //s21_from_decimal_to_float(bb, &b);
            s21_sub(aa, bb, &rr);
            s21_from_decimal_to_float(rr, &r);
            if (fabsf(r - (a - b)) > 0.0001) {
                printf("%f != (%f) - (%f) == (%f)\n", r, a, b, a - b);
                ck_assert_float_eq_tol(r, a - b, 0.0001);
            }
            //printf("a - %d\n",a);
            //printf("b - %d\n",b);
            //printf("r - %d\n",r);
        }
    }
}
END_TEST

START_TEST(ADD_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1<<31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1<<31}};
    s21_decimal result;
    int res = s21_add(test1, test2, &result);
    ck_assert_int_eq(res, 2);

    test2[3] = 0;
    s21_add(test1, test2, &result);
    for (int i = 0; i < 3; ++i) ck_assert_int_eq(result.bits[i], 0);


    test1[3] = 0;
    res = s21_add(test1, test2, &result);
    ck_assert_int_eq(res, 1);

    init_0((uint32_t*)test1.bits, 3);
    init_0((uint32_t*)test2.bits, 3);
    test1[0] = 1;
    test1[3] = 1 << 31;
    test2[2] = 1;
    s21_add(test2, test1, &result);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);

    s21_negate(test1, &test1);
    s21_negate(test2, &test2);
    ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
    ck_assert_int_eq(result.bits[3], 1 << 31);
}
END_TEST

START_TEST(TO_FROM_INT) {
    decimal d;
    int check;
    for (int i = -100; i < 100; i++) {
        s21_from_int_to_decimal(i, &d);
        s21_from_decimal_to_int(d, &check);
        if (check != i) {
            printf("%d != %d\n", check, i);
            ck_assert_int_eq(check, i);
        }
    }
}
END_TEST

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
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1<<31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    int res = s21_is_greater(test1, test2);
    ck_assert_int_eq(res, 0);

    res = s21_is_greater(test2, test1);
    ck_assert_int_eq(res, 1);

    s21_negate(test1, &test1);
    res = s21_is_greater(test2, test1);
    ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(EQUAL_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1<<31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    int res = s21_is_equal(test1, test2);
    ck_assert_int_eq(res, 0);

    res = s21_is_equal(test2, test1);
    ck_assert_int_eq(res, 0);

    res = s21_is_equal(test2, test2);
    ck_assert_int_eq(res, 0);
}
END_TEST


START_TEST(MUL_TEST) {
    s21_decimal test1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1<<31}};
    s21_decimal test2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal result;
    int res = s21_mul(test1, test2, &result);
    ck_assert_int_eq(res, 2);

    s21_negate(test1, &test1);
    res = s21_mul(test1, test2, &result);
    ck_assert_int_eq(res, 1);

    init_0((uint32_t *)test2.bits, 3);
    test2.bits[0] = 1;
    setDecimalExp(&test2, 28);
    res = s21_mul(test1, test2, &result);
    ck_assert_int_eq(result.bits[3], 28 << 15);
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
            //s21_from_decimal_to_int(aa, &a);
            s21_from_int_to_decimal(b, &bb);
            //s21_from_decimal_to_int(bb, &b);
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
            //s21_from_decimal_to_float(aa, &a);
            s21_from_float_to_decimal(b, &bb);
            //s21_from_decimal_to_float(bb, &b);
            s21_mul(aa, bb, &rr);
            s21_from_decimal_to_float(rr, &r);
            if (fabsf(r - (a * b)) > 0.001) {
                printf("%f != (%f) * (%f) == (%f)\n", r, a, b, a * b);
                ck_assert_float_eq_tol(r, a * b, 0.001);
            }
            //printf("a - %d\n",a);
            //printf("b - %d\n",b);
            //printf("r - %d\n",r);
        }
    }
}
END_TEST

Suite *f_example_suite_create() {
    Suite *s1 = suite_create("Test_decimal");

    TCase *p_case = tcase_create("Core");

    tcase_set_timeout(p_case, 0);
    tcase_add_test(p_case, SUB_TEST);
    tcase_add_test(p_case, ADD_TEST);
    tcase_add_test(p_case, GREATER_TEST);
    tcase_add_test(p_case, EQUAL_TEST);
    tcase_add_test(p_case, DIV_TEST);
    tcase_add_test(p_case, MOD_TEST);
    tcase_add_test(p_case, MUL_TEST);
    tcase_add_test(p_case, NEGATE_TEST) ;
    tcase_add_test(p_case, TO_FROM_INT);
    suite_add_tcase(s1, p_case);
    return s1;
}

int main() {
    Suite *s1 = f_example_suite_create();
    // Runner
    SRunner *runner = srunner_create(s1);
    int number_failed;
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed == 0 ? 0 : 1;
}