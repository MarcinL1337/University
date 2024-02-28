#include <stdio.h>
#include <stdlib.h>

int max_sum(int rows, int cols, int cells[rows][cols]){
    int max_res = 0;
    for(int x = 0; x < cols; x++){
        for(int y = 0; y < rows; y++){
            int x_pom = x, y_pom = y;
            int res = 0;
            while(x_pom < cols && y_pom < rows){
                res += cells[y_pom][x_pom];
                y_pom += 1;
                x_pom += 1;
            }        
            x_pom = x - 1;
            y_pom = y + 1;
            while(x_pom >= 0 && y_pom < rows){
                res += cells[y_pom][x_pom];
                y_pom += 1;
                x_pom -= 1;
            }
            x_pom = x + 1;
            y_pom = y - 1;
            while(x_pom < cols && y_pom >= 0){
                res += cells[y_pom][x_pom];
                x_pom += 1;
                y_pom -= 1;
            }
            x_pom = x - 1;
            y_pom = y - 1;
            while(x_pom >= 0 && y_pom >= 0){
                res += cells[y_pom][x_pom];
                x_pom -= 1;
                y_pom -= 1;
            }

            max_res = res > max_res ? res : max_res;
        }
    }
    return max_res;
}

int main(){
    int no_of_test_cases;
    scanf("%d", &no_of_test_cases);
    int rows, cols;
    printf("\n\n");

    for(int i = 0; i < no_of_test_cases; i++){
        scanf("%d %d", &rows, &cols);
        int cells[rows][cols];
        for(int j = 0; j < rows; j++){
            for(int k = 0; k < cols; k++)
                scanf("%d", &cells[j][k]);
        }
        // int sum = 0;
        // printf("%d %d %d\n\n", no_of_test_cases, rows, cols);
        // for(int i = 0; i < rows; i++){
        //     for(int j = 0; j < cols; j++){
        //         printf("%d ", cells[i][j]);
        //         sum += cells[i][j];
        //     }
        //     printf("\n");
        // }
        // printf("\n\n");
        // printf("SUM = %d\n\n\n", sum);
        // printf("Res%d = %d\n\n", i, max_sum(rows, cols, cells));
        // max_sum(rows, cols, cells);
        int res = max_sum(rows, cols, cells);
        printf("%d\n", res);
    }
}