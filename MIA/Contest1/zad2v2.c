#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(){
    int no_of_cubes;
    scanf("%d", &no_of_cubes);
    int possible_ints[100];

    int cur_face, faces_count[no_of_cubes][6];
    for(int i = 0; i < no_of_cubes; i++){
        for(int j = 0; j < 6; j++){
            scanf("%d ", &cur_face);
            faces_count[i][j] = cur_face;
        }
    }

    if(no_of_cubes == 1){
        for(int i = 0; i < 6; i++)
            possible_ints[faces_count[0][i]] = 1;
    }
    else if(no_of_cubes == 2){
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                possible_ints[faces_count[0][i] * 10 + faces_count[1][j]] = 1;
                possible_ints[faces_count[0][i] + faces_count[1][j] * 10] = 1;
            }
        }
    }
    else{
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 6; j++){
                for(int k = 0; k < 6; k++){
                    possible_ints[faces_count[0][k] * 10 + faces_count[1][j]] = 1;
                    possible_ints[faces_count[0][i] + faces_count[1][j] * 10] = 1;
                }
            }
        }
    }
}