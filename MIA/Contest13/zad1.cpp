#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int


int main(){
    int array_length;
    cin >> array_length;

    ll cur_num, cur_max = -100000000;
    for(int i = 0; i < array_length; i++){
        cin >> cur_num;
        if(cur_num < 0 && cur_num > cur_max){
            cur_max = cur_num;
            continue;
        }
        if(cur_num == 0) continue;
        if(sqrt(cur_num) != (int)sqrt(cur_num)){
            if(cur_num > cur_max)
                cur_max = cur_num;
        }
    }

    cout << cur_max << endl;
}