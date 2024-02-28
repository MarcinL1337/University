#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double zad2(double x)
{
    return 12123 * (x - sin(x)) / pow(x, 3);
}

int main()
{
    for(int i = 11; i <= 20; i++)
    {
        printf("%lf\n", zad2(pow(10, -i)));
    }
}