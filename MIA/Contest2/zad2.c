#include <stdlib.h>
#include <stdio.h>

int main(){
    long long int length, reverse_cost, invert_cost;
    scanf("%lld %lld %lld", &length, &reverse_cost, &invert_cost);
    char input_string[length];
    scanf("%s", input_string);
    long long int chunks_of_zeros = 0, chunks_of_ones = 0;
    long long int counter = 0;
    while(counter < length){
        if(input_string[counter] == '0'){
            counter += 1;
            chunks_of_zeros += 1;
            while(input_string[counter] == '0') counter += 1;
        }
        else{
            counter += 1;
            chunks_of_ones += 1;
            while(input_string[counter] == '1') counter += 1;
        }
    }
    
    long long int res;

    if(chunks_of_zeros == 0) res = 0;
    else if(chunks_of_zeros == 1) res = invert_cost;
    else{
        if(reverse_cost > invert_cost){
            res = chunks_of_zeros * invert_cost;
        }
        else{
            res = (chunks_of_zeros - 1) * reverse_cost + invert_cost;
        }
    }
    printf("%lld", res);
}