#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define ll long long int

int main(){
    string input;
    char c;
    ll res = 0;
    int cur_digit;

    cin >> input;

    if((input[0] - '0') % 4 == 0) res++;

    for(int i = 1; i < input.length(); i++){
        cur_digit = (input[i] - '0');
        if(cur_digit % 4 == 0) res++;

        if((cur_digit + (input[i-1] - '0') * 10) % 4 == 0) res += i;
    }

    cout << res;
}