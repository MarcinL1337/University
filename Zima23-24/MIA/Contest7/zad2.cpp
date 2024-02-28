#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define ll long long int

int main(){
    int n;
    cin >> n;
    ll res = 1;
    bool f1 = false, f2 = false, f3 = false, f4 = false;
    for(int i = 0; i < 5; i++){
        res *= (n-i) * (n-i);
        if(!f1 && !(res%2)){
            f1 = true;
            res/=2;
        }
        if(!f2 && !(res%3)){
            f2 = true;
            res/=3;
        }
        if(!f3 && !(res%4)){
            f3 = true;
            res/=4;
        }
        if(!f4 && !(res%5)){
            f4 = true;
            res/=5;
        }
    }
    cout << res;
}
