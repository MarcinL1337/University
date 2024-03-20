#include <iostream>
#include <math.h>

class punkt {
    private:
        double x, y;
    
    public:
        punkt();
        punkt(double x, double y);
        punkt(const punkt& p);
        double getX();
        double getY();
        void setX(double val);
        void setY(double val);
        void move(double x, double y);
        void obrot(punkt a, double kat);
        void print();
};

class odcinek {
    private:
        punkt p1, p2;

    public:
        odcinek();
        odcinek(punkt p1, punkt p2);
        odcinek(const odcinek& o);
        punkt getP1();
        punkt getP2();
        void move(double x, double y);
        void obrot(punkt a, double kat);
        double dlugosc();
        bool czyNalezy(punkt a);
        void print();
};

class trojkat {
    private:
        punkt p1, p2, p3;

    public:
        trojkat();
        trojkat(punkt p1, punkt p2, punkt p3);
        trojkat(const trojkat& t);
        punkt getP1();
        punkt getP2();
        punkt getP3();
        void move(double x, double y);
        void obrot(punkt a, double kat);
        double pole();
        double obwod();
        bool czyPunktNalezy(punkt a);
        void print();
};