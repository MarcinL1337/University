#include "kolejka.hpp"


kolejka::kolejka() : kolejka(1)
{}


kolejka::kolejka(int pojemnosc){
    if(pojemnosc < 1){
        throw std::invalid_argument("Pojemność nie może być mniejsza od 1!");
        exit(EXIT_FAILURE);
    }

    this->pojemnosc = pojemnosc;
    this->tab = new std::string[pojemnosc];
}


kolejka::kolejka(kolejka &kolejka2){        //konstruktor kopiujący
    pojemnosc = kolejka2.pojemnosc;
    poczatek = kolejka2.poczatek;
    ile = kolejka2.ile;
    tab = new std::string[pojemnosc];
    for(int i = 0; i < ile; i++){
        tab[(poczatek+i)%pojemnosc] = kolejka2.tab[(poczatek+i)%pojemnosc];
    }
}


kolejka &kolejka::operator=(kolejka &kolejka2){
    pojemnosc = kolejka2.pojemnosc;
    poczatek = kolejka2.poczatek;
    ile = kolejka2.ile;
    tab = new std::string[pojemnosc];
    for(int i = 0; i < ile; i++){
        tab[(poczatek+i)%pojemnosc] = kolejka2.tab[(poczatek+i)%pojemnosc];
    }
    return *this;
}


kolejka::kolejka(kolejka &&kolejka2){       //konstruktor przenoszący
    pojemnosc = kolejka2.pojemnosc;
    poczatek = kolejka2.poczatek;
    ile = kolejka2.ile;
    tab = new std::string[pojemnosc];
    for(int i = 0; i < ile; i++){
        tab[(poczatek+i)%pojemnosc] = kolejka2.tab[(poczatek+i)%pojemnosc];
    }
    kolejka2.pojemnosc = 0;
    kolejka2.poczatek = 0;
    kolejka2.ile = 0;
    kolejka2.tab = nullptr;
}

kolejka &kolejka::operator=(kolejka &&kolejka2){
    pojemnosc = kolejka2.pojemnosc;
    poczatek = kolejka2.poczatek;
    ile = kolejka2.ile;
    tab = new std::string[pojemnosc];
    for(int i = 0; i < ile; i++){
        tab[(poczatek+i)%pojemnosc] = kolejka2.tab[(poczatek+i)%pojemnosc];
    }
    kolejka2.pojemnosc = 0;
    kolejka2.poczatek = 0;
    kolejka2.ile = 0;
    kolejka2.tab = nullptr;
    return *this;
}


kolejka::kolejka(std::initializer_list<std::string> lista){
    pojemnosc = lista.size();
    tab = new std::string[pojemnosc];
    ile = pojemnosc;
    poczatek = 0;
    int i = 0;
    for(auto slowo : lista){
        tab[i] = slowo;
        i++;
    }
}


kolejka::~kolejka(){
    delete[] this->tab;
}


void kolejka::wstaw(std::string tekst){
    if(this->ile < this->pojemnosc){
        this->tab[(poczatek + ile) % pojemnosc] = tekst;
        ile++;
    }
    else{
        this->tab[poczatek] = tekst;
        poczatek = (poczatek + 1) % pojemnosc;
    }
}


std::string kolejka::usun(){
    if(ile > 1){
        std::string ret = tab[poczatek];
        tab[poczatek] = "";
        poczatek = poczatek == pojemnosc - 1 ? 0 : poczatek + 1;
        ile--;
        return ret;
    }
    else if(ile > 0){
        std::string ret = tab[poczatek];
        tab[poczatek] = "";
        poczatek = 0;
        ile--;
        return ret;
    }
    else{
        throw std::invalid_argument("Nie można usunąć z pustej kolejki!");
        exit(EXIT_FAILURE);
    }
}


std::string kolejka::zprzodu(){
    return tab[poczatek];
}


int kolejka::dlugosc(){
    return this->ile;
}


void kolejka::wypisz(){
    for(int i = 0; i < ile; i++){
        std::cout << tab[(poczatek + i) % pojemnosc] << " -> ";
    }
    std::cout << std::endl << std::endl;
}