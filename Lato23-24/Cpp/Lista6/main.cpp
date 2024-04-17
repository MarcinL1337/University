#include "wielomian.hpp"


int main(){
    
    wielomian f({1,2,3});
    const double wsp[3] = {1, 3, 5};
    wielomian g(2, wsp);
    wielomian h;
    std::cin >> h; // 3 stopień;

    std::cout << "f(x) = " << f << "\n";
    std::cout << "g(x) = " << g << "\n";
    std::cout << "h(x) = " << h << "\n\n";


    std::cout << "f(x) + h(x) = " << f + h << "\n";
    std::cout << "f(x) = " << f << '\n';
    std::cout << "h(x) = " << h << '\n' << '\n';

    f += h;
    std::cout << "f(x) += h(x) -> f(x) = " << f << '\n' << '\n';

    f -= h;
    std::cout << "f(x) -= h(x) -> f(x) = " << f << '\n' << '\n';

    g *= 2;
    std::cout << "g(x) *= 2 -> g(x) = " << g << '\n' << '\n';


    std::cout << "g(x) * h(x) = " << g * h << "\n";
    std::cout << "h(x) - g(x) = " << h - g << "\n" << "\n";

    std::cout << "g(x) = " << g << "\n";
    std::cout << "g(3) = " << g(3) << "\n";
    std::cout << "h(x) = " << h << "\n";
    std::cout << "h(-2) = " << h(-2) << "\n" << "\n";

    std::cout << "h(x)[2] = " << h[2] << "\n" << "\n";

    std::cout << "Kopia h(x) do f(x): ";
    f = h;
    std::cout << f << "\n" << "\n";

    std::cout << "Przeniesienie f(x) do g(x): ";
    g = std::move(f);
    std::cout << g << "\n" << "\n";


    double zla_lista_wsp[3] = {10, 9, 0};
    wielomian zly_wielomian(2, zla_lista_wsp);
}