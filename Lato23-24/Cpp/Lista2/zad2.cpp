#include <iostream>
#include <string>
#include <vector>
#include <math.h>


std::vector<int64_t> rozklad(int64_t n){
    std::vector<int64_t> res;
    
    while(n % 2 == 0){
        n /= 2;
        res.push_back(2);
    }

    for(int i = 3; i <= int(sqrt(n)) + 1; i = i + 2){
        while(n % i == 0){
            n /= i;
            res.push_back(i);
        }
    }

    if(n > 2) res.push_back(n);

    return res;
}


int main(int argc, char **argv){

    if(argc <= 1){
        std::cerr << "Usage: ./zad2.cpp num1 num2 ... numn" << std::endl;
        return 0;
    }

    for(int i = 1; i < argc; i++){
        try{
            int64_t cur_num = std::stoll(argv[i]);
            
            if(cur_num < 2){
                if(cur_num > -2){
                    std::clog << "Ta liczba nie ma rozkładu na czynniki pierwsze! (" << cur_num << ")" << std::endl << std::endl;
                    continue;
                }
                else if(cur_num == INT64_MIN){
                    cur_num /= 2;
                    cur_num *= -1;
                    std::cout << INT64_MIN << " = -1 * " << 2 << " * ";
                }
                else{
                    std::cout << cur_num << " = -1 * ";
                    cur_num *= -1;
                }
            }
            else
                std::cout << cur_num << " = ";

            std::vector<int64_t> prime_fact = rozklad(cur_num);
            for(int j = 0; j < prime_fact.size()-1; j++){
                std::cout << prime_fact[j] << " * ";
            }
            std::cout << prime_fact[prime_fact.size()-1] << std::endl << std::endl;
        }
        catch(const std::invalid_argument& e) {
            std::clog << e.what() << " invalid argument!" << std::endl << std::endl;
        }
        catch(const std::out_of_range& e) {
            std::clog << e.what() << " argument out of range!" << std::endl << std::endl;
        }
    }
}