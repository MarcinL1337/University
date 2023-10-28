#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int no_of_hairs, no_of_requests, fav_num;

    cin >> no_of_hairs >> no_of_requests >> fav_num;
    long long int hair_lengths[100000];
    for(int i = 0; i < no_of_hairs; i++)
        cin >> hair_lengths[i];

    int j = 0, counter = 0, cur = hair_lengths[0];
    while(j < no_of_hairs){
        bool flag = false;
        while(j < no_of_hairs && cur > fav_num){
            cur = hair_lengths[++j];
            flag = true;
        }
        if(flag) counter++;
        while(j < no_of_hairs && cur <= fav_num)
            cur = hair_lengths[++j];
    }
    int t, p, d;
    for(int i = 0; i < no_of_requests; i++){
        cin >> t;
        if(t){
            cin >> p >> d;
            hair_lengths[p-1] += d;
            if((hair_lengths[p-1] > fav_num) && (hair_lengths[p-1] - d <= fav_num)){
                if((p-2 >= 0) && (hair_lengths[p-2] > fav_num) && (p < no_of_hairs) && (hair_lengths[p] > fav_num)) counter--;
                else if((p-2 >= 0) && (hair_lengths[p-2] <= fav_num) && (p < no_of_hairs) && (hair_lengths[p] <= fav_num)) counter++;
                else if((p-1 == 0) && (hair_lengths[p] <= fav_num)) counter++;
                else if((p-1 == no_of_hairs - 1) && (hair_lengths[p-2] <= fav_num)) counter++;
            }
        }
        else cout << counter << endl;
    }
}