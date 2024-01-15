#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int


int main(){
    int no_of_test_cases;
    cin >> no_of_test_cases;

    for(int i = 0; i < no_of_test_cases; i++){
        ll a, b, c;
        cin >> a >> b >> c;
        if(c % 2){
            if(a >= b)  cout << "First" << endl;
            else        cout << "Second" << endl;
        }
        else{
            if(a > b)   cout << "First" << endl;
            else        cout << "Second" << endl;
        }
    }
}