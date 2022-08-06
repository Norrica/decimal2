//
// Created by Gladis Ariane on 7/31/22.
//
#include <check.h>
#include <stdio.h>
#include "s21_decimal.h"

START_TEST(SUB_TEST) {
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

START_TEST(TO_FROM_INT) {
    decimal d;
    int check;
    for (int i = INT32_MIN; i <= INT32_MAX; ++i) {
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
    decimal d;
    int check;
    for (int i = -100; i < 100; ++i) {
        s21_from_int_to_decimal(i, &d);
        s21_negate(d, &d);
        s21_from_decimal_to_int(d, &check);
        ck_assert_int_eq(-i, check);
    }
}
END_TEST

START_TEST(MUL_TEST) {
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
                //ck_assert_int_eq(r, a * b);
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
            if (fabsf(r - (a * b)) > 0.0001) {
                printf("%f != (%f) & (%f) == (%f)\n", r, a, b, a * b);
                //ck_assert_float_eq_tol(r, a * b, 0.0001);
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
    //tcase_add_test(p_case, SUB_TEST);
    //tcase_add_test(p_case, MUL_TEST);
    //tcase_add_test(p_case, NEGATE_TEST) ;
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