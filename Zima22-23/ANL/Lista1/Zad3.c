#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double foo(int x)
{
    if(x == 0) return 1.0;

    else if(x == 1) return -1.0*(1.0/7.0);

    return ((146.0/7.0) * foo(x-1) + 3 * foo(x-2));
}

int main()
{
    for(int i = 2; i <= 40; i++)
    {
        printf("%lf\n", foo(i));
    }
    return 0;
}