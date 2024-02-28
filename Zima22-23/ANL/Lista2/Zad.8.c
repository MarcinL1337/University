#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double ver1(double x)
{
    x = pow(x, 13.0);
    return 4044 * (sqrt(x + 1) - 1) / x;
}

double ver2(double x)
{
    x = pow(x, 13.0);
    return 4044 / (sqrt(x + 1) + 1);
}

int main()
{
    for(int i = 3; i <= 12; i++)
    {
        printf("Złe: %f\n", ver1(pow(10, -i)));
        printf("Dobre: %f\n", ver2(pow(10, -i)));
    }
}