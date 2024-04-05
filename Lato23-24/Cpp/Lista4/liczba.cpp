#include "liczba.hpp"


liczba::liczba(): liczba(0.0) {} 


liczba::liczba(double val){
    this->history = new double[history_length];
    history[0] = val;
    this->idx = 0;
    this->no_of_elems = 1;
}


liczba::liczba(liczba &inna_liczba){                //konstruktor kopiujący
    this->history = new double[history_length];
    history[0] = inna_liczba.get_current();
    this->idx = 0;
    this->no_of_elems = 1;
}


liczba &liczba::operator=(liczba &inna_liczba){
    insert(inna_liczba.get_current());
    return *this;
}

liczba::liczba(liczba &&inna_liczba){               //konstruktor przenoszący
    this->history = inna_liczba.history;
    this->idx = inna_liczba.idx;
    this->no_of_elems = inna_liczba.no_of_elems;
    inna_liczba.history = nullptr;
    inna_liczba.idx = -1;
    inna_liczba.no_of_elems = 0;
}


liczba &liczba::operator=(liczba &&inna_liczba){
    this->history = inna_liczba.history;
    this->idx = inna_liczba.idx;
    this->no_of_elems = inna_liczba.no_of_elems;
    inna_liczba.history = nullptr;
    inna_liczba.idx = -1;
    inna_liczba.no_of_elems = 0;
    return *this;
}


liczba::~liczba(){
    delete[] this->history;
}


double liczba::get_current(){
    return this->history[this->idx];
}


void liczba::insert(double n){
    this->idx = (this->idx + 1) % history_length;
    this->history[this->idx] = n;
    this->no_of_elems = this->no_of_elems == history_length ? history_length : no_of_elems+1;
}


void liczba::print_history(){
    if(this->no_of_elems < history_length){
        for(int i = 0; i < this->idx; i++){
            std::cout << "Pozycja " << this->no_of_elems-i << " od końca: " << history[i] << std::endl;
        }
    }
    else{
        int temp_idx = this->idx;
        for(int i = 0; i < history_length-1; i++){
            temp_idx = (temp_idx + 1) % history_length;
            std::cout << "Pozycja " << history_length-i << " od końca: " << history[temp_idx] << std::endl;
        }
    }
}