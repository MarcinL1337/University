#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int


int main(){
    int n;
    cin >> n;
    
    string cur_word;
    int cur_length;


    for(int i = 0; i < n; i++){
        cin >> cur_length;
        cin >> cur_word;
        string res = "";

        if(cur_length > 1){
            if(cur_word[0] == cur_word[1] || cur_word[1] > cur_word[0]){
                printf("%c%c\n", cur_word[0], cur_word[0]);
                continue;
            }
        }
        
        int it = 1;

        char aux2 = cur_word[0];
        res += aux2;

        while(it < cur_length && aux2 >= cur_word[it]){
            aux2 = cur_word[it];
            res += aux2;
            it++;
        }

        cout << res;
        reverse(res.begin(), res.end());
        cout << res;
        printf("\n");
    }
}