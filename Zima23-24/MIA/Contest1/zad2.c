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

    bool flag = true;
    int cur_to_check = 0;
    int dig1 = 0, dig2 = 0;
    while(flag){
        cur_to_check++;
        int temp = cur_to_check;
        
        if(cur_to_check < 10)
            flag = (faces_count[0][cur_to_check] || faces_count[1][cur_to_check] || faces_count[2][cur_to_check]);
        else{
            dig1 = cur_to_check % 10;
            dig2 = cur_to_check / 10;
            flag = false;
            for(int i = 0; i < 3; i++){
                if(faces_count[i][dig2]){
                    for(int j = 0; j < 3; j++){
                        if(faces_count[j][dig1] && j != i) flag = true;
                    }
                }
            }
        }    
    }

    printf("%d", cur_to_check-1);
}