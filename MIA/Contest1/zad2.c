#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(){
    int no_of_cubes;
    scanf("%d", &no_of_cubes);

    int cur_face, faces_count[3][10] = {0};
    for(int i = 0; i < no_of_cubes; i++){
        for(int j = 0; j < 6; j++){
            scanf("%d ", &cur_face);
            faces_count[i][cur_face] = 1;
        }
    }

    bool used_cubes[3];

    long long int cur_to_check = 0, temp;
    bool flag = true;
    while(flag){
        used_cubes[0] = used_cubes[1] = used_cubes[2] = false;
        cur_to_check++;
        temp = cur_to_check;
        int aux_tab[10] = {0};
        while(temp > 0){
            aux_tab[temp%10]++;
            temp /= 10;
        }
        for(int i = 0; i < 10; i++){
            if(aux_tab[i] == 1){
                if(faces_count[0][i] && !used_cubes[0]) 
                    used_cubes[0] = true;
                else if(faces_count[1][i] && !used_cubes[1])
                    used_cubes[1] = true;
                else if(faces_count[2][i] && !used_cubes[2])
                    used_cubes[2] = true;
                else{
                    flag = false;
                    break;
                }
            }
            else if(aux_tab[i] == 2){
                if(faces_count[0][i] && faces_count[1][i] ||
                   faces_count[1][i] && faces_count[2][i] ||
                   faces_count[0][i] && faces_count[2][i])
                   continue;
            }
        }
    }

    printf("%lld", cur_to_check-1);
}