#include <iostream>
#include <math.h>

static uint32_t history_length = 3;

class liczba{
    private:
        double *history;
        uint32_t idx, no_of_elems;

    public:
        liczba();
        liczba(double val);
        liczba(liczba &inna_liczba);
        liczba &operator=(liczba &inna_liczba);
        liczba(liczba &&inna_liczba);
        liczba &operator=(liczba &&inna_liczba);

        ~liczba();

        void insert(double n);
        void print_history();
        double get_current();
};