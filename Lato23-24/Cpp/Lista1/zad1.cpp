#include <iostream>
#include <string>
#include <vector>
#include <utility>

const std::vector<std::pair<int, std::string>> roman = {
    {1000, "M"},
    {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
    {90, "XC"}, {50, "L"}, {40, "XL"}, {10, "X"},
    {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
};

std::string toRoman(int liczba){
    int8_t indeks = 0;
    std::string res = "";
    while(liczba > 0){
        for(;indeks<roman.size(); indeks++){
            if(roman.at(indeks).first <= liczba){
                liczba -= roman.at(indeks).first;
                res += roman.at(indeks).second;
                break;
            }
        }
    }
    return res;
}

int main(){
    for(;;){
        std::cout << "Podaj liczbę w zapisie arabskim z zakresu od 1 do 3999" << std::endl;
        std::string liczba_s;
        std::cin >> liczba_s;

        try{
            int liczba = stoi(liczba_s);
            if(liczba > 3999 or liczba < 1){
                std::cout << "Podana liczba jest niezgodna ze specyfikacją." << std::endl;
                continue;
            }
            std::cout << toRoman(liczba) << std::endl;
        }
        catch (std::invalid_argument const& ex){
            std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
        }
    }
}