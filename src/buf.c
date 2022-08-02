//
// Created by Gladis Ariane on 7/30/22.
//

#include <stdlib.h>
#include "s21_decimal.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#define N 79228162514264337593543950336

unsigned long divu10(unsigned long n) {
    unsigned long q, r;
    q = (n >> 1) + (n >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q >> 3;
    r = n - (((q << 2) + q) << 1);
    return q + (r > 9);
}
void print_binary(int *arr, int size) {
    unsigned int tmp = 0;
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 31; j >= 0; j--) {
	    tmp = arr[i] & (1 << j);
	    tmp >>= j;
            printf("%u", tmp);
        }
	printf(" ");
    }
    puts("");
}

int main() {

    uint32_t a[4];
    a[0] = 0;
    a[1] = 1;
    a[2] = 0xFFFFFFFF;
    a[3] = 2;
    print_binary(a, 4);
    a[0] = 0xFFFFFFFE;
    a[1] = 0xF0000000;
    a[2] = 0xFFFFFFFE;
    a[3] = 0xFFFFFFFE;
    print_binary(a, 4);
    int x = 31;
    printf("%u\n", (a[3] & (1 << x)) >> x);
    /* FOR (int i = 0; i < 4; i++) { */
    /* 	printf("%u\n", a[i]); */
    /* } */
    shiftl1(a, 4);
    printf("%u\n", 0xFFFFFFFF & (1 << 31));
    printf("%u\n", 2147483648 >> 31);
    s21_decimal aa = {1, 0, 0, 0};
    s21_decimal b = {0, 0, 0, 0};
    printf("is less%d\n", s21_is_less(aa, b));
    printf("is less%d\n", s21_is_less(b, aa));
    //uint32_t x[7] = {0};
    //div10(x,7);
    //uint32_t a = 60;
    //a -= a >> 1;
    //a -= a >> 1;
    ////a -= a >> 1;
    //printf("%u\n", a);
    //a = a >> 1;
    //printf("%u\n", a);

    //printBits(4,&a,4);
    //shiftr1(&a,1);
    //printBits(4,&a,4);
    //int a = -39;
    //int b = -7;
    //int r = 0;
    //decimal aa;
    //decimal bb;
    //decimal rr;
    ////init_0(&rr.bits,4);
    ////printBits(sizeof(decimal),&rr,4);
    ////setDecimalExp(&rr,0b11111111);
    ////printBits(sizeof(decimal),&rr,4);
    ////int exp = getDecimalExp(rr);
    ////printBits(4,&exp,4);
    //s21_from_int_to_decimal(a, &aa);
    //s21_from_decimal_to_int(aa, &a);
    //s21_from_int_to_decimal(b, &bb);
    //s21_from_decimal_to_int(bb, &b);
    //s21_sub(aa,bb,&rr);
    //
    //
    //printDecimal(&aa);
    //printDecimal(&bb);
    //printDecimal(&rr);
    //s21_from_decimal_to_int(rr, &r);
    //printf("a - %d\n",a);
    //printf("b - %d\n",b);
    //printf("r - %d\n",r);
}
