#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, a, b, c;
    cin >> n >> a >> b >> c;
    int arr[] = {a, b, c};
    int low, mid, high;
    int size = sizeof(arr)/sizeof(arr[0]);
    sort(arr, arr+3);
    low = arr[0]; mid = arr[1]; high = arr[2];

    int counter = 0;
    while(n > 0){
        while(n > 0){
            n -= low;
            counter++;
        }
        if(n == 0){
            cout << counter << endl;
        }
        else if(n < 0){
            counter--;
            n += low;
        }
        while
    }    
}


/*
    1000, 11, 15, 20
    1000/11 = 90 + r10
    1000/15 = 66 + r10
    1000/20 = 50 + r0
*/