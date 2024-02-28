#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double foo(double x) {
    return pow(x, 3) + pow(x, 2) - x + 5;
}

double bar(double x) {
    return (pow(x, 5) + log(x + 5));
}

double fis(double x) {
    return (pow((x + 2), 3) - pow(log(x + 3), 2));
}

void Zad8() {
    for (double i = 1.0e-1; i >= 1.0e-7; i *= 1.0e-1) {
        double number = i;
        printf("%g\n", number);
        printf("Function foo: %g\n", (foo(number) - foo(0)) / number);
        printf("Function bar: %g\n", (bar(number) - bar(0)) / number);
        printf("Function fis: %g\n", (fis(number) - fis(0)) / number);
        printf("--------------------------------------\n");
        printf("Function foo: %g\n", (foo(number) - foo(-number)) / (2 * number));
        printf("Function bar: %g\n", (bar(number) - bar(-number)) / (2 * number));
        printf("Function fis: %g\n", (fis(number) - fis(-number)) / (2 * number));
        printf("\n\n");
    }
    printf("(foo)' = -1\n");
    printf("(bar)' = 0.2\n");
    printf("(fis)' = 11.267591807554593\n");
}

int main() {
    Zad8();
    return 0;
}