#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void foo()
{
    double wynik = 0.0;
    for(int i = 0; i <= 2 * pow(10, 6); i++)
        wynik += pow(-1.0, i)/(2.0 * i + 1);

    printf("Pi: %lf", wynik*4);
}

int main()
{
    foo();
}