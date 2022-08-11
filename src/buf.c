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
    uint32_t check= UINT32_MAX/2-1;
    uint32_t res = check*2+1;
    printf("%u\n",res);
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


uint64_t div5exact(uint64_t n)
{
    // returns n/5 as long as n actually divides 5
    // (because 'n * (INV5 * 5)' == 'n * 1' mod 2^32

#define INV5 0xcccccccccccccccd

    return n * INV5;

}

unsigned int divides5(unsigned int n)
{
    unsigned int q = div5exact(n);
    if (q <= 0x33333333) /* q*5 < 2^32? */
    {
        /* q*5 doesn't overflow, so n == q*5 */
        return 1;
    }
    else
    {
        /* q*5 overflows, so n != q*5 */
        return 0;
    }
}

int divides2(unsigned int n)
{
    /* easy divisibility by 2 test */
    return (n & 1) == 0;
}

int divides10(unsigned int n)
{
    return divides2(n) && divides5(n);
}


/* fast one-liner: */
#define DIVIDES10(n) ( ((n) & 1) == 0 && ((n) * 0xcccccccd) <= 0x33333333 )

int main() {
    test();
    //Bar();
    //ud10();
    //printf("%lu\n",div5exact(10));
    //d10();
}
