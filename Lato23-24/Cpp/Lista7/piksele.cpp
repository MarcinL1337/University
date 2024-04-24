#include "piksele.hpp"


piksel::piksel() : piksel(0, 0){}


piksel::piksel(int x, int y){
    if(x < 0 || szer <= x || y < 0 || wys <= y)
        throw std::invalid_argument("Piksele wychodzą poza ekran!");
    this->x = x;
    this->y = y;
}


int piksel::get_x(){ return this->x; }


int piksel::get_y(){ return this->y; }


int piksel::calc_dist_left() { return this->x; }


int piksel::calc_dist_right() { return (szer - x - 1); }


int piksel::calc_dist_up() { return this->y; }


int piksel::calc_dist_down() { return (wys - y - 1); }


float piksel::calc_dist(int x, int y) {
    return abs(sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y)));
}


int pixel_distance(piksel &p1, piksel &p2){
    return (int)(p1.calc_dist(p2.get_x(), p2.get_y()));
}


int pixel_distance(piksel *p1, piksel *p2){
    return (int)(p1->calc_dist(p2->get_x(), p2->get_y()));
}


pikselkolorowy::pikselkolorowy() : pikselkolorowy(0, 0, ""){}

pikselkolorowy::pikselkolorowy(int x = 0, int y = 0, std::string nazwa = ""){
    if(x < 0 || szer <= x || y < 0 || wys <= y)
        throw std::invalid_argument("Piksele wychodzą poza ekran!");
    
    this->x = x;
    this->y = y;
    this->kolor = nazwa;
}


void pikselkolorowy::przesun(int dx, int dy){
    this->x += dx;
    this->y += dy;
    if(x < 0 || szer <= x || y < 0 || wys <= y)
        throw std::invalid_argument("Piksele wychodzą poza ekran!");
}


