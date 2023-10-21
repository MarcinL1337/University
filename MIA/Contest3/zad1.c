#include <stdio.h>
#include <stdlib.h>

int main(){
    int no_of_cases;
    scanf("%d", &no_of_cases);

    int rows, cols;
    char cur_letter;
    for(int i = 0; i < no_of_cases; i++){
        scanf("%d %d", &rows, &cols);
        char carpet[rows][cols];
        int v_min = cols + 1, i_min = cols + 1, k_min = cols + 1, a_min = cols + 1;
        for(int j = 0; j < rows; j++)
            scanf("%s", carpet[j]);
        for(int j = 0; j < rows; j++){
            for(int k = 0; k <= cols; k++){
                if(carpet[j][k] == 'v' && k < v_min) v_min = k;
            }
            // printf("%s \n", carpet[j]);
        }
        if(v_min == cols+1){
            printf("no\n");
            continue;
        }
        
        for(int j = 0; j < rows; j++){
            for(int k = 0; k < cols; k++){
                if(carpet[j][k] == 'i' && k < i_min && k > v_min) i_min = k;
            }
        }
        if(i_min == cols+1){
            printf("no\n");
            continue;
        }

        for(int j = 0; j < rows; j++){
            for(int k = 0; k < cols; k++){
                if(carpet[j][k] == 'k' && k < k_min && k > v_min && k > i_min) k_min = k;
            }
        }
        if(k_min == cols+1){
            printf("no\n");
            continue;
        }

        for(int j = 0; j < rows; j++){
            for(int k = 0; k < cols; k++){
                if(carpet[j][k] == 'a' && k < a_min && k > v_min && k > i_min && k > k_min) a_min = k;
            }
        }
        if(a_min == cols+1){
            printf("no\n");
            continue;
        }

        if(v_min < i_min < k_min < a_min) printf("yes\n");
        else (printf("no\n"));
    }
}