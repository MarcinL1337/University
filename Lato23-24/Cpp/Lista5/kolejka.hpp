#include <iostream>
#include <cstring>
#include <initializer_list>
#include <bits/stdc++.h>


class kolejka{
    private:
        int pojemnosc, poczatek = 0, ile = 0;
        std::string *tab;

    public:
        kolejka();
        kolejka(int pojemnosc);

        kolejka(kolejka &kolejka2);
        kolejka &operator=(kolejka &kolejka2);
        
        kolejka(kolejka &&kolejka2);
        kolejka &operator=(kolejka &&kolejka2);

        kolejka(std::initializer_list<std::string>);

        ~kolejka();

        void wstaw(std::string);
        std::string usun();
        std::string zprzodu();
        int dlugosc();
        void wypisz();
};