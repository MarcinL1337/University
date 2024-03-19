#include "figury.hpp"


punkt::punkt(){
    this->x = 0;
    this->y = 0;
}


punkt::punkt(double x, double y): x(x), y(y) {}


punkt::punkt(const punkt& p){
    this->x = p.x;
    this->y = p.y;
}


double punkt::getX() { return this->x; }
double punkt::getY() { return this->y; }
void punkt::setX(double val){ this->x = val; }
void punkt::setY(double val){ this->y = val; }

void punkt::move(double x, double y){
    this->x += x;
    this->y += y;
}


void punkt::obrot(punkt a, double kat){
    double ax = a.getX();
    double ay = a.getY();

    this->setX(this->getX() - ax);
    this->setX(this->getX() * cos(kat) - this->getY() * sin(kat));
    this->setX(this->getX() + ax);
    
    this->setY(this->getY() - ay);
    this->setY(this->getX() * sin(kat) + this->getY() * cos(kat));
    this->setY(this->getY() + ay);
}


void punkt::print(){
    std::cout << "p.x = " << this->x << " p.y = " << this->y << std::endl;
}


odcinek::odcinek(){
    this->p1 = punkt();
    this->p2 = punkt();
}


// odcinek::odcinek(punkt p1, punkt p2): p1(p1), p2(p2){}
odcinek::odcinek(punkt p1, punkt p2){
    this->p1 = p1;
    this->p2 = p2;
}


odcinek::odcinek(const odcinek& o){
    this.p1 = o.p1;
    this.p2 = o.p2;
}


punkt odcinek::getP1(){ return this->p1; }
punkt odcinek::getP2(){ return this->p2; }


void odcinek::move(double x, double y){
    this->p1.move(x, y);
    this->p2.move(x, y);
}


void odcinek::obrot(punkt a, double kat){
    double ax = a.getX();
    double ay = a.getY();

    this->getP1().setX(this->getP1().getX() - ax);
    this->getP1().setX(this->getP1().getX() * cos(kat) - this->getP1().getY() * sin(kat));
    this->getP1().setX(this->getP1().getX() + ax);

    this->getP1().setY(this->getP1().getY() - ay);
    this->getP1().setY(this->getP1().getX() * sin(kat) + this->getP1().getY() * cos(kat));
    this->getP1().setY(this->getP1().getY() + ay);
    
    this->getP2().setX(this->getP2().getX() - ax);
    this->getP2().setX(this->getP2().getX() * cos(kat) - this->getP2().getY() * sin(kat));
    this->getP2().setX(this->getP2().getX() + ax);

    this->getP2().setY(this->getP2().getY() - ay);
    this->getP2().setY(this->getP2().getX() * sin(kat) + this->getP2().getY() * cos(kat));
    this->getP2().setY(this->getP2().getY() + ay);
}


double odcinek::length(){
    return sqrt(pow(this->getP2().getX() - this->getP1().getX(), 2) + 
                pow(this->getP2().getY() - this->getP1().getY(), 2));
}


bool odcinek::czyNalezy(punkt a){
    double ax = a.getX();
    double ay = a.getY();
    double p1x = this->getP1().getX();
    double p1y = this->getP1().getY();
    double p2x = this->getP2().getX();
    double p2y = this->getP2().getY();


    if((ax < p1x && ax < p2x) || (ay < p1y && ay < p2y) ||
       (ax > p1x && ax > p2x) || (ay > p1y && ay > p2y))
        return false;
    else{
        if(p2x*ay + p1x*p1y + ax*p1y - p1x*ay - p2x*p1y - ax*p2y <= 1e-9)
            return true;
    }       
    return false;
}


void odcinek::print(){
    std::cout << "p1 = "; 
    this->p1.print();
    std::cout << "p2 = "; 
    this->p2.print();
}


trojkat::trojkat(){
    this->p1 = punkt();
    this->p2 = punkt();
    this->p3 = punkt();
}   


trojkat::trojkat(punkt p1, punkt p2, punkt p3){
    double x1 = p1.getX();
    double x2 = p2.getX();
    double x3 = p3.getX();
    double y1 = p1.getY();
    double y2 = p2.getY();
    double y3 = p3.getY();
    
    if(fabs((y1 - y2) * (x1 - x3) - (y1 - y3) * (x1 - x2)) <= 1e-9){
        throw std::invalid_argument("Punkty trójkąta nie mogą być współliniowe!");
    }
    else{
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }
}


