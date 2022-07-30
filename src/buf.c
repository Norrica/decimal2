//
// Created by Gladis Ariane on 7/30/22.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

int main() {
    //printf("%d\n",INT32_MAX);
    //printf("%f\n",FLT_MAX_10_EXP);
    //def fun():
    //    a = 340282346638528859811704183484516925440.000000
    //    b =           79228162514264337593543950335
    //    if(convert_to_int(a))
    //        decimal+=a
    //    else
    //        return fun(a/2)+fun(a/2)

    double d = 0;
    float num = 56.67;
    printf("%.5f\n", num);
    char ch[100];
    sprintf(ch, "%.5f", num);
    int exp = strlen(ch) - (strchr(ch, '.') - ch) - 1;
    //puts(ch);
    //printf("%d\n",exp);
    for (int i = strlen(ch) - exp - 1; i < strlen(ch); ++i) {
        ch[i] = ch[i + 1];
    }
    puts(ch);
    int res = atoi(ch);

    printf("%d\n",exp);
    printf("%d\n",res);
}