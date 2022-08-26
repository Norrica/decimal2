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