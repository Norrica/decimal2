//
// Created by Gladis Ariane on 7/31/22.
//
#include <check.h>
#include <stdio.h>
#include "s21_decimal.h"

START_TEST(sub_test) {
    for (int i = 0; i < 1200; ++i) {
        for (int j = 0; j < 1200; ++j) {
            int a = i;
            int b = j;
            int r;
            decimal aa;
            decimal bb;
            decimal rr;

            s21_from_int_to_decimal(a, &aa);
            s21_from_decimal_to_int(aa, &a);
            s21_from_int_to_decimal(b, &bb);
            s21_from_decimal_to_int(bb, &b);
            s21_sub(aa, bb, &rr);
            s21_from_decimal_to_int(rr, &r);
            if (r != a - b) {
                printf("%d != (%d) - (%d) == (%d)\n", r, a, b, a - b);
                //ck_assert_int_eq(r, a - b);
            }
        }
    }
    //printf("a - %d\n",a);
    //printf("b - %d\n",b);
    //printf("r - %d\n",r);
}
END_TEST

Suite *f_example_suite_create() {
    Suite *s1 = suite_create("Test_decimal");

    TCase *SubTest = tcase_create("sub_test");

    suite_add_tcase(s1, SubTest);
    tcase_add_test(SubTest, sub_test);
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