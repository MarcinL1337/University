#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    string cur;

    for(int i = 0; i < n; i++){
        bool is_zero = false;
        int even_count = 0;
        int digits_sum = 0;
        cin >> cur;
        int cur_len = cur.length();
        for(int j = 0; j < cur_len; j++){
            int cur_digit =  int(cur[j]) - '0';
            digits_sum += cur_digit;
            // cout << "char = " << cur[j] << " | int = " << cur_digit << endl;
            if(cur_digit == 0) is_zero = true;
            if((cur_digit % 2) == 0) even_count += 1;
        }
        // cout << is_even << is_zero << endl;
        // cout << "Digits sum = " << digits_sum << endl << endl;
        if(digits_sum == 0) cout << "red" << endl;
        else if(even_count > 1 && is_zero && (digits_sum % 3 == 0)) cout << "red" << endl;
        else cout << "cyan" << endl;
    }
}