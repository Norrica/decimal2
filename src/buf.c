//
// Created by Gladis Ariane on 7/30/22.
//

#include <stdlib.h>
#include "s21_decimal.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

void Foo();


void test(){

    decimal d={{0}};
    printBits(16,&d,4);
    setDecimalSign(&d,1);
    printBits(16,&d,4);
    setDecimalExp(&d,28);
    printBits(16,&d,4);


}




uint64_t divu10(uint64_t n) {
    uint64_t q, r,tmp;
    q = (n >> 1) + (n >> 2);

    printBits(8,&q,4);
    q += (q >> 4);
    printBits(8,&q,4);
    q += (q >> 8);
    printBits(8,&q,4);
    q += (q >> 16);
    printBits(8,&q,4);

    q = q >> 3;
    printBits(8,&q,4);
    tmp = q + (q << 2);
    printBits(8,&tmp,4);
    r = n - (tmp << 1);
    printBits(8,&r,4);
    //printf("%u\n",r);
    uint64_t res = q + (r > 9);
    printBits(8,&res,4);
    return res;
}
void ud10(){
    uint64_t i = 30;
    printBits(8,&i,4);
    //printf("%lu\n",i);
    uint64_t r= divu10(i);
    printBits(8,&r,4);
    //printf("%lu\n",r);
}

void d10(){
    decimal d = {0,UINT32_MAX/2,0,0};
    printBits(8,&d,4);
    div10((uint32_t*)d.bits,3);
    printBits(8,&d,4);
    printf("%lu\n",*(d.bits));
}

void check_test(){
    decimal d1 = {{UINT32_MAX,UINT32_MAX,UINT32_MAX,0}};
    setDecimalExp(&d1,20);
    decimal d2 = {{UINT32_MAX,UINT32_MAX,0,0}};
    setDecimalExp(&d2,20);
    decimal d3;
    s21_add(d1,d2,&d3);
    printBits(16,&d3,4);
}


int main() {
    test();
    //Bar();
    //ud10();
    //printf("%lu\n",div5exact(10));
    //d10();
}
