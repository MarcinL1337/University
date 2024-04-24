#include <iostream>
#include <cctype>


class kolor {
    protected:
        unsigned short r,g,b;

    public:
        kolor();
        kolor(unsigned short, unsigned short, unsigned short);

        void jasnosc(double);
        unsigned short polacz();

        unsigned short get_r();
        unsigned short get_g();
        unsigned short get_b();
        
        void set_r(unsigned short);
        void set_g(unsigned short);
        void set_b(unsigned short);
};


class kolortransparentny : public virtual kolor {
    protected:
        unsigned short alpha;  
        
    public:
        kolortransparentny();
        kolortransparentny(unsigned short, unsigned short, unsigned short, unsigned short);

        unsigned short get_alpha();
        void set_alpha(unsigned short);
};


class kolornazwany : public virtual kolor {
    public:
        std::string nazwa;

    public:
        kolornazwany(unsigned short, unsigned short, unsigned short, std::string);
        
        std::string get_nazwa();
        void set_nazwa(std::string);
};


class kolortn : public kolortransparentny, public kolornazwany {
    public:
        kolortn(unsigned short, unsigned short, unsigned short, std::string, unsigned short);
};


