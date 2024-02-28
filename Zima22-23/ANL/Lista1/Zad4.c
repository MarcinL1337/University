#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void foo()
{
    printf("nieparzyste:\n");
    double i = log(2023.0/2022.0);
    for(int j = 1; j <= 19; j += 1)
    {
        i = 1.0/(double)j - 2022.0 * i;
        if(j % 2 == 1) printf("%lf\n", i);
    }

    printf("\nparzyste:\n");
    i = log(2023.0/2022.0);
    for(int j = 1; j <= 19; j += 1)
    {
        i = 1.0/(double)j - 2022.0 * i;
        if(j % 2 == 0) printf("%lf\n", i);
    }
}

int main()
{
    foo();

    return 0;
}