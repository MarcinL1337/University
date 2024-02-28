#include <stdio.h>
#include <math.h>

//------------------Zad_1-----------------------

double a1(double x){
	x = x + sqrt(x*x + 2022*2022);
	return 1/x;
}

double a2(double x){
	x = sqrt(x*x + 2022*2022) - x;
	return x/(2022*2022);
}

double log_a_to_base_b(double a, double b){
    return log2(a) / log2(b);
}

double b1(double x){
    x = log_a_to_base_b(x,3);
    return x - 7.0;
}

double b2(double x){
    x = x/pow(3,7);
    x = log_a_to_base_b(x,3);
    return x;
}

double c1(double x){
    return 4 * pow(cos(x),2) - 3;
}

double c2(double x){
    return cos(3*x)/cos(x);
}

void zadanie_1(){
    printf("Podpunkt (a):\n\n");
    printf("Dla x > 2^23\n");
    for (double i = pow(2,23); i < pow(2,26); i*=2){
        printf("a1(%lf) = %.50lf\n",i,a1(i));
        printf("a2(%lf) = %.50lf\n\n",i,a2(i));
    }

    printf("\nDla x < -(2^34)\n");
    for (double i = pow(2,34); i < pow(2,37); i*=2){
        printf("a1(%lf) = %.50lf\n",-i,a1(-i));
        printf("a2(%lf) = %.50lf\n\n",-i,a2(-i));
    }

    printf("\n\nPodpunkt (b):\n\n");
    for(double i = pow(3,7) - pow(2,-37); i <= pow(3,7) ; i+= pow(2,-40)) {
        printf("b1(%.30lf) = %.40lf\n",i,b1(i));
        printf("b2(%.30lf) = %.40lf\n\n",i,b2(i));
    }

    #define M_PIl 3.141592653589793238462643383279502884L

    printf("\n\n%.20lf\n\n",4*pow(cos(M_PI/6.0),2));
    printf("\n\n%.20lf\n\n",cos((M_PI/6.0) * 3));
    printf("\n\nPodpunkt (c):\n\n");
    for(double i = M_PI/6.0 - pow(2,-50); i <= M_PI/6.0; i+= pow(2,-52)) {
        printf("c1(%.30lf) = %.60lf\n",i,c1(i));
        printf("c2(%.30lf) = %.60lf\n\n",i,c2(i));
    }
}

//------------------Zad_2-----------------------

void M_zerowe1(double a, double b, double c){
    double x1 = (-b + sqrt(b*b - 4*a*c)) / 2*a*c;
    double x2 = (-b - sqrt(b*b - 4*a*c)) / 2*a*c;
    printf("x1 = %.20lf\nx2 = %.20lf\n\n",x1,x2);
}

void M_zerowe2(double a, double b, double c) {
    double x1,x2;
    if (b > 0) {
        x1 = (-2*c)/(b+sqrt(b*b-4*a*c));
        x2 = (-b - sqrt(b * b - 4 * a * c)) / 2 * a * c;
    }
    else {
        x1 = (-b + sqrt(b * b - 4 * a * c)) / 2 * a * c;
        x2 = (2 * c) / (-b + sqrt(b * b - 4 * a * c));
    }
        printf("x1 = %.20lf\nx2 = %.20lf\n\n", x1, x2);
}

void zadanie_2(){
    double a = 1e-10;
    double b = 1e10;
    double c = 1;

    printf("a = %.20lf, b = %lf, c= %.20lf\n",a,b,c);
    printf("Metoda szkolna:\n");
    M_zerowe1(a,b,c);
    printf("Metoda lepsza:\n");
    M_zerowe2(a,b,c);

    b = -b;
    printf("a = %.20lf, b = %lf, c= %.20lf\n",a,b,c);
    printf("Metoda szkolna:\n");
    M_zerowe1(a,b,c);
    printf("Metoda lepsza:\n");
    M_zerowe2(a,b,c);
}

void zadanie(int x){
    switch (x){
        case 1:
            zadanie_1();
            break;
        case 2:
            zadanie_2();
            break;
        default:
            printf("Niepoprawny numer zadania");
    }

}

int main() {
    zadanie(1);
    return 0;
}