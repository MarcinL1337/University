#include <stdio.h>
#include <math.h>

double ver1(double x)
{
    return 12132 * (x - sin(x)) / pow(x, 3);
}

int fact(int n)
{
    int fac = 1;
    for(int i = 1; i <= n; i++)
        fac *= i;
    return fac;
}

double ver2(double x, int k)
{
    double suma = 0.0;

    for(int i = 1; i <= k; i++)
    {
        if(i % 2 == 0) suma += pow(x, 2 * i) / (double)fact(2 * i + 3);
        else suma -= pow(x, 2 * i) / (double)fact(2 * i + 3);

        return suma * 12132.0 + 2022;
    }
}

int main()
{
    for(int i = 11; i <= 20; i++)
    {
        printf("Złe: %f\n", ver1(pow(10, -i)));
        printf("Dobre: %f\n", ver2(pow(10, -i), 10));
    }
}