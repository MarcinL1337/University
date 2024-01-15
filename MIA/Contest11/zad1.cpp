#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int


int main(){
    int no_of_snacks;
    int snacks_array[100001] = {0};
    
    cin >> no_of_snacks;

    int cur_desired_size = no_of_snacks;
    int cur_snack_size;
    for(int i = 0; i < no_of_snacks; i++){
        cin >> cur_snack_size;
        snacks_array[cur_snack_size] = 1;
        if(cur_snack_size == cur_desired_size){
            while(snacks_array[cur_desired_size]){
                cout << cur_desired_size << " ";
                cur_desired_size--;
            }
            cout << endl;
        }
        else
            cout << endl;
    }
}