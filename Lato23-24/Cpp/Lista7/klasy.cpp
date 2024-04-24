#include "klasy.hpp"

kolor::kolor() : kolor(0, 0, 0) {}


kolor::kolor(unsigned short r, unsigned short g, unsigned short b){
    if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255){
        throw std::invalid_argument("Wartości rgb muszą być z przedziału [0, 255]\n");
        exit(EXIT_FAILURE);
    }
    
    this->r = r;
    this->g = g;
    this->b = b;
}


void kolor::jasnosc(double amount){
    this->r = this->r * amount > 255 ? 255 : this->r * amount < 0 ? 0 : this->r * amount;
    this->g = this->g * amount > 255 ? 255 : this->g * amount < 0 ? 0 : this->g * amount;
    this->b = this->b * amount > 255 ? 255 : this->b * amount < 0 ? 0 : this->b * amount;
}


unsigned short kolor::polacz(){
    return (this->r + this->g + this->b) / 3;
}


unsigned short kolor::get_r(){ return this->r; }
unsigned short kolor::get_g(){ return this->g; }
unsigned short kolor::get_b(){ return this->b; }


void kolor::set_r(unsigned short new_r){ this->r = new_r; }
void kolor::set_g(unsigned short new_g){ this->g = new_g; }
void kolor::set_b(unsigned short new_b){ this->b = new_b; }


kolortransparentny::kolortransparentny() : kolortransparentny(0, 0, 0, 255){}


kolortransparentny::kolortransparentny(unsigned short r, unsigned short g, unsigned short b, unsigned short transparency) : kolor(r,g,b) {
    if(transparency < 0 || transparency > 255){
        throw std::invalid_argument("Wartość alpha musi być z przedziału [0, 255]");
        exit(EXIT_FAILURE);
    }
    this->alpha = transparency;
}


unsigned short kolortransparentny::get_alpha() { return this->alpha; }


void kolortransparentny::set_alpha(unsigned short new_alpha) { this->alpha = new_alpha; }


std::string kolornazwany::get_nazwa() { return this->nazwa; }


void kolornazwany::set_nazwa(std::string nowa_nazwa) { this->nazwa = nowa_nazwa; }


kolornazwany::kolornazwany(unsigned short r = 0, unsigned short g = 0, unsigned short b = 0, std::string nazwa = "") : kolor(r,g,b) {
    for(int i = 0; i < nazwa.length(); i++){
        if(!isalpha(nazwa[i])){ //(char)?
            throw std::invalid_argument("Nazwa koloru musi składać się jedynie z liter alfabetu.");
            exit(EXIT_FAILURE);
        }
        nazwa[i] = tolower(nazwa[i]);
    }
    this->nazwa = nazwa;
}


kolortn::kolortn(unsigned short r = 0, unsigned short g = 0, unsigned short b = 0, std::string nazwa = "", unsigned short transparency = 255) : kolor(r, g, b) {
    if(transparency < 0 || transparency > 255){
        throw std::invalid_argument("Wartość alpha musi być z przedziału [0, 255]");
        exit(EXIT_FAILURE);
    }
    this->alpha = transparency;

    for(int i = 0; i < nazwa.length(); i++){
        if(!isalpha(nazwa[i])){ //(char)?
            throw std::invalid_argument("Nazwa koloru musi składać się jedynie z liter alfabetu.");
            exit(EXIT_FAILURE);
        }
        nazwa[i] = tolower(nazwa[i]);
    }
    this->nazwa = nazwa;
}