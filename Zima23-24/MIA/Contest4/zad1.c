#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    int no_of_queries, no_of_kids;
    scanf("%d ", &no_of_queries);

    for(int i = 0; i < no_of_queries; i++){
        scanf("%d", &no_of_kids);
        int permutation[no_of_kids];

        for(int j = 0; j < no_of_kids; j++){
            scanf("%d ", &permutation[j]);
        }

        for(int j = 0; j < no_of_kids; j++){
            int cur_child = permutation[j];
            int counter = 1;
            while(cur_child != j + 1){
                counter++;
                cur_child = permutation[cur_child-1];
            }
            printf("%d ", counter);
        }
        printf("\n");
    }
}