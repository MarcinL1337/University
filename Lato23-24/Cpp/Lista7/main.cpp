#include "klasy.hpp"
#include "piksele.hpp"


int main(int argc, char **argv){
    kolor k(1, 2, 3);
    kolortransparentny kt(4, 5, 6, 150);
    kolornazwany kn(7, 8, 9, "nazwakoloru");
    kolortn ktn(1, 3, 6, "nazwakolorudwa", 100);


    std::cout << "k: r = " << k.get_r() << " g = " << k.get_g() << " b = " << k.get_b() << "\n\n";
    std::cout << "kt: r = " << kt.get_r() << " g = " << kt.get_g() << " b = " << kt.get_b() << " alpha = " << kt.get_alpha() << "\n\n";
    std::cout << "kn: r = " << kn.get_r() << " g = " << kn.get_g() << " b = " << kn.get_b() << " nazwa = " << kn.get_nazwa() << "\n\n";
    std::cout << "ktn: r = " << ktn.get_r() << " g = " << ktn.get_g() << " b = " << ktn.get_b() << " alpha = " << ktn.get_alpha() << " nazwa = " << ktn.get_nazwa() << "\n\n";


    ktn.jasnosc(5.0);
    std::cout << "ktn: r = " << ktn.get_r() << " g = " << ktn.get_g() << " b = " << ktn.get_b() << "\n\n";

    piksel p1(3, 4);
    pikselkolorowy p2(10, 20, "kolorowypiksel");

    std::cout << p2.calc_dist_down() << "\n";
    std::cout << p2.calc_dist_left() << "\n";
    std::cout << p2.calc_dist_right() << "\n";
    std::cout << p2.calc_dist_up() << "\n";


    p2.przesun(100, 500);
    std::cout << "x = " << p2.get_x() << " y = " << p2.get_y() << "\n\n";

    std::cout << pixel_distance(p1, p2) << " == " << pixel_distance(&p1, &p2) << "\n";
}