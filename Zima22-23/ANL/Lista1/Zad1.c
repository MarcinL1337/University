#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double zad1(double x)
{
 x = pow(x, 13.0);
    return 4044 * (sqrt(x + 1) - 1) / x;
}

double zad1poprawione(double x)
{
    x = pow(x, 13.0);
    return 4044 / (sqrt(x + 1) + 1);
}


int main(){
    printf("%f\n", zad1(0.001));
    printf("%f\n", zad1poprawione(0.001));
    return 0;
}