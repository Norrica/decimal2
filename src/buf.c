//
// Created by Gladis Ariane on 9/9/22.
//
#include <float.h>
#include <stdio.h>

#include "s21_arrays.h"
#include "s21_decimal.h"

//void printBits(const size_t size, const void *ptr, int sep_n) {
//  unsigned char *b = (unsigned char *)ptr;
//  unsigned char byte;
//  int i, j;
//
//  for (i = size - 1; i >= 0; i--) {
//    for (j = 7; j >= 0; j--) {
//      byte = (b[i] >> j) & 1;
//      printf("%u", byte);
//    }
//    if (!(i % sep_n)) printf(" ");
//  }
//  puts("");
//}
//void bar() {
//  s21_decimal x = {{20, 0, 0, 0}};
//  s21_decimal y = {{21, 0, 0, 0}};
//  s21_decimal z = {{0, 0, 0, 0}};
//  setDecimalSign(&x, 1);
//  //setDecimalSign(&y, 1);
//  s21_add(x, y, &z);
//  printBits(16, &x, 12);
//  printBits(16, &y, 12);
//  printBits(16, &z, 12);
//}
//void foo() {
//  uint32_t a[] = {0, 0, 0, 0};
//  char str[] = "79228162514264337593543950335";
//  for (size_t i = 0; i < strlen(str); ++i) {
//    mul10(a, 4);
//    bit_add(a, str[i] - '0', 4);
//  }
//  printBits(16, a, 4);
//}
//
//void baz(){
//  decimal dst;
//  float a = 1;
//  float x;
//  float res;
//  for (int i = 0; i < 100; ++i) {
//    x = (float)rand() / (float)(RAND_MAX / a);
//    s21_from_float_to_decimal(x, &dst);
//    s21_from_decimal_to_float(dst, &res);
//    printf("%.8f\t%.8f\n", x, res);
//  }
//}
//
//void bif(){
//  float a = 0.0f;
//  float res;
//  decimal dst;
//  int err = s21_from_float_to_decimal(a,&dst);
//  s21_from_decimal_to_float(dst,&res);
//  printBits(16,&dst,12);
//  printf("%.10f\n",res);
//  printf("%d\n",err);
//}

void negate(){
  decimal a={0,0,0,0};
  //decimal *b = NULL;
  int err =s21_negate(a,&a);
  if (err ==1){
    puts("lol");
  }
  //printf("%d\n",err);
  //printBits(16,&a,12);
}

void dif(){
  s21_decimal x = {{0, 0, 555555555, 0}};
  s21_decimal y = {{1, 0, 0, 0}};
  setDecimalExp(&y, 1);
  s21_decimal z = {{0, 0, 0, 0}};
  s21_mul(x, y, &z);
  //printf("%d\n",n1);
  //printf("%u\n",z.bits[0]);
  printBits(16,&z,12);
}

void f(){
  s21_decimal x = {{1050000, 0, 0, 0}};
  //s21_decimal y = {{3, 0, 0, 0}};
  //s21_decimal z = {{0, 0, 0, 0}};
  //setDecimalExp(&y, 20);
  //s21_div(x, y, &z);
  //div10(z.bits,3);
  //printf("%u\n",z.bits[0]);
  int scale = 6;
  reduce_scale_arr(x.bits,3,&scale);
  setDecimalExp(&x,scale);
  printBits(16,&x,2);
}

int main() {
   dif();

}
