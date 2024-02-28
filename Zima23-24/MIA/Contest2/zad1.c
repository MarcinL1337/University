#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(){
    int length, programmers, athletes, total_seated = 0;
    scanf("%d %d %d\n", &length, &programmers, &athletes);
    char seats[length];
    scanf("%s\n", seats);
    int8_t prev_seated = 0; // 0 - none were seated or *, 1 - programmer, 2 - athlete
    for(int i = 0; i < length; i++){
        if(seats[i] == '*'){
            prev_seated = 0;
            continue;
        }
        if(prev_seated == 0){
            if(programmers > athletes && programmers > 0){
                programmers -= 1;
                prev_seated = 1;
                total_seated += 1;
            }
            else if(athletes >= programmers && athletes > 0){
                athletes -= 1;
                prev_seated = 2;
                total_seated += 1;
            }
        }
        else if (prev_seated == 1 && athletes > 0){
            athletes -= 1;
            total_seated += 1;
            prev_seated = 2;
        }
        else if (prev_seated == 2 && programmers > 0){
            programmers -= 1;
            total_seated += 1;
            prev_seated = 1;
        }
        else prev_seated = 0;
    }
    printf("%d", total_seated);
}