#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define ll long long int

ll fast_pow(ll a, ll b){
    if(b == 0) return 1;
    ll res = fast_pow(a, b/2);
    if(b % 2) return res * res * a;
    else return res * res;
}

int main(){
    ll no_of_cases;
    string s,t;
    cin >> no_of_cases;
    ll s_a_count, t_a_count;
    for(int i = 0; i < no_of_cases; i++){
        cin >> s >> t;
        bool flag_t = true;

        if(t.length() == 1 && t[0] == 'a'){
            cout << 1 << endl;
            continue;
        }

        for(int j = 0; j < t.length(); j++){
            if(t[j] == 'a'){
                cout << -1 << endl;
                flag_t = false;
                break;
            }
        }

        if(flag_t){
            cout << fast_pow(2, s.size()) << endl;
        }
    }
}