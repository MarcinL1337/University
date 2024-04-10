#include "liczba.hpp"


int main(){
    liczba liczba1;
    std::cout << "Aktualna liczba w liczba1: " << liczba1.get_current() << std::endl << std::endl;
    liczba1.insert(420.69);
    std::cout << "Aktualna liczba w liczba1: " << liczba1.get_current() << std::endl << std::endl;
    liczba1.insert(123123123);
    std::cout << "Aktualna liczba w liczba1: " << liczba1.get_current() << std::endl << std::endl;
    std::cout << "Historia w liczba1: " << std::endl;
    liczba1.print_history();
    liczba1.insert(1337);
    std::cout << "Aktualna liczba w liczba1: " << liczba1.get_current() << std::endl << std::endl;
    std::cout << "Historia w liczba1: " << std::endl;
    liczba1.print_history();


    liczba liczba2(10);
    liczba2 = std::move(liczba1);
    
    std::cout << std::endl << "Historia w liczba2 po użyciu konstruktora przenoszącego: " << std::endl;
    liczba2.print_history();


    liczba liczba3;
    liczba3 = liczba2;

    liczba3.insert(3);

    std::cout << std::endl << "Aktualna liczba w liczba3 po użyciu konstruktora kopiującego: " << liczba3.get_current() << std::endl;
    std::cout << std::endl << "Historia w liczba3 po użyciu konstruktora kopiującego: " << std::endl;
    liczba3.print_history();

    std::cout << std::endl << "Historia w liczba2 po użyciu konstruktora kopiującego: " << std::endl;
    liczba2.print_history();
}