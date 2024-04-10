#include "kolejka.hpp"

int main(int argc, char **argv){
    kolejka kol1(3);

    kol1.wstaw("Ala");
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    kol1.wstaw("ma");
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    kol1.wstaw("kota");
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    kol1.wstaw("i");
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    kol1.wstaw("psa.");
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    kol1.usun();
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    kol1.usun();
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    kol1.usun();
    std::cout << "kol1 początek = " << kol1.zprzodu() << std::endl;
    kol1.wypisz();

    // std::cout << "Test usuwania z pustej kolejki: ";
    // kol1.usun();


    kol1.wstaw("1");
    kol1.wstaw("2");
    kol1.wstaw("3");
    
    std::cout << "Kopiowanie kol1 do kol2 \n\n";

    kolejka kol2(kol1);

    std::cout << "kol1: ";
    kol1.wypisz();
    std::cout << "kol2: ";
    kol2.wypisz();

    kolejka kol3;

    kol3 = std::move(kol2);

    std::cout << "Przenoszenie kol2 do kol3 \n\n";
    std::cout << "kol2: ";
    kol2.wypisz();
    std::cout << "kol3: ";
    kol3.wypisz();


    kolejka kol4 = {"a", "b", "c", "d"};
    std::cout << "initializer_list kol4: ";
    kol4.wypisz();
    std::cout << "Pierwszy element w kol4: " << kol4.zprzodu() << "\n";


    std::cout << "\nkonstruktor domyślny kol5: ";
    kolejka kol5;
    kol5.wypisz();
    kol5.wstaw("a");
    kol5.wypisz();
    kol5.wstaw("b");
    kol5.wypisz();
    std::cout << "dlugosc kol5 = " << kol5.dlugosc() << "\n";


    // std::cout << "Próba utworzenia kolejki o pojemności < 1: ";
    // kolejka kol6(-1);
}