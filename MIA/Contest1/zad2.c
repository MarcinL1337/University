#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

    long long int cur_to_check = 0, temp;
    bool flag = true;
    while(flag){
        cur_to_check++;
        temp = cur_to_check;
        int aux_tab[10] = {0};
        while(temp > 0){
            aux_tab[temp%10]++;
            temp /= 10;
        }
        for(int i = 0; i < 10; i++){
            if(aux_tab[i] > (faces_count[0][i] + faces_count[1][i] + faces_count[2][i])){
                flag = false;
                break;
            }
        }
    }

    printf("%lld", cur_to_check-1);
}