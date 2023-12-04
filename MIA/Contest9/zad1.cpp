#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define ll long long int
int tab[10000] = {0};

int main(){
    int n, count = 0;
    cin >> n;
    for(int i = 1; i < n + 1; i++){
        cin >> tab[i];
    }
    for(int i = 1; i < n + 1; i++){
        if(tab[tab[tab[i]]] == i){
            cout << "YES" << endl;
            return 0;
        }
    }
    cout << "NO" << endl;
}