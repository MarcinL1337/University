#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int a_len, b_len;
    cin >> a_len >> b_len;

    vector<int> a, b;
    int cur_cin;
    for(int i = 0; i < a_len; i++){
        cin >> cur_cin;
        a.push_back(cur_cin);
    }
    for(int i = 0; i < b_len; i++){
        cin >> cur_cin;
        b.push_back(cur_cin);
    }

    sort(a.begin(), a.end());

    for(auto & b_elem : b){
        auto upper = upper_bound(a.begin(), a.end(), b_elem);
        cout << distance(a.begin(), upper) << " ";
    }

    return 0;
}