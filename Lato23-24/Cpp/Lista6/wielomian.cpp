#include "wielomian.hpp"

wielomian::wielomian(int degree, double coefficient){
    n = degree;
    a = new double[n + 1];

    a[0] = coefficient;
}


wielomian::wielomian(int degree, const double coefficient[]){
    n = degree;
    a = new double[n + 1];

    for (int i = degree; i >= 0; i--)
        a[i] = coefficient[i];
}


wielomian::wielomian(std::initializer_list<double> coefficient){
    n = coefficient.size()-1;
    a = new double[n+1];

    int i = 0;
    
    for (auto x : coefficient){
        a[i] = x;
        i++;
    }

}


wielomian::wielomian(const wielomian& wielomian){
    n = wielomian.n;
    a = new double[n+1];
    for(int i = 0; i < n+1; i++)
        a[i] = wielomian.a[i];
}


wielomian::wielomian(wielomian&& wielomian){
    n = wielomian.n;
    a = wielomian.a;
    wielomian.n = 0;
    wielomian.a = nullptr;
}


wielomian & wielomian::operator=(const wielomian& wielomian){
    delete[] a;

    n = wielomian.n;
    a = new double[n+1];
    for(int i = 0; i < n+1; i++)
        a[i] = wielomian.a[i];

    return *this;
}


wielomian & wielomian::operator=(wielomian&& wielomian){
    delete[] a;
    n = wielomian.n;
    a = wielomian.a;
    wielomian.n = 0;
    wielomian.a = nullptr;

    return *this;
}


wielomian::~wielomian(){
    delete[] a;
}


std::ostream& operator<<(std::ostream& output, const wielomian& wielomian){
    for (int i = wielomian.n; i >= 0; i--)
    {
        output << wielomian.a[i] << "x^" << i << " "; 
    }

    return output;
}


std::istream& operator>>(std::istream& input, wielomian& wielomian){
    std::cout << "Stopien: ";
    input >> wielomian.n;
    std::cout << "Współczynniki: [" << wielomian.n + 1 << "] ";
    wielomian.a = new double [wielomian.n + 1];
    for(int i = 0; i < wielomian.n + 1; i++)
        input >> wielomian.a[i];
    return input;
}


wielomian operator+(const wielomian& p1, const wielomian& p2){
    int maxStopien = p1.n > p2.n ? p1.n : p2.n;
    wielomian p3(maxStopien, 0.0);
    for(int i = 0; i < p1.n + 1; i++)
        p3.a[i] = p1.a[i];
    for(int i = 0; i < p2.n + 1; i++)
        p3.a[i] += p2.a[i]; 
    return p3;
}


wielomian operator-(const wielomian& p1, const wielomian& p2){
    int maxStopien = p1.n > p2.n ? p1.n : p2.n;
    wielomian p3(maxStopien, 0.0);
    for(int i = 0; i < p1.n + 1; i++)
        p3.a[i] = p1.a[i];
    for(int i = 0; i < p2.n + 1; i++)
        p3.a[i] -= p2.a[i]; 
    return p3;
}


wielomian operator*(const wielomian& p1, const wielomian& p2){
    int maxStopien = p1.n + p2.n;
    wielomian p3(maxStopien, 0.0);
    for(int i = 0; i < p1.n+1; i++){
        for(int j = 0; j < p2.n+1; j++)
            p3.a[i+j] += p1.a[i] * p2.a[j];
    }
    return p3;
}


wielomian wielomian::operator*(double c){
    wielomian p(*this);
    for(int i = 0; i < n+1; i++)
        p.a[i] *= c;
    return p;
}


wielomian& wielomian::operator+=(const wielomian& p){
    int maxStopien = n > p.n ? n : p.n;
    double *a2 = new double[maxStopien+1];
    for(int i = 0; i < maxStopien+1; i++)
        a2[i] = 0.0;
    for(int i = 0; i < n+1; i++) a2[i] = a[i];
    for(int i = 0; i < p.n+1; i++) a2[i] += p[i];
    delete[] a;
    n = maxStopien;
    a = a2;
    return *this;
}


wielomian& wielomian::operator-=(const wielomian& wielomian){
    int maxStopien = n > wielomian.n ? n : wielomian.n;
    double *a2 = new double[maxStopien+1];
    for(int i = 0; i < maxStopien+1; i++)
        a2[i] = 0.0;
    for(int i = 0; i < n+1; i++) a2[i] = a[i];
    for(int i = 0; i < wielomian.n+1; i++) a2[i] -= wielomian[i];
    delete[] a;
    n = maxStopien;
    a = a2;
    return *this;
}


wielomian& wielomian::operator*=(const wielomian& wielomian){
    int maxStopien = n + wielomian.n;
    double *a2 = new double[maxStopien+1];
    for(int i = 0; i < maxStopien+1; i++)
        a2[i] = 0.0;
    for(int i = 0; i < n+1; i++){
        for(int j = 0; j < n+1; j++)
            a2[i+j] += a[i]*wielomian.a[j];
    }
    delete[] a;
    n = maxStopien;
    a = a2;
    return *this;
}


wielomian& wielomian::operator*=(double c){
    for(int i = 0; i < n+1; i++)
        a[i] *= c;
    return *this;
}


double wielomian::operator()(double x) const{
    double res = a[n];
    for(int i = n-1; i >= 0; i--)
        res = res*x + a[i];

    return res;
}


double wielomian::operator[](int idx) const{
    return a[idx];
}


int wielomian::getStopien(){
    return this->n;
}