trojkat::trojkat(const trojkat& t){
    this->p1 = t.p1;
    this->p2 = t.p2;
    this->p3 = t.p3;
}


punkt trojkat::getP1(){ return this->p1; }
punkt trojkat::getP2(){ return this->p2; }
punkt trojkat::getP3(){ return this->p3; }


void trojkat::move(double x, double y){
    this->p1.move(x, y);
    this->p2.move(x, y);
    this->p3.move(x, y);
}


void trojkat::obrot(punkt a, double kat){
    double ax = a.getX();
    double ay = a.getY();

    this->getP1().setX(this->getP1().getX() - ax);
    this->getP1().setX(this->getP1().getX() * cos(kat) - this->getP1().getY() * sin(kat));
    this->getP1().setX(this->getP1().getX() + ax);

    this->getP1().setY(this->getP1().getY() - ay);
    this->getP1().setY(this->getP1().getX() * sin(kat) + this->getP1().getY() * cos(kat));
    this->getP1().setY(this->getP1().getY() + ay);
    
    this->getP2().setX(this->getP2().getX() - ax);
    this->getP2().setX(this->getP2().getX() * cos(kat) - this->getP2().getY() * sin(kat));
    this->getP2().setX(this->getP2().getX() + ax);

    this->getP2().setY(this->getP2().getY() - ay);
    this->getP2().setY(this->getP2().getX() * sin(kat) + this->getP2().getY() * cos(kat));
    this->getP2().setY(this->getP2().getY() + ay);

    this->getP3().setX(this->getP3().getX() - ax);
    this->getP3().setX(this->getP3().getX() * cos(kat) - this->getP3().getY() * sin(kat));
    this->getP3().setX(this->getP3().getX() + ax);

    this->getP3().setY(this->getP3().getY() - ay);
    this->getP3().setY(this->getP3().getX() * sin(kat) + this->getP3().getY() * cos(kat));
    this->getP3().setY(this->getP3().getY() + ay);
}


void trojkat::print(){
    std::cout << "p1 = "; this->p1.print();
    std::cout << "p2 = "; this->p2.print();
    std::cout << "p3 = "; this->p3.print();
}


int main(int argc, char **argv){
    
    // ========================PUNKT========================

    punkt p0;
    
    punkt p1(1, 2);
    punkt p2(2, 4);
    punkt p3(4, 1);
    std::cout << std::endl << "punkt.print()" << std::endl << std::endl;
    std::cout << "p1 "; p1.print();
    std::cout << "p2 "; p2.print();
    std::cout << "p3 "; p3.print();


    p1.move(2, 3);
    p2.move(-2, 1.5);
    p3.move(0, 1);
    std::cout << std::endl << "punkt.print() move" << std::endl << std::endl;
    std::cout << "p1 "; p1.print();
    std::cout << "p2 "; p2.print();
    std::cout << "p3 "; p3.print();

    p1.obrot(p0, 90);
    std::cout << std::endl << "punkt.print() obrot" << std::endl << std::endl;
    std::cout << "p1 "; p1.print();


    // ========================ODCINEK========================


    odcinek o1(p2, p3);
    std::cout << std::endl << "odcinek.print()" << std::endl << std::endl;
    o1.print();

    o1.move(2, -3);
    std::cout << std::endl << "odcinek.print() move" << std::endl << std::endl;
    o1.print();

    std::cout << std::endl << "długość odcinka o1 wynosi: " << o1.length() << std::endl << std::endl;


    odcinek o2(punkt(2, 3), punkt(4, 5));
    if(o2.czyNalezy(punkt(3, 4)))
        std::cout << "Punkt (3,4) należy do odcinka o2 (2,3)->(4,5)" << std::endl;


    o1.obrot(p0, 90);
    std::cout << std::endl << "odcinek.print() obrot" << std::endl << std::endl;
    o1.print();


    // ========================TROJKAT========================


    trojkat t1(p1, p2, p3);
    std::cout << std::endl << "trojkat.print()" << std::endl << std::endl;
    t1.print();
    
    t1.move(3, 3);
    std::cout << std::endl << "trojkat.print() move" << std::endl << std::endl;
    t1.print();
}