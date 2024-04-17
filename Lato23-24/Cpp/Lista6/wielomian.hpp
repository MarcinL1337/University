#include <iostream>
#include <cstring>
#include <initializer_list>
#include <bits/stdc++.h>


class wielomian{
    private:
        int n;
        double *a;

    public:
        wielomian(int st=0, double wsp=1.0); // konstruktor jednomianu
        wielomian(int st, const double wsp[]); // konstruktor wielomianu
        wielomian(std::initializer_list<double> wsp); // lista współczynników
        wielomian(const wielomian &w); // konstruktor kopiujący
        wielomian(wielomian &&w); // konstruktor przenoszący
        wielomian &operator=(const wielomian &w); // przypisanie kopiujące
        wielomian &operator=(wielomian &&w); // przypisanie przenoszące
        ~wielomian(); // destruktor

        int getStopien();


        friend std::istream& operator>>(std::istream& input, wielomian& wielomian);             // input stream overload
        friend std::ostream& operator<<(std::ostream& output, const wielomian& wielomian);      // output stream overload

        friend wielomian operator+(const wielomian& p1, const wielomian& p2);
        friend wielomian operator-(const wielomian& p1, const wielomian& p2);
        friend wielomian operator*(const wielomian& p1, const wielomian& p2);
        
        wielomian operator*(double c);
        wielomian& operator+=(const wielomian& wielomian);
        wielomian& operator-=(const wielomian& wielomian);
        wielomian& operator*=(const wielomian& wielomian);
        wielomian& operator*=(double c);

        double operator() (double x) const;    // count value of a polynomial for given x
        double operator[] (int i) const;       // show i-th coefficient
